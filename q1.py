"""
Q1 [C, Python] The probability of rain on a given calendar day in Vancouver is p[i], where i is the day's index. For
example, p[0] is the probability of rain on January 1st, and p[10] is the probability of precipitation on January 11
th. Assume the year has 365 days (i.e., p has 365 elements). What is the chance it rains more than n (e.g., 100) days in Vancouver?
Write a function that accepts p (probabilities of rain on a given calendar day) and n as input arguments and returns the
possibility of raining at least n days.
"""
import numpy as np

SIMULATION_PERIODS = 1000

# My solution focuses on calculating the expected days of rain over a 365 day period, and simulating the 365 day period many times. 
# I chose this approach because we have an unknown distribution: the probability of rain each day could be different, so I cannot model it 
# using a binomial distribution. 
# Simulation: 
# Simulate expected days of rain using random values, if a value is less than the the given probability of rain, then it "rains", 
# Final probability of raining at least n days is see how many simulation periods we have rain more than n days, and divide that by total simulation periods
def prob_rain_more_than_n(p: list[float], n: int) -> float:
    simulation_expected_rain_days = 0
    at_least_n_days_rain_all_periods = 0
    
    #simulate for 10 years
    for simulation in range(SIMULATION_PERIODS):
        
        # expected days of rain for 1 year
        simulation_values = np.random.rand(len(p))
        simulation_expected_rain_days = simulation_values < p
        
        # if at least n days of rain
        if simulation_expected_rain_days >= n:
            at_least_n_days_rain_all_periods += 1
    
    return float(at_least_n_days_rain_all_periods / SIMULATION_PERIODS)
    