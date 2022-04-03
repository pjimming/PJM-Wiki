class Solution {
public:
    int add(int a, int b) {
        while (b != 0) {
            int sum = a ^ b;
            int carry = (unsigned int)(a & b) << 1;
            // C++ 不支持负数移位，unsigned int 转化成无符号数。
            
            a = sum, b = carry;
        }
        return a;
    }
};