"""
Q2 [C, Python] A phoneme is a sound unit (similar to a character for text). We have an extensive pronunciation
dictionary (think millions of words). Below is a snippet:
ABACUS AE B AH K AH S
BOOK B UH K
THEIR DH EH R
THERE DH EH R
TOMATO T AH M AA T OW
TOMATO T AH M EY T OW
Given a sequence of phonemes as input (e.g. ["DH", "EH", "R", "DH", "EH", "R"]), find all the combinations of the words that
can produce this sequence (e.g. [["THEIR", "THEIR"], ["THEIR", "THERE"], ["THERE", "THEIR"], ["THERE", "THERE"]]). You can
preprocess the dictionary into a different data structure if needed.
"""

# for this question, I will first pre-process the pronounciation dictionary to use the phonemes as key, and the list of words as value 
# If we traverse from start to end, there will be cases where a substring phonemes ('TAH') can produce another word, and also part of 
# a bigger word (tomato). I'm using sliding window approach, increment the end pointer and find the combination from the beginning of the 
# phonemes to the end pointer, this way it accounts for all combinations. Store the words formed in a separate array, result[i] represent 
# all the combinations that can be formed up until phonemes[i]. Total combination that can be formed at'end pointer' location is all the combinations
# from [start: end], and all the combination that can be formed until 'start_pointer'. 

from collections import defaultdict

word_dict = [
    ['ABACUS', 'AE', 'B', 'AH', 'K', 'AH', 'S'],
    ['BOOK', 'B', 'UH', 'K'],
    ['THEIR', 'DH', 'EH', 'R'],
    ['THERE', 'DH', 'EH', 'R'],
    ['TOMATO', 'T', 'AH', 'M', 'AA', 'T', 'OW'],
    ['TOMATO', 'T', 'AH', 'M', 'EY', 'T', 'OW'],
]

def process_dictionary(sample_dict: list[list[str]]) -> dict:
    phonemes_dict = defaultdict(list)
    for word, *sequence in sample_dict:
        phonemes_key = "".join(sequence)
        phonemes_dict[phonemes_key].append(word)
    
    return phonemes_dict

def find_word_combos_with_pronunciation(phonemes: list[str]) -> list[list[str]]:
    phonemes_to_Word = process_dictionary(word_dict)
    #print(phonemes_to_Word)
    all_combinations = [[] for _ in range(len(phonemes) + 1)]
    all_combinations[0].append([]) # assume number of words that can be formed with 1 phoneme is 0, so empty list that can be later concatenated
    # print("after initialization")
    # print("all combinations: ", all_combinations)
    
    for end in range(len(phonemes)+1):
        # all combinations up until end pointer
        for start in range(end):
            phonemes_combination = "".join(phonemes[start:end])
            #print(phonemes_combination)
            if phonemes_combination in phonemes_to_Word:
                for word in phonemes_to_Word[phonemes_combination]:
                    # print("here")
                    # append this word to all other words that can be formed up to the start pointer
                    for prev_word in all_combinations[start]:
                        # concatenate list
                        #print("here")
                        all_combinations[end].append( prev_word + [word])
    
    #print(all_combinations)
    return all_combinations[-1]

test = ["DH", "EH", "R", "DH", "EH", "R"] #[DH", "EH", "R"]
print(find_word_combos_with_pronunciation(test))