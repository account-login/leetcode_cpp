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
        int product = 1;

        for (int n : nums) {
            if (n == 0) {
                ref = 1;
                product = 1;
                ans = max(ans, 0);
            } else {
                product *= n;
                if (product > 0) {
                    ans = max(ans, product);
                    if (ref > 0) {
                        ref = max(ref, product);
                    }
                } else {
                    ans = max(ans, product / ref);
                    if (ref > 0) {
                        ref = product;
                    } else {
                        ref = max(ref, product);
                    }
                }
            }
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
    CHECK(s.maxProduct({0, -1, 0}) == 0);
    CHECK(s.maxProduct({1, 2, 0, 2, 3}) == 6);
    CHECK(s.maxProduct({1, -2, 0, -2, 3}) == 3);
    CHECK(s.maxProduct({1, -2, 1, -2, 3}) == 12);
    CHECK(s.maxProduct({1, -2, 1, -2, 3, -10}) == 60);
    CHECK(s.maxProduct({-2, 3, 4}) == 12);
}
#endif
