#include "includes.h"


/*
https://leetcode.com/problems/remove-duplicates-from-sorted-array/description/

Given a sorted array, remove the duplicates in-place such that 
each element appear only once and return the new length.

Do not allocate extra space for another array, you must do this by
modifying the input array in-place with O(1) extra memory.

Example:
Given nums = [1,1,2],

Your function should return length = 2, with the first two elements of nums 
being 1 and 2 respectively. It doesn't matter what you leave beyond the new length.
*/

// idea
// 2 pointers, l, r starts from beginning
// increment r if l,r points to duplicates (note this is just continue in a for loop)
// otherwise, copy over nums[l] to nums[l+1] then l++
// keep on going until r reaches end 
int removeDuplicates(vector<int>& nums) {
    if(nums.size() == 0) return 0;
    if(nums.size() == 1) return 1;
    
    int l = 0;
    int r = 1;
    
    for(; r < nums.size(); ++r) {
        if(nums[l] != nums[r]) {
            l++;
            nums[l] = nums[r];
        }
    }
    
    return l+1;
}