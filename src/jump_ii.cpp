#include <vector>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/jump-game-ii/


using namespace std;


class Solution {
public:
    int jump(const vector<int> &nums) {
        if (nums.size() <= 1) {
            return 0;
        }

        int ans = 0;
        int start = 0;
        int end = 1;
        while (end < nums.size()) {
            int new_end = end;
            for (int i = start; i < end; i++) {
                if (nums[i] + i + 1 > new_end) {
                    new_end = nums[i] + i + 1;
                }
            }

            assert(new_end > end);
            start = end;
            end = new_end;
            ans++;
        }

        return ans;
    }
};


#ifdef RUN_TEST
TEST_CASE("45. Jump Game II") {
    Solution s;

    CHECK(s.jump({ 0 }) == 0);
    CHECK(s.jump({ 2, 3, 1, 1, 4 }) == 2);
    CHECK(s.jump({ 1, 1, 1, 1, 1, 1 }) == 5);

    vector<int> nums;
    int N = 10000;
    for (int i = 0; i < N; i++) {
        nums.push_back(10);
    }
    CHECK(s.jump(nums) == N / 10);
}
#endif
