class Solution {
public:
    bool check(vector<int>& weights, int weight, int days) {
        int cnt = 1, t = 0;
        for (int i = 0; i < weights.size(); i++) {
            if (weight < weights[i]) return false;
            if (t + weights[i] > weight) {
                t = 0;
                cnt++;
            }
            t += weights[i];
        }
        return cnt <= days;
    }

    int shipWithinDays(vector<int>& weights, int days) {
        int l = 1, r = 25000000;
        while (l < r) {
            int mid = l + r >> 1;
            if (check(weights, mid, days)) r = mid;
            else l = mid + 1;
        }
        return l;
    }
};