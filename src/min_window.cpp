#include <iostream>
#include <cstdint>
#include <limits>
#include <vector>
#include <algorithm>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/minimum-window-substring/


using namespace std;


class Solution {
public:
    string minWindow(const string &s, const string &t) {
        int t_len = 0;
        int t_count[128] = {};
        for (uint8_t ch : t) {
            if (t_count[ch] == 0) {
                t_len++;
            }
            t_count[ch]++;
        }
        if (t_len == 0) {
            return "";
        }

        int s_count[128] = {};
        int s_flag_count = 0;
        int s_len = s.size();

        int win_len = numeric_limits<int>::max();
        int win_start = 0;

        int start = 0;
        int end = 0;
        while (true) {
            if (s_flag_count < t_len) {
                end++;
                if (end > s_len) {
                    break;
                }
                uint8_t ch = s[end - 1];
                if (t_count[ch]) {
                    s_count[ch]++;
                    if (s_count[ch] == t_count[ch]) {
                        s_flag_count++;
                    }
                }
            } else {
                assert(s_flag_count == t_len);
                if (end - start < win_len) {
                    win_len = end - start;
                    win_start = start;
                    if (win_len == t_len) {
                        break;
                    }
                }

                start++;
                uint8_t ch = s[start - 1];
                if (t_count[ch]) {
                    s_count[ch]--;
                    if (s_count[ch] == t_count[ch] - 1) {
                        s_flag_count--;
                    }
                }
            }
        }

        if (win_len == numeric_limits<int>::max()) {
            return "";
        } else {
            return s.substr(win_start, win_len);
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("76. Minimum Window Substring") {
    Solution s;

    CHECK(s.minWindow("ADOBECODEBANC", "ABC") == "BANC");
    CHECK(s.minWindow("assssdssf", "s") == "s");

    // non-unique `t`
    CHECK(s.minWindow("aa", "aa") == "aa");
    CHECK(s.minWindow("abcaba", "aa") == "aba");

    CHECK(s.minWindow("asdf", "") == "");
    CHECK(s.minWindow("ADOBECODEBANC", "ABCX") == "");

    CHECK(s.minWindow(string(10000, 'A'), "A") == "A");
    CHECK(s.minWindow(string(10000, 'A') + string(10000, 'B'), "AB") == "AB");
    string str = string("A") + string(10000, 'C') + string("B");
    CHECK(s.minWindow(str, "BA") == str);
}
#endif
