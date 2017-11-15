#include "includes.h"

/* 
    Given a non-empty array of non-negative integers nums, the degree of this array is defined as the maximum frequency of any one of its elements.

    Your task is to find the smallest possible length of a (contiguous) subarray of nums, that has the same degree as nums.
*/

// idea
// 1. Computes frequency/occurrences of each int in vector
// 2. Computes the shortest range of ints with highest frequency/occurrences
int findShortestSubArray(vector<int>& nums) {
    unordered_map<int, vector<int>> freq;     // <value of element, index of occurrences>
    for(int i = 0; i < nums.size(); ++i)
        freq[nums[i]].push_back(i);
    
    int degree = 0;
    for(auto it = freq.begin(); it != freq.end(); ++it) 
        degree = max(degree, (int)(it->second.size()));
    
    int shortest_len = nums.size();
    for(auto it = freq.begin(); it != freq.end(); ++it) {
        if(it->second.size() == degree) 
            shortest_len = min(shortest_len, it->second.back() - it->second.front() + 1);
    }
    return shortest_len;
}