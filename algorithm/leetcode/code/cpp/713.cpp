class Solution {
public:
    int numSubarrayProductLessThanK(vector<int>& nums, int k) {
        if (k <= 1) return 0;
        int res = 0, t = 1;
        for (int l = 0, r = 0; r < nums.size(); r++) {
            t *= nums[r];
            while (t >= k) {
                t /= nums[l];
                l++;
            }
            res += r - l + 1;
        }
        return res;
    }
};