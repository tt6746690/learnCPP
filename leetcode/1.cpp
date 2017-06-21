#include <vector> 
#include <iostream>

using namespace std;

vector<int> twoSum(vector<int>& nums, int target) {
    
    vector<int> indexes;

    for(vector<int>::size_type i = 0; i != nums.size(); i++){
        for(vector<int>::size_type j = i + 1; j != nums.size(); j++){
            if( (nums[j] + nums[i]) == target){
                indexes.push_back(i);
                indexes.push_back(j);
                return indexes;
            }
        }

    }
    return indexes;
}

    
class Solution {
    public:
        vector<int> twoSum(vector<int>& nums, int target) {
            unordered_map<int, int> lookup;
            for (int i = 0; i < nums.size(); ++i) {
                if (lookup.count(target - nums[i])) {
                                    return {lookup[target - nums[i]], i};
                                                
                }
                            lookup[nums[i]] = i;
                                    
            }
                    return {};
                                
        }
        
};

int main(){

    vector<int> nums;
    nums.push_back(3);
    nums.push_back(2);
    nums.push_back(4);
    int target = 6;
    
    for(vector<int>::size_type i = 0; i != nums.size(); i++){
        cout << "before: " << nums[i] << endl;
    }

    vector<int> r = twoSum(nums, target);

    for(vector<int>::size_type i = 0; i != r.size(); i++){
        cout << "after: " << r[i] << endl;
    }
                                
}
                
