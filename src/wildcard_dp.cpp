#include <string>
#include <random>
#include <algorithm>
#include <iostream>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#   include "fnmatch.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/wildcard-matching/


using namespace std;


class Solution {
public:
    bool isMatch(const string &str, const string &pattern) {
        int s_len = str.size();
        // vector<char> is faster than vector<bool>
        /// \todo use only one array instead of two
        vector<char> dp1(s_len + 1, false);
        vector<char> dp2(s_len + 1, false);
        dp2[0] = true;

        int pi = 0;
        while (true) {
            if (pattern[pi] == '\0') {
                if (dp2[s_len]) {
                    return true;
                } else {
                    return false;
                }
            }

            bool no_sol = true;
            char ch = pattern[pi];
            if (ch == '*') {
                for (int i = 0; i < s_len + 1; i++) {
                    if (dp2[i]) {
                        no_sol = false;
                        for (int j = i; j < s_len + 1; j++) {
                            dp1[j] = true;
                        }
                        break;
                    } else {
                        dp1[i] = false;
                    }
                }
            } else {
                dp1[0] = false;
                for (int i = 0; i < s_len; i++) {
                    if (dp2[i] && (str[i] == ch || ch == '?')) {
                        no_sol = false;
                        dp1[i + 1] = true;
                    } else {
                        dp1[i + 1] = false;
                    }
                }
            }

            if (no_sol) {
                return false;
            }

            pi++;
            swap(dp1, dp2);
        }
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

    CHECK(s.isMatch("XasdfX", "X*????*X") == true);
    CHECK(s.isMatch("XasdfasdfasdfasdfX", "X*asdfa???asd?????*X") == true);
    CHECK(s.isMatch("XasdfX", "X*a????*X") == false);
    CHECK(s.isMatch("XasdfX", "X*???f*X") == true);
    CHECK(s.isMatch("XasdfX", "X*???f?*X") == false);

    CHECK(s.isMatch("xcacabbbcabaacbacbacbccbcbbbccbabbbabbaaacccbcacbbbcacccaacbccabbaacbbcabbbcccaccacbababbccccbbabaccccbcacacabcccbacaacabcbbaaacbacacaaacacbaaacaababbbbcbaccabbcaccacbacacaabccacx",
                    "x*cbc?bbcc*x") == true);
    CHECK(s.isMatch("xabaaaababbbcbcbbccabbbacabaaabaccabbcbbccccaaabccbbacccbaccacaaaccaccccaabcbbbbccabbacabbbbbbabacbccccccabacccacbababccaccaababbaacabbabababacabcaccacabcccbaacbabbcacccabbaccacaacbaaabcbccbcabacbccbcabcbacbx",
                    "x*cbc?bcb??*x") == true);

    random_device rd;
    char random_chars[] = { 'a', 'b', 'c', '?' };
    uniform_int_distribution<int> random_int(0, 255);
    uniform_int_distribution<int> pattern_len_gen(1, 15);

    for (int i = 0; i < 1000; i++) {
        int pattern_len = pattern_len_gen(rd);
        uniform_int_distribution<int> str_len_gen(pattern_len, pattern_len + 300);

        string pattern_mid = "";    // random
        for (int _ = 0; _ < pattern_len; _++) {
            char ch = random_chars[random_int(rd) % 4];
            pattern_mid.push_back(ch);
        }
        string pattern = "x*" + pattern_mid + "*x";

        for (int j = 0; j < 100; j++) {
            CAPTURE(i);
            CAPTURE(j);
            int str_len = str_len_gen(rd);

            string str_mid = "";    // random
            for (int _ = 0; _ < str_len; _++) {
                char ch = random_chars[random_int(rd) % 3];
                str_mid.push_back(ch);
            }
            string str = "x" + str_mid + "x";

            CAPTURE(str);
            CAPTURE(pattern);
            bool ans = fnmatch(pattern.data(), str.data(), FNM_NOESCAPE) == 0;
            CHECK(s.isMatch(str, pattern) == ans);
        }
    }
}
#endif
