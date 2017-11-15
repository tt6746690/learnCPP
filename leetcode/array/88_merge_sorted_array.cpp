#include "includes.h"

// https://leetcode.com/problems/merge-sorted-array/description/

// nums1.size() = m+n, 
// merge sorted array nums1 and nums2
void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {

    // dont have to consider cases where m = 0 or n = 0
    // since if i1 = -1, first while skipped, if i2 = -1, both while look skipped
    int i1 = m-1;   
    int i2 = n-1;
    int k = m+n-1;

    while (i1 >= 0 && i2 >= 0) {
        nums1[k--] = (nums1[i1] > nums2[i2]) ? nums1[i1--] : nums2[i2--];
    } // exists if either nums1 or nums2 exhausted

    // nums2 has element left, dump to nums1
    while (i2 >= 0 && k >= 0) {
        nums1[k--] = nums2[i2--];
    }

}

// time O(m+n) linear complexity
// space inplace, no space overhead