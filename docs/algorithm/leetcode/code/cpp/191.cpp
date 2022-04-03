class Solution {
public:
    int hammingWeight(uint32_t n) {
        int i = 32, res = 0;

        while (i--) {
            res += n & 1;
            n >>= 1;
        }
        return res;
    }
};