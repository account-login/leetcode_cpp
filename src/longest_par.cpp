#include <iostream>
#include <algorithm>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/longest-valid-parentheses/


using namespace std;


class Solution {
public:
    static int longestValidParentheses(string &s) {
        int lpar = -1;  ///< position of left parenthesis

        for (int i = 0; i < s.size(); i++) {
            char ch = s[i];

            if (ch == '(') {
                lpar = i;
            } else if (ch == ')') {
                if (lpar >= 0) {
                    // kill matched parenthesis
                    s[lpar] = '.';
                    s[i] = '.';

                    // find prevous left perenthesis
                    do {
                        lpar--;
                    } while (lpar >= 0 && s[lpar] != '(');
                }
            }
        }

        /// \todo one pass algorithm?

        int ans = 0;
        int candidate = 0;
        for (char ch : s) {
            if (ch == '.') {
                candidate++;
            } else {
                ans = max(ans, candidate);
                candidate = 0;
            }
        }
        ans = max(ans, candidate);

        return ans;
    }
};


#ifdef RUN_TEST
TEST_CASE("32. Longest Valid Parentheses") {
    Solution s;

    string str = "(()";
    CHECK(s.longestValidParentheses(str) == 2);
    CHECK(str == "(..");

    str = ")()())";
    CHECK(s.longestValidParentheses(str) == 4);
    CHECK(str == ")....)");

    str = ")(()()))";
    CHECK(s.longestValidParentheses(str) == 6);
    CHECK(str == ")......)");

    str = "))()()))((()))(()";
    CHECK(s.longestValidParentheses(str) == 6);
    CHECK(str == "))....))......(..");

    str = string(100000, '(') + string(100000, ')');
    CHECK(s.longestValidParentheses(str) == 200000);

    str = string(100000, ')') + string(100000, '(');
    CHECK(s.longestValidParentheses(str) == 0);

    str = string(100001, '(') + string(100000, ')');
    CHECK(s.longestValidParentheses(str) == 200000);
}
#endif
