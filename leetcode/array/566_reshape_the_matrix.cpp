#include "includes.h"


// inplace 
// time O(r+c)
// space O(r+c) for the output matrix contructed
vector<vector<int>> matrixReshape(vector<vector<int>>& nums, int r, int c) 
{
    // reshaping is not possible in this case
    if(nums.empty() || r*c != nums.size() * nums[0].size()) return nums;
    
    // allocate enough space for the output matrix
    vector<vector<int>> out(r);
    for(auto& row : out) { row.resize(c); }
    
    int added = 0;
    for(const auto& row : nums) {
        for(int elem : row) {
            out[added/c][added%c] = elem;
            added += 1;
        }
    }
    return out;
}