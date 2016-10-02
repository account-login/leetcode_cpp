#include <algorithm>
#include <vector>
#include <queue>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/maximal-rectangle/


using namespace std;


class Solution {
public:
    int maximalRectangle(const vector<vector<char>> &matrix) {
        int xlen = matrix.size();
        if (xlen == 0) {
            return 0;
        }
        int ylen = matrix[0].size();
        assert(ylen > 0);

        int ans = 0;
        for (int x1 = 0; x1 < xlen; x1++) {
            queue<int> q;
            for (int i = 0; i < ylen; i++) {
                q.push(i);
            }

            for (int x2 = x1; x2 < xlen && !q.empty(); x2++) {
                int max_y_width = 0;
                int y_width = 0;
                int last_y = -1;

                int qsize = q.size();
                for (int i = 0; i < qsize; i++) {
                    int y = q.front();

                    if (last_y >= 0 && last_y + 1 != y) {
                        max_y_width = max(max_y_width, y_width);
                        y_width = 0;
                    }

                    if (matrix[x2][y] == '1') {
                        q.push(y);
                        y_width++;
                    } else {
                        max_y_width = max(max_y_width, y_width);
                        y_width = 0;
                    }

                    last_y = y;
                    q.pop();
                }
                max_y_width = max(max_y_width, y_width);

                int area = (x2 - x1 + 1) * max_y_width;
                ans = max(ans, area);
            }
        }

        return ans;
    }
};


#ifdef RUN_TEST
TEST_CASE("85. Maximal Rectangle") {
    Solution s;

    CHECK(s.maximalRectangle({}) == 0);

    auto make_mat = [](const vector<string> &input) {
        vector<vector<char>> ans;
        for (auto &str : input) {
            ans.emplace_back(str.begin(), str.end());
        }

        return ans;
    };

    auto M = make_mat({
        "10100",
        "10111",
        "11111",
        "10010"
    });
    CHECK(s.maximalRectangle(M) == 6);

    M = make_mat({
        "00000000000",
        "00000000000",
        "00000000000",
        "00000000000",
        "00000000000",
        "00000000000",
        "00000000000",
        "00000000000",
    });
    CHECK(s.maximalRectangle(M) == 0);


    M = make_mat({
        "00000000000",
        "01111011100",
        "01111111100",
        "00011111000",
        "00111111110",
        "00111001110",
        "00001000000",
        "00000000000",
    });
    CHECK(s.maximalRectangle(M) == 15);

    M = make_mat({
        "11111111111",
        "11111111111",
        "11111111111"
    });
    CHECK(s.maximalRectangle(M) == 33);

    M = make_mat({
        "00010111",
        "01100101",
        "10111101",
        "00010000",
        "00100010",
        "11100111",
        "10011001",
        "01001100",
        "10010000"
    });
    CHECK(s.maximalRectangle(M) == 4);
}
#endif
