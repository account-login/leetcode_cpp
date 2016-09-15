#include <iostream>
#include <cstring>
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
        bool t_set[256];
        memset(t_set, 0, sizeof(t_set));
        for (uint8_t ch : t) {
            t_set[ch] = true;
        }
        int t_len = t.size();
        if (t_len == 0) {
            return "";
        }

        int s_count[256];
        memset(s_count, 0, sizeof(s_count));
        vector<bool> s_flag(256, false);
        int s_len = s.size();

        int win_len = numeric_limits<int>::max();
        int win_start = 0;

        int start = 0;
        int end = 0;
        while (true) {
            if (count(s_flag.begin(), s_flag.end(), true) < t_len) {
                end++;
                if (end > s_len) {
                    break;
                }
                uint8_t ch = s[end - 1];
                if (t_set[ch]) {
                    if (s_count[ch] == 0) {
                        s_flag[ch] = true;
                    }
                    s_count[ch]++;
                }
            } else {
                assert(count(s_flag.begin(), s_flag.end(), true) == t_len);
                if (end - start < win_len) {
                    win_len = end - start;
                    win_start = start;
                    if (win_len == t_len) {
                        break;
                    }
                }

                start++;
                uint8_t ch = s[start - 1];
                if (t_set[ch]) {
                    s_count[ch]--;
                    if (s_count[ch] == 0) {
                        s_flag[ch] = false;
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
    CHECK(s.minWindow("asdf", "") == "");
    CHECK(s.minWindow("assssdssf", "s") == "s");
    CHECK(s.minWindow(string(10000, 'A'), "A") == "A");
    CHECK(s.minWindow(string(10000, 'A') + string(10000, 'B'), "AB") == "AB");
    string str = string("A") + string(10000, 'C') + string("B");
    CHECK(s.minWindow(str, "BA") == str);
    CHECK(s.minWindow("ADOBECODEBANC", "ABCX") == "");
}
#endif
