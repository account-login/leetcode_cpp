#include <iostream>
#include <string>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/wildcard-matching/


using namespace std;


class Solution {
public:
    bool isMatch(const string &s, const string &p) {
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
                jmp = this->find_word_kmp(str, pattern);
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

    /**
     * @brief Find out weither str starts with first word of \a pattern.
     *
     *        \a pattern is a sequence of word seperated by '*'.
     *        A word may contain '?'.
     * @param str
     * @param pattern
     * @return return the length of first word of pattern if str starts with
     *         first word of \a pattern, otherwise return a negative number.
     */
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

    /**
     * @brief search the first word of \a pattern in \a str. A naive method.
     * @param str
     * @param pattern
     * @return return the offset from \a str to the end of matched word on success,
     *         return a negative number on fail.
     */
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

    /**
     * @brief memcmp() that ignore '?' on both buffer. Used by KMP algorithm.
     * @param buf1
     * @param buf2
     * @param size
     * @return return weither \a buf1 equals \a buf2.
     */
    bool memcmp_qm(const char *buf1, const char *buf2, size_t size) {
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

    /**
     * @brief Find out the longest prefix of \a str that matches the prefix of \a pattern.
     * @param str
     * @param pattern
     * @param start   skip first n characters of \a str. Used by KMP algorithm.
     * @return return the length of matched prefix.
     *         return a negative number on fail.
     */
    int match_partial(const char *str, const char *pattern, int start = 0) {
        for (int ret = start; ; ret++) {
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

    /**
     * @brief search the first word of \a pattern in \a str using KMP algorithm.
     * @param str
     * @param pattern
     * @return return the offset from \a str to the end of matched word on success,
     *         return a negative number on fail.
     */
    int find_word_kmp(const char *str, const char *pattern) {
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
                if (memcmp_qm(pattern + j, pattern, size)) {
                    jmp_table[i] = j;
                    break;
                }
            }
        }

        const char *origin_str = str;
        int start = 0;
        while (true) {
            int prefix_len = match_partial(str, pattern, start);
            if (prefix_len < 0) {
                return -1;
            }

            // the whole word matched, success.
            if (prefix_len == pattern_len) {
                return str - origin_str + pattern_len;
            }

            // go to the next posible matched position
            int jmp = jmp_table[prefix_len];
            str += jmp;

            // skip matched prefix
            if (prefix_len > 0) {
                start = prefix_len - jmp;
            } else {
                start = 0;
            }
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
    CHECK(s.isMatch("asdfasdfasdfasdfasdfasdfasdfasdfasdfasdf", "asdfa*sd*sdfasdfasdf*asdf") == true);
    CHECK(s.isMatch("asdfasdfasdfasdfasdfasdfasdfasdf", "asdfa*sd*dasdfaXsdf*asdf") == false);

    CHECK(s.isMatch("b", "?*?") == false);
    CHECK(s.isMatch("b", "*?*?*") == false);
}
#endif
