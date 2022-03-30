class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> mp; // key存数的值，value存数的下标
        vector<int> res(2);
        for (int i = 0; i < nums.size(); i++) {
            if (mp.count(target - nums[i])) {
                res[0] = mp[target - nums[i]];
                res[1] = i;
                break;
            }
            mp[nums[i]] = i;
        }
        return res;
    }
};