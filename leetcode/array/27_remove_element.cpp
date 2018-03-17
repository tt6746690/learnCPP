
/* 
    https://leetcode.com/problems/remove-element/description/
    Given an array and a value, remove all instances of that value 
    in-place and return the new length.

    Do not allocate extra space for another array, you must do this 
    by modifying the input array in-place with O(1) extra memory.

    The order of elements can be changed. It doesn't matter what you leave beyond the new length.

    Example:
    Given nums = [3,2,2,3], val = 3,
    Your function should return length = 2, with the first two elements of nums being 2.
*/


int removeElement1(vector<int>& nums, int val) {
    
    if(nums.size() == 0) return 0;
    if(nums.size() == 1) return nums[0] == val ? 0 : 1;

    int i = 0, j = nums.size()-1;
    while(i < j) {
        if(nums[i] == val) {
            nums[i] = nums[j];
            --j; 
            continue;
        } else {
            ++i;
        }
    }

    return nums[j] == val ? j : j+1;    // cases where i==j nad nums[i] == val
}

// 2 pointers is BETTER!
// idea is to scan the array and dump any value not equal to val to the front of the array. 
// time: O(n)
// space: O(1)
int removeElement2(vector<int>& nums, int val) {

    int i = 0;
    for (int j = 0; j < nums.size(); j++) {
        if(nums[j] != val) {
            nums[i] = nums[j];
            i++;
        }
    }
    return i;
}
    