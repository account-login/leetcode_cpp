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
    bool circularArrayLoop(vector<int> &nums) {
        int len = nums.size();
        for (int i = 0; i < len; i++) {
            // skip visited nums, O(n)
            if (nums[i] != 0) {
                if (check_loop(nums, i)) {
                    return true;
                } else {
                    mark_dead(nums, i);
                }
            }
        }

        return false;
    }

    bool check_loop(vector<int> &nums, int pos) {
        bool forward = (nums[pos] > 0);
        int len = nums.size();
        for (int i = 0; i < len; i++) {
            if (nums[pos] % len == 0) {
                return false;               // jump to itself
            } else {
                int shift = nums[pos];
                if ((shift > 0) != forward) {
                    return false;           // direction changed
                }
                pos += (shift % len) + len;
                pos %= len;
            }
        }
        // not exited within `len` step, a loop.
        return true;
    }

    void mark_dead(vector<int> &nums, int pos) {
        bool forward = (nums[pos] > 0);
        int len = nums.size();
        while (true) {
            if (nums[pos] % len == 0) {
                nums[pos] = 0;      // mark
                return;             // jump to itself
            } else {
                int shift = nums[pos];
                if ((shift > 0) != forward) {
                    return;         // direction changed
                } else {
                    nums[pos] = 0;  // mark
                }
                pos += (shift % len) + len;
                pos %= len;
            }
        }
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

    vector<int> vec;
    int N = 1000000;
    for (int i = 0; i < N; i++) {
        if (i % 50 == 0) {
            vec.push_back(-49);
        } else {
            vec.push_back(1);
        }
    }

    CHECK_FALSE(s.solve(vec));

    vec.clear();
    for (int i = 0; i < N; i++) {
        vec.push_back(1);
    }
    vec.push_back(-1);

    CHECK_FALSE(s.solve(vec));
}
#endif
