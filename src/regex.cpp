#include <iostream>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/regular-expression-matching/


using namespace std;


class Solution {
public:
    static bool isMatch(const string &s, const string &p) {
        return isMatch(s.data(), p.data());
    }

    static bool isMatch(const char *str, const char *pattern) {
        // pattern ended
        if (pattern[0] == '\0') {
            return str[0] == '\0';
        }

        // optimization, not necessary.
        if (str[0] == '\0') {
            return is_match_empty(pattern);
        }

        int si = 0;
        int pi = 0;

        while (true) {
            if (pattern[pi + 1] != '*') {
                // str[si] == '\0' exited here
                if (!char_eq(str[si], pattern[pi])) {
                    return false;
                }

                si++;
                pi++;
                // pattern ended
                if (pattern[pi] == '\0') {
                    return str[si] == '\0';
                }
            } else {
                char pch = pattern[pi];
                pi += 2;

                if (isMatch(&str[si], &pattern[pi])) {
                    return true;
                }

                // str[si] == '\0' exited here
                while (char_eq(str[si], pch)) {
                    si++;

                    if (isMatch(&str[si], &pattern[pi])) {
                        return true;
                    }
                }

                return false;
            }
        }
    }

    static inline bool char_eq(char c, char p) {
        return c != '\0' && (p == '.' || c == p);
    }

    static inline bool is_match_empty(const char *pattern) {
        while (*pattern != '\0') {
            if (pattern[1] != '*') {
                return false;
            }
            pattern += 2;
        }

        return true;
    }
};


#ifdef RUN_TEST
TEST_CASE("10. Regular Expression Matching") {
    Solution s;

    CHECK(s.isMatch("aa","a") == false);
    CHECK(s.isMatch("a", "aa") == false);
    CHECK(s.isMatch("aa","aa") == true);
    CHECK(s.isMatch("aaa","aa") == false);
    CHECK(s.isMatch("aa", "a*") == true);
    CHECK(s.isMatch("aa", ".*") == true);
    CHECK(s.isMatch("ab", ".*") == true);
    CHECK(s.isMatch("aab", "c*a*b") == true);

    CHECK(s.isMatch("abc*d", "a*.**d") == true);

    CHECK(s.isMatch("aabbccddeeff","a*abbc*d*eee*.f") == true);
    CHECK(s.isMatch("aabbccddeeff","a*abx*bc*d*eee*.f") == true);

    char p[] = { '\0', '*', 'a', 'b', 'c', '\0' };
    CHECK(s.isMatch("", p) == true);
    CHECK(s.isMatch("abc", p) == false);
    CHECK(s.isMatch("", "a") == false);

    CHECK(s.isMatch("a", "ab*") == true);
    CHECK(s.isMatch("", "b*") == true);
    CHECK(s.isMatch("", "b*a") == false);

    CHECK(s.isMatch("", ".") == false);
}
#endif
