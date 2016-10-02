#include <algorithm>
#include <vector>
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
            vector<char> dp(ylen, true);

            bool rect_found;
            for (int x2 = x1; x2 < xlen; x2++) {
                int y_begin = -1;
                rect_found = false;
                for (int y = 0; y < ylen; y++) {
                    dp[y] = dp[y] && (matrix[x2][y] == '1');

                    if (dp[y]) {
                        rect_found = true;
                        if (y_begin < 0) {
                            y_begin = y;
                        }
                    } else {
                        // max rect found
                        if (y_begin >= 0) {
                            int area = (x2 - x1 + 1) * (y - y_begin);
                            ans = max(ans, area);
                            y_begin = -1;
                        }
                    }
                }

                if (!rect_found) {
                    // stop searching x2
                    break;
                }

                // last rect
                if (y_begin >= 0) {
                    int area = (x2 - x1 + 1) * (ylen - y_begin);
                    ans = max(ans, area);
                }
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
}
#endif
