class Solution {
public:
    bool find132pattern(vector<int>& nums) {
        int right = INT_MIN;    // 起初定义为最小值，保证单调性
        stack<int> s;
        for (int i = nums.size() - 1; i >= 0; i--) {
            if (nums[i] < right) return true;
            while (!s.empty() && s.top() < nums[i]) {
                right = max(right, s.top());
                s.pop();
            }
            s.push(nums[i]);
        }
        return false;
    }
};