#include "includes.h"


// solution 1: by sorting
// time:  O(nlogn)  
// space: O(1)
int missingNumber1(vector<int>& nums) 
{  
    sort(nums.begin(), nums.end());
    // if sorted 
    // check if first and last position are valid 
    if (nums.front() != 0) return 0;
    if (nums.back() != nums.size()) return nums.size();
    
    // then check range (0, n) 
    for (int i = 0; i < nums.size()-1; ++i) {
        if(nums[i]+1 != nums[i+1]) return nums[i] + 1;
    }
    return -1;
}

// solution 2: with unorderd_set 
// time: O(n) amortized 
// space: O(n) 
int missingNumber2(vector<int>& nums) 
{
    unordered_set<int> S(nums.begin(), nums.end());
    for(int i = 0; i < nums.size() + 1; ++i) {
        if(S.find(i) == S.end()) return i;
    }
    return -1;
}

// solution 3: with bit manipulation
// index: 0  1  2  3
// value: 0  2  3  4
// idea is to xor length with xor of index and its value 
// missing = 4 ^ (0^0) ^ (1^2) ^ (2^3) ^ (3^4)
//         = (0^0) ^ (1) ^ (2^2) ^ (3^3) ^ (4^4)
//         = 1
// time: O(n)
// space: O(1)
int missingNumber3(vector<int>& nums) 
{
    int missing = nums.size();
    for(int i = 0; i < nums.size(); ++i) {
        missing ^= (i^nums[i]);
    }
    return missing;
}