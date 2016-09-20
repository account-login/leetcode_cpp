#include <string>
#include <random>
#include <iostream>

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
     * @brief Find out weither \a str starts with first word of \a pattern.
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

        // vector<int> next(pattern_len);
        int next[pattern_len];
        next[0] = 0;
        if (pattern_len > 1) {
            next[1] = 0;
            for (int i = 2; i < pattern_len; i++) {
                next[i] = next[i - 1] + 1;
                while (!char_match_bi(pattern[next[i] - 1], pattern[i - 1])) {
                    if (next[i] == 1) {
                        next[i] = 0;
                        break;
                    } else {
                        next[i] = next[next[i] - 1] + 1;
                    }
                }
                assert(next[i] < i);
            }
        }

        int si = 0;
        int pi = 0;
        while (true) {
            if (str[si] == '\0') {
                if (pi == pattern_len) {
                    return si;
                } else {
                    return -1;
                }
            }
            if (pi == pattern_len) {
                return si;
            }

            if (char_match(str[si], pattern[pi])) {
                si++;
                pi++;
            } else {
                if (pi == 0) {
                    si++;
                } else {
                    pi = next[pi];
                }
            }
        }
    }

    static inline bool char_match(char c, char p) {
        return p == '?' || c == p;
    }

    static inline bool char_match_bi(char p1, char p2) {
        return (p1 == '?' || p2 == '?') || p1 == p2;
    }
};


#ifdef RUN_TEST
TEST_CASE("44. Wildcard Matching") {
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
    CHECK(s.isMatch("asdfasdfxasdfasdfxasdfasdfxasdfasdfxasdfasdf", "asdfa*sd*sdfasdfasdf*asdf") == false);
    CHECK(s.isMatch("asdfasdfasdfasdfasdfasdfasdfasdf", "asdfa*sd*dasdfaXsdf*asdf") == false);

    CHECK(s.isMatch("b", "?*?") == false);
    CHECK(s.isMatch("b", "*?*?*") == false);

    random_device rd;
    uniform_int_distribution<int> random_char('a', 'c');
    uniform_int_distribution<int> pattern_len_gen(1, 15);

    for (int i = 0; i < 1000; i++) {
        int pattern_len = pattern_len_gen(rd);
        uniform_int_distribution<int> str_len_gen(pattern_len, pattern_len + 300);

        string pattern_mid = "";    // random
        for (int _ = 0; _ < pattern_len; _++) {
            pattern_mid.push_back(random_char(rd));
        }
        string pattern = "x*" + pattern_mid + "*x";

        for (int j = 0; j < 100; j++) {
            CAPTURE(i);
            CAPTURE(j);
            int str_len = str_len_gen(rd);

            string str_mid = "";    // random
            for (int _ = 0; _ < str_len; _++) {
                str_mid.push_back(random_char(rd));
            }
            string str = "x" + str_mid + "x";

            CAPTURE(str);
            CAPTURE(pattern);
            bool ans = str_mid.find(pattern_mid) != string::npos;
            CHECK(s.isMatch(str, pattern) == ans);
        }
    }
}
#endif
