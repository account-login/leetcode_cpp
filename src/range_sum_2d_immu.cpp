#include <vector>
#include <algorithm>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/range-sum-query-2d-immutable/


using namespace std;


class NumMatrix {
public:
    vector<vector<int>> sum;

    explicit NumMatrix(const vector<vector<int>> &matrix)
        : sum(matrix)
    {
        int xlen = sum.size();
        if (xlen == 0) {
            return;
        }
        int ylen = sum[0].size();
        if (ylen == 0) {
            return;
        }

        int steps = xlen + ylen - 1;
        for (int i = 1; i < steps; i++) {
            for (int x = max(0, i - ylen + 1); x < min(i + 1, xlen); x++) {
                int y = i - x;
                assert(0 <= x && x < xlen);
                assert(0 <= y && y < ylen);

                if (x == 0) {
                    sum[x][y] += sum[x][y - 1];
                } else if (y == 0) {
                    sum[x][y] += sum[x - 1][y];
                } else {
                    sum[x][y] += sum[x - 1][y] + sum[x][y - 1] - sum[x - 1][y - 1];
                }
            }
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        assert(row1 <= row2);
        assert(col1 <= col2);
        row1--;
        col1--;

        int ret = sum[row2][col2];
        if (row1 >= 0) {
            ret -= sum[row1][col2];
        }
        if (col1 >= 0) {
            ret -= sum[row2][col1];
        }
        if (row1 >= 0 && col1 >= 0) {
            ret += sum[row1][col1];
        }

        return ret;
    }
};


// Your NumMatrix object will be instantiated and called as such:
// NumMatrix numMatrix(matrix);
// numMatrix.sumRegion(0, 1, 2, 3);
// numMatrix.sumRegion(1, 2, 3, 4);


#ifdef RUN_TEST
TEST_CASE("304. Range Sum Query 2D - Immutable") {
    auto mat = NumMatrix({});
    vector<vector<int>> sum = {};
    CHECK(mat.sum == sum);

    mat = NumMatrix({{}});
    sum = {{}};
    CHECK(mat.sum == sum);

    mat = NumMatrix({ { 1 } });
    sum = { { 1 } };
    CHECK(mat.sum == sum);

    mat = NumMatrix({ { 1, 2 } });
    sum = { { 1, 3 } };
    CHECK(mat.sum == sum);

    mat = NumMatrix({ { 1 }, { 2 } });
    sum = { { 1 }, { 3 } };
    CHECK(mat.sum == sum);

    mat = NumMatrix({
        { 1, 2 },
        { 3, 4 }
    });
    sum = {
        { 1, 3 },
        { 4, 10 }
    };
    CHECK(mat.sum == sum);

    mat = NumMatrix({
        { 1,  2,  5,  6 },
        { 3,  4,  7,  8 },
        { 9, 10, 11, 12 }
    });
    sum = {
        {  1,  3,  8, 14 },
        {  4, 10, 22, 36 },
        { 13, 29, 52, 78 }
    };
    CHECK(mat.sum == sum);

    CHECK(mat.sumRegion(0, 0, 0, 0) == 1);
    CHECK(mat.sumRegion(0, 0, 1, 0) == 4);
    CHECK(mat.sumRegion(0, 0, 0, 1) == 3);
    CHECK(mat.sumRegion(0, 0, 1, 1) == 10);
    CHECK(mat.sumRegion(1, 1, 1, 1) == 4);
    CHECK(mat.sumRegion(0, 1, 1, 1) == 6);
    CHECK(mat.sumRegion(1, 0, 1, 1) == 7);
    CHECK(mat.sumRegion(0, 0, 2, 3) == 78);

    mat = NumMatrix({
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
        {1, 0, 3, 0, 5}
    });
    CHECK(mat.sumRegion(2, 1, 4, 3) == 8);
}
#endif
