#include <iostream>
#include <string>
#include <cctype>
#include <climits>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/string-to-integer-atoi/


using namespace std;


class Solution {
public:
    int myAtoi(string str) {
        const char *s = str.data();
        while (isspace(*s)) {
            s++;
        }

        int sign = 1;
        if (*s == '-') {
            sign = -1;
            s++;
        } else if (*s == '+') {
            sign = 1;
            s++;
        }

        long long ans = 0;
        while (isdigit(*s)) {
            ans *= 10;
            ans += *s - '0';
            s++;

            // check overflow
            if (sign > 0) {
                if (ans > INT_MAX) {
                    return INT_MAX;
                }
            } else {
                if (-ans < INT_MIN) {
                    return INT_MIN;
                }
            }
        }

        return ans * sign;
    }
};


#ifdef RUN_TEST
TEST_CASE("8. String to Integer (atoi)") {
    Solution s;

    CHECK(s.myAtoi("0") == 0);
    CHECK(s.myAtoi("0123") == 123);
    CHECK(s.myAtoi("-1234") == -1234);
    CHECK(s.myAtoi("+4321") == 4321);

    CHECK(s.myAtoi("asdf") == 0);
    CHECK(s.myAtoi("-0012asdf") == -12);
    CHECK(s.myAtoi("") == 0);

    CHECK(s.myAtoi(to_string((long long)INT_MAX + 1)) == INT_MAX);
    CHECK(s.myAtoi(to_string((long long)INT_MIN - 1)) == INT_MIN);
}
#endif
