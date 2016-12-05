#include <string>
#include <vector>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#   define private public
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/count-the-repetitions/


using namespace std;


class Solution {
public:
    int getMaxRepetitions(const string &s1, int n1, const string &s2, int n2) {
        assert(s1.size() > 0);
        assert(s2.size() > 0);
        assert(n1 >= 0);
        assert(n2 >= 1);

        size_t start = s1.find(s2[0]);
        if (start == string::npos) {
            return 0;
        }

        size_t s1_len = s1.size();
        int len1 = s1_len * n1;
        vector<int> go_from_s1(s1_len);

        vector<int> next_start(s1_len, -1);
        int dist_to_start = -1;
        for (size_t i = 0; i < s1_len; i++) {
            size_t index = s1_len + start - i;
            index %= s1_len;
            if (s1[index] == s2[0]) {
                dist_to_start = 0;
            }
            next_start[index] = dist_to_start;
            dist_to_start++;
        }
#ifdef RUN_TEST
        for (size_t i = 0; i < next_start.size(); i++) {
            size_t index = i + next_start[i];
            assert(s1[index % s1_len] != -1);
            assert(s1[index % s1_len] == s2[0]);
        }
#endif

        bool has_period = false;
        int cur = start;
        int s2_count = 0;
        while (true) {
            cur += next_start[cur % s1_len];

            if (go_from_s1[cur % s1_len] != 0) {
                if (!has_period && cur % s1_len == start) {
                    has_period = true;
                    int one_period = cur - start;
                    int periods = (len1 - start) / one_period;
                    s2_count *= periods;
                    cur = start + one_period * periods;
                    assert(cur <= len1);
                    continue;
                } else {
                    cur += go_from_s1[cur % s1_len];
                }
            } else {
                int save_cur = cur;
                for (char s2_ch : s2) {
                    while (s1[cur % s1_len] != s2_ch) {
                        cur++;
                    }
                    cur++;
                }

                go_from_s1[save_cur % s1_len] = cur - save_cur;
            }

            if (cur > len1) {
                break;
            } else {
                s2_count++;
            }
        }

        return s2_count / n2;
    }
};


#ifdef RUN_TEST
TEST_CASE("466. Count The Repetitions") {
    Solution s;

    CHECK(s.getMaxRepetitions("caca", 0, "a", 1) == 0);
    CHECK(s.getMaxRepetitions("caca", 100, "b", 17) == 0);
    CHECK(s.getMaxRepetitions("caca", 1, "cacaca", 1) == 0);

    CHECK(s.getMaxRepetitions("acb", 4, "ab", 2) == 2);
    CHECK(s.getMaxRepetitions("acb", 8, "ab", 2) == 4);
    CHECK(s.getMaxRepetitions("acb", 101, "ab", 2) == 50);
    CHECK(s.getMaxRepetitions("acb", 101, "ab", 4) == 25);
    CHECK(s.getMaxRepetitions("acb", 100, "ab", 5) == 20);
    CHECK(s.getMaxRepetitions("acac", 4000, "aaaa", 100) == 20);
    CHECK(s.getMaxRepetitions("caca", 4000, "aaaa", 100) == 20);
    CHECK(s.getMaxRepetitions("caca", 4001, "aaaa", 100) == 20);
    CHECK(s.getMaxRepetitions("cacacacacacacaca", 100, "ca", 7) == 8 * 100 / 7);

    CHECK(s.getMaxRepetitions(
          "phqghumeaylnlfdxfircvscxggbwkfnqduxwfnfozvsrtkjpre"
          "pggxrpnrvystmwcysyycqpevikeffmznimkkasvwsrenzkycxf",
          1000000,
          "xtlsgypsfadpooefxzbcoejuvpvaboygpoeylfpbnpljvrvipy"
          "amyehwqnqrqpmxujjloovaowuxwhmsncbxcoksfzkvatxdknly",
          100
    ) == 303);
}
#endif
