#include <iostream>
#include <vector>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/decode-ways/


using namespace std;


class Solution {
public:
    int numDecodings(const string &s) {
        if (s.size() == 0) {
            return 0;
        }

        int ans[s.size() + 1];
        ans[0] = 1;
        if ('1' <= s[0] && s[0] <= '9') {
            ans[1] = 1;
        } else {
            return 0;
        }

        for (size_t i = 2; i <= s.size(); i++) {
            ans[i] = 0;

            if ('1' <= s[i - 1] && s[i - 1] <= '9') {
                ans[i] += ans[i - 1];
            } else if (s[i - 1] != '0') {
                return 0;
            }

            if (s[i - 2] == '1' || (s[i - 2] == '2' && s[i - 1] <= '6')) {
                ans[i] += ans[i - 2];
            }
        }

        return ans[s.size()];
    }
};


#ifdef RUN_TEST
TEST_CASE("91. Decode Ways") {
    Solution s;

    CHECK(s.numDecodings("0") == 0);
    CHECK(s.numDecodings("") == 0);
    CHECK(s.numDecodings("01") == 0);

    CHECK(s.numDecodings("10") == 1);

    CHECK(s.numDecodings("1") == 1);
    CHECK(s.numDecodings("12") == 2);
    CHECK(s.numDecodings("121") == 3);
    CHECK(s.numDecodings("1212") == 5);

    CHECK(s.numDecodings("27") == 1);
    CHECK(s.numDecodings("26") == 2);
}
#endif
