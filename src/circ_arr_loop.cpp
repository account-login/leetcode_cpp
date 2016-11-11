#include <vector>
#include <algorithm>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/circular-array-loop/


using namespace std;


class Solution {
public:
    bool circularArrayLoop(const vector<int> &nums) {
        int len = nums.size();
        for (int i = 0; i < len; i++) {
            if (check_loop(nums, i)) {
                return true;
            }
        }

        return false;
    }

    bool check_loop(const vector<int> &nums, int pos) {
        bool forward = (nums[pos] > 0);
        int len = nums.size();
        for (int i = 0; i < len; i++) {
            if (nums[pos] % len == 0) {
                return false;   // jump to itself
            } else {
                int shift = nums[pos];
                if ((shift > 0) != forward) {
                    return false;   // direction changed
                }
                pos += (shift % len) + len;
                pos %= len;
            }
        }
        // not exited within `len` step, a loop.

        return true;
    }

    bool solve(vector<int> nums) {
        return circularArrayLoop(nums);
    }
};


#ifdef RUN_TEST
TEST_CASE("457. Circular Array Loop") {
    Solution s;

    CHECK(s.solve({2, -1, 1, 2, 2}));
    CHECK(s.solve({ -1, 5, 103, -8 }));
    CHECK_FALSE(s.solve({-2, 1, -1, -2, -2}));
    CHECK_FALSE(s.solve({-1, 2}));
    CHECK_FALSE(s.solve({1, -2}));
    CHECK_FALSE(s.solve({2, -1, 1, -2, -2}));

    CHECK_FALSE(s.solve({}));
    CHECK_FALSE(s.solve({123}));
    CHECK_FALSE(s.solve({5, -3}));
    CHECK_FALSE(s.solve({5, -27}));
}
#endif
