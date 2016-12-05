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

        int len1 = s1.size() * n1;
        vector<int> go_from_s1(s1.size());
        size_t start = s1.find(s2[0]);
        if (start == string::npos) {
            return 0;
        }

        bool has_period = false;
        int cur = start;
        int s2_count = 0;
        while (true) {
            while (s1[cur % s1.size()] != s2[0]) {
                cur++;
            }

            if (go_from_s1[cur % s1.size()] != 0) {
                if (!has_period && cur % s1.size() == start) {
                    has_period = true;
                    int one_period = cur - start;
                    int periods = (len1 - start) / one_period;
                    s2_count *= periods;
                    cur = start + one_period * periods;
                    assert(cur <= len1);
                    continue;
                } else {
                    cur += go_from_s1[cur % s1.size()];
                }
            } else {
                int save_cur = cur;
                for (char s2_ch : s2) {
                    while (s1[cur % s1.size()] != s2_ch) {
                        cur++;
                    }
                    cur++;
                }

                go_from_s1[save_cur % s1.size()] = cur - save_cur;
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
}
#endif
