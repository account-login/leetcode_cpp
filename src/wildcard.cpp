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

    // naive
    int find_word_naive(const char *str, const char *pattern) {
        const char *str_begin = str;
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

    bool memcmp_question_mark(const char *buf1, const char *buf2, size_t size) {
        for (size_t i = 0; i < size; i++) {
            if (buf1[i] == '?' || buf2[i] == '?') {
                continue;
            }
            if (buf1[i] != buf2[i]) {
                return false;
            }
        }

        return true;
    }

    int match_partial(const char *str, const char *pattern) {
        for (int ret = 0; ; ret++) {
            if (pattern[ret] == '*' || pattern[ret] == '\0') {
                return ret;
            }
            if (str[ret] == '\0') {
                return -1;
            }

            if (pattern[ret] == '?') {
                continue;
            }
            if (pattern[ret] != str[ret]) {
                return ret;
            }
        }
    }

    // KMP
    int find_word(const char *str, const char *pattern) {
        int pattern_len = 0;
        while (pattern[pattern_len] != '*' && pattern[pattern_len] != '\0') {
            pattern_len++;
        }

        // TODO: optimize this
        int jmp_table[pattern_len];
        for (int i = 0; i < pattern_len; i++) {
            jmp_table[i] = 1;
            for (int j = 1; j <= i; j++) {
                int size = i - j;
                if (memcmp_question_mark(pattern + j, pattern, size)) {
                    jmp_table[i] = j;
                    break;
                }
            }
        }

        const char *origin_str = str;
        while (true) {
            int prefix_len = match_partial(str, pattern);
            if (prefix_len < 0) {
                return -1;
            }

            if (prefix_len == pattern_len) {
                return str - origin_str + pattern_len;
            }
            str += jmp_table[prefix_len];
        }
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
    CHECK(s.isMatch("asdfasdfasdfasdfasdfasdfasdfasdf", "asdfa*sd*sdfasdf*asdf") == true);
    CHECK(s.isMatch("asdfasdfasdfasdfasdfasdfasdfasdf", "asdfa*sd*dasdfaXsdf*asdf") == false);

    CHECK(s.isMatch("b", "?*?") == false);
    CHECK(s.isMatch("b", "*?*?*") == false);
}
#endif
