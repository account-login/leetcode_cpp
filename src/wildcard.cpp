#include <iostream>
#include <string>
#include <cassert>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#endif

// https://leetcode.com/problems/wildcard-matching/


using namespace std;


class Solution {
public:
    bool isMatch(string s, string p) {
        const char *str = s.data();
        const char *pattern = p.data();
        const char *pattern_begin = pattern;

        // match first word
        int jmp = this->cmp_word(str, pattern);
        if (jmp >= 0) {
            str += jmp;
            pattern += jmp;

            // only one word, first word is last word
            if (pattern[0] == '\0') {
                if (str[0] != '\0') {
                    return false;
                } else {
                    return true;
                }
            }

            while (pattern[0] == '*') {
                pattern++;
            }
        } else {
            return false;
        }

        while (true) {
            size_t pos = p.find('*', pattern - pattern_begin);
            if (pos == string::npos) {
                // match last word
                size_t last_pattern_len = p.size() - (pattern - pattern_begin);
                if ((str - s.data()) + last_pattern_len > s.size()) {
                    // str is shorter than pattern
                    return false;
                }
                str = s.data() + (s.size() - (last_pattern_len));
                jmp = this->cmp_word(str, pattern);
                if (jmp < 0) {
                    return false;
                } else {
                    str += jmp;
                    assert(str[0] == '\0');
                    return true;
                }
            } else {
                // match middle word
                jmp = this->find_word(str, pattern);
                if (jmp < 0) {
                    return false;
                } else {
                    str += jmp;
                    pattern = pattern_begin + pos;
                    while (pattern[0] == '*') {
                        pattern++;
                    }
                }
            }
        }
    }

    int cmp_word(const char *str, const char *pattern) {
        for (size_t i = 0; ; i++) {
            if (pattern[i] == '\0' || pattern[i] == '*') {
                return i;
            }

            if (str[i] == '\0') {
                return -1;
            }
            if (pattern[i] != '?' && pattern[i] != str[i]) {
                return -2;
            }
        }
    }

    int find_word(const char *str, const char *pattern) {
        const char *str_begin = str;
        // naive
        do {
            int jmp = this->cmp_word(str, pattern);
            if (jmp >= 0) {
                return jmp + (str - str_begin);
            } else {
                str++;
            }
        } while (str[0] != '\0');

        return -1;
    }
};


#ifdef RUN_TEST
TEST_CASE("Wildcard Matching") {
    Solution s;

    CHECK(s.isMatch("", "") == true);
    CHECK(s.isMatch("aa", "aa") == true);
    CHECK(s.isMatch("a", "aa") == false);
    CHECK(s.isMatch("aa", "a") == false);
    CHECK(s.isMatch("asdf", "**") == true);
    CHECK(s.isMatch("asdf", "a*s*f") == true);
    CHECK(s.isMatch("bbasdf", "*asdf") == true);
    CHECK(s.isMatch("asdf", "asdf*") == true);
    CHECK(s.isMatch("asdfasdf", "asdf*") == true);
    CHECK(s.isMatch("asdf", "as*sf") == false);
    CHECK(s.isMatch("asdf", "as*d") == false);

    CHECK(s.isMatch("b", "?*?") == false);
    CHECK(s.isMatch("b", "*?*?*") == false);
}
#endif
