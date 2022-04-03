class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int n = matrix.size(), m = matrix[0].size();
        bool col0 = false, row0 = false;

        // find row-0
        for (int i = 0; i < m; i++)
            if (!matrix[0][i])
                row0 = true;

        // find col-0
        for (int i = 0; i < n; i++)
            if (!matrix[i][0])
                col0 = true;

        // find zero in matrix
        for (int i = 1; i < n; i++)
            for (int j = 1; j < m; j++)
                if (!matrix[i][j]) {
                    matrix[0][j] = matrix[i][0] = 0;
                }

        // col -> 0
        for (int i = 1; i < m; i++)
            if (!matrix[0][i])
                for (int j = 1; j < n; j++)
                    matrix[j][i] = 0;

        // row -> 0
        for (int i = 1; i < n; i++)
            if (!matrix[i][0])
                for (int j = 1; j < m; j++)
                    matrix[i][j] = 0;

        // if row-0 has zero, transform all of row-0 to zero
        if (row0)
            for (int i = 0; i < m; i++)
                matrix[0][i] = 0;
        
        // if col-0 has zero, transform all of col-0 to zero
        if (col0)
            for (int i = 0; i < n; i++)
                matrix[i][0] = 0;
    }
};