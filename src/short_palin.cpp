#include <iostream>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/shortest-palindrome/


using namespace std;


class Solution {
public:
    string shortestPalindrome(const string &s) {
        for (auto i = s.size(); i >= 0; i--) {
            if (is_palindrome(s, i)) {
                string ret(s.rbegin(), s.rbegin() + (s.size() - i));
                ret += s;
                return ret;
            }
        }

        assert(0);
        return "";  // suppress -Wreturn
    }

    bool is_palindrome(const string &s, size_t len) {
        for (size_t i = 0; i < len / 2; i++) {
            if (s[i] != s[len - i - 1]) {
                return false;
            }
        }

        return true;
    }
};


#ifdef RUN_TEST
TEST_CASE("214. Shortest Palindrome") {
    Solution s;

    CHECK(s.is_palindrome("", 0) == true);
    CHECK(s.is_palindrome("a", 1) == true);
    CHECK(s.is_palindrome("ba", 2) == false);

    CHECK(s.shortestPalindrome("") == "");
    CHECK(s.shortestPalindrome("a") == "a");
    CHECK(s.shortestPalindrome("as") == "sas");
    CHECK(s.shortestPalindrome("asdfqwer") == "rewqfdsasdfqwer");
    CHECK(s.shortestPalindrome("asad") == "dasad");
    CHECK(s.shortestPalindrome("asdfdsa") == "asdfdsa");
}
#endif
