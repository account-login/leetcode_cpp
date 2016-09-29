#include <string>
#include <random>
#include <algorithm>
#include <iostream>

#ifdef RUN_TEST
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
        vector<char> prefix_match(s_len + 1, false);
        prefix_match[0] = true;

        for (int pi = 0, start = 0, end = s_len + 1; ; pi++) {
            if (pattern[pi] == '\0') {
                if (prefix_match[s_len]) {
                    return true;
                } else {
                    return false;
                }
            }

            bool no_sol = true;
            char ch = pattern[pi];
            if (ch == '*') {
                for (int i = start; i < end; i++) {
                    if (prefix_match[i]) {
                        no_sol = false;
                        end = s_len + 1;
                        for (int j = i + 1; j < s_len + 1; j++) {
                            prefix_match[j] = true;
                        }
                        break;
                    }
                }
            } else {
                int new_end;
                bool ahead = prefix_match[start];
                for (int i = start; i < min(end, s_len); i++) {
                    bool cur = ahead;
                    ahead = prefix_match[i + 1];

                    if (cur && (str[i] == ch || ch == '?')) {
                        if (no_sol) {
                            start = i + 1;
                        }
                        no_sol = false;
                        prefix_match[i + 1] = true;
                        new_end = i + 2;
                        assert(new_end <= s_len + 1);
                    } else {
                        prefix_match[i + 1] = false;
                    }
                }
                end = new_end;
            }

            if (no_sol) {
                return false;
            }
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
