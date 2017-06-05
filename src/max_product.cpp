#include <algorithm>
#include <vector>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#   define private public
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/maximum-product-subarray/


using namespace std;


class Solution {
public:
    int maxProduct(const vector<int> &nums) {
        assert(nums.size() > 0);

        int ans = nums[0];
        int ref = 1;
        int prev = 1;
        bool has_zero = false;

        for (int n : nums) {
            if (n == 0) {
                ref = 1;
                prev = 1;
                has_zero = true;
            } else {
                int cur = prev * n;
                if (cur > 0) {
                    ans = max(ans, cur);
                    if (ref > 0) {
                        ref = max(ref, cur);
                    }
                } else {
                    ans = max(ans, cur / ref);
                    if (ref > 0) {
                        ref = cur;
                    } else {
                        ref = max(ref, cur);
                    }
                }
                prev = cur;
            }
        }

        if (has_zero) {
            ans = max(0, ans);
        }
        return ans;
    }
};


#ifdef RUN_TEST
TEST_CASE("152. Maximum Product Subarray") {
    Solution s;

    CHECK(s.maxProduct({2, 3, -2, 4}) == 6);
    CHECK(s.maxProduct({6}) == 6);
    CHECK(s.maxProduct({-6}) == -6);
    CHECK(s.maxProduct({-6, 1}) == 1);
    CHECK(s.maxProduct({1, -6}) == 1);
    CHECK(s.maxProduct({0}) == 0);
    CHECK(s.maxProduct({0, 0}) == 0);
    CHECK(s.maxProduct({0, 0, 0}) == 0);
    CHECK(s.maxProduct({1, 2, 0, 2, 3}) == 6);
    CHECK(s.maxProduct({1, -2, 0, -2, 3}) == 3);
    CHECK(s.maxProduct({1, -2, 1, -2, 3}) == 12);
    CHECK(s.maxProduct({1, -2, 1, -2, 3, -10}) == 60);
    CHECK(s.maxProduct({-2, 3, 4}) == 12);
}
#endif
