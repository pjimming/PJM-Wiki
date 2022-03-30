class Solution {
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
        // 两矩形面积和
        int tot = (ax2 - ax1) * (ay2 - ay1) + (bx2 - bx1) * (by2 - by1);

        // 重合矩形
        int x = min(ax2, bx2) - max(ax1, bx1);  // x轴处投影
        int y = min(ay2, by2) - max(ay1, by1);  // y轴处投影
        if (x > 0 && y > 0) tot -= x * y;   // 若存在重合矩形，tot减去一个重合矩形面积

        return tot;
    }
};