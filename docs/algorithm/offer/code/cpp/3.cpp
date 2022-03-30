class Solution {
public:
    int findRepeatNumber(vector<int>& nums) {
        int n = nums.size();
        unordered_set<int> S;

        int res = -1;
        for (int i = 0; i < n; i++) {
            if (S.count(nums[i])) {
                res = nums[i];
                break;
            }
            S.insert(nums[i]);
        }

        return res;
    }
};