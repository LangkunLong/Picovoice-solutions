//Find the n most frequent words in the TensorFlow Shakespeare dataset. 
//The dataset is available at https://storage.googleapis.com/download.tensorflow.org/data/shakespeare.txt.

// Solution:
// I will preprocess the text by removing all special characters and converting all words to lowercase.
// Then i will use a hashtable to store the frequency of each word 
// then i will sort the hashtable by frequency and return the n most frequent words 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_WORD_SIZE 100 // assume maximum size of a word is 100 bytes
#define MAX_TABLE_SIZE 1000000 // assume maximum size of hashtable is 1 million

typedef struct Node{
    char word[MAX_WORD_SIZE];
    int frequency;
    struct Node *next;
}Word_count;

Word_count *hashtable[MAX_TABLE_SIZE]; // hashtable of word_count pointers, for separate chaining

// djb2 hash function: http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word){
    unsigned long hash = 5381;
    int c;
    while (c = *word++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

//insert word if previously not present, else increment frequency
void hashmap_insert(char * new_word){
    unsigned int hash_index = hash(new_word);
    Word_count *head_ptr, *prev_ptr = hashtable[hash_index];
    while(head_ptr != NULL){
        if (strcmp(head_ptr->word, new_word) == 0){
            head_ptr->frequency ++;
            return;
        }else{
            prev_ptr = head_ptr;
            head_ptr = head_ptr->next;
        }
    }
    // reached end of the list, or no list present, insert new node
    Word_count *new_node = (Word_count*)malloc(sizeof(Word_count));
    new_node->frequency = 1;
    new_node->next = NULL;
    strcmp(new_node->word, new_word);
    prev_ptr ->next = new_node;
}

// get rid of special characters and convert to lowercase
void preprocess_word(char *word){
    int write_index = 0;
    for(int i = 0; word[i] != '\0'; i++){
        if (!ispunct(word[i])) {
            word[write_index] = tolower(word[i]);
            write_index++;
        }
    }
}

// read .txt and insert each word to hashtable
void process_file(const char *file_path){
    FILE *file_Ptr = fopen(file_path, "r");
    if(file_Ptr == NULL){
        printf("Error: Could not open file\n");
        return;
    }
    char word[MAX_WORD_SIZE];
    while(fscanf(file_Ptr, "%s", word) != EOF){
        preprocess_word(word);
        if (strlen(word) > 0){
            hashmap_insert(word);
        }
    }
}

// this function counts all the valid word entries in the hashmap, this is used to 
// allocate memory for the array to collect all the word entries 
int count_all_entries(){
    int total_count = 0;
    for(int i = 0; i < MAX_TABLE_SIZE; i++){
        Word_count *cur_ptr = hashtable[i];
        while (cur_ptr != NULL){
            if (cur_ptr ->frequency > 0){
                total_count++;
            }
            cur_ptr = cur_ptr->next;
        }
    }
    return total_count;
}

// stores all valid entires in 'entry_array', later to be sorted 
void get_all_hashtable_entries(Word_count* entry_array[]){
    int index = 0;
    for (int i = 0; i < MAX_TABLE_SIZE; i++){
        Word_count *cur_ptr = hashtable[i];
        while(cur_ptr != NULL){
            if (cur_ptr->frequency > 0){
                entry_array[index] = cur_ptr;
                index++;
            }
            cur_ptr = cur_ptr->next;
        }
    }
}

// comparison function:
// sort in descending order
int compare(const void *a, const void *b){
    int freq_1 = ((Word_count*)a) ->frequency;
    int freq_2 = ((Word_count*)b) ->frequency;
    return freq_2 - freq_1;
}

char **find_frequent_words(const char *path, int32_t n){
    // read the file, process each word, and insert into hashtable
    process_file(path);
    
    // need to collect all hashtable entries to a sortable object like an array
    int entry_count = count_all_entries();
    Word_count *all_entry_array = (Word_count*)malloc(entry_count * sizeof(Word_count));
    get_all_hashtable_entries(all_entry_array);

    // quicksort
    qsort(all_entry_array, entry_count, sizeof(Word_count), compare);

    // n frequent words
    char **frequent_words = (char**)malloc(n* sizeof(char*)); //array of pointers
    printf("%d n most frequent words: \n");
    for (int i = 0; i < n; i++){
        frequent_words[i] = (char*)malloc((strlen(all_entry_array[i].word) + 1) * sizeof(char));
        strcpy(frequent_words[i], all_entry_array[i].word);
        printf("%s, %d\n", all_entry_array[i].word, all_entry_array[i].frequency);
    }
    
    //free memory
    free(all_entry_array); // free storage array
    for(int i = 0; i < MAX_TABLE_SIZE; i++){
        Word_count *head_ptr = hashtable[i];
        while(head_ptr != NULL){
            Word_count *temp = head_ptr;
            head_ptr = head_ptr->next;
            free(temp);
        }
    }
    free(hashtable);

    return frequent_words; 
} 

int main(){
    int frequent_word = 5;
    char ** n_frequent_words = find_frequent_words("text.txt", frequent_word);

    for (int i = 0; i < frequent_word; i++) {
        free(n_frequent_words[i]);
    }
    free(n_frequent_words);
}