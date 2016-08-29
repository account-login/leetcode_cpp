#include <iostream>
#include <vector>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
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
        if (s.size() <= 1) {
            return 1;
        }

        int ans[s.size() + 1];
        ans[0] = 1;
        ans[1] = 1;
        for (size_t i = 2; i <= s.size(); i++) {
            ans[i] = ans[i - 1];
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

    CHECK(s.numDecodings("") == 1);
    CHECK(s.numDecodings("1") == 1);
    CHECK(s.numDecodings("12") == 2);
    CHECK(s.numDecodings("121") == 3);
    CHECK(s.numDecodings("1212") == 5);

    CHECK(s.numDecodings("27") == 1);
    CHECK(s.numDecodings("26") == 2);
}
#endif
