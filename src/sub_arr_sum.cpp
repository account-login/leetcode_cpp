#include <vector>
#include <unordered_set>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#   define private public
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/continuous-subarray-sum/


using namespace std;


class Solution {
public:
    bool checkSubarraySum(const vector<int> &nums, int k) {
        if (nums.size() <= 1) {
            return false;
        }
        if (k == 0) {
            return nums.size() >= 2 && nums[0] == 0 && nums[1] == 0;
        }

        unordered_set<int> prefix_sum {0, nums[0]};
        int prev = nums[0];
        for (size_t i = 1; i < nums.size(); ++i) {
            int cur = (prev + nums[i]) % k;
            if (prefix_sum.find(cur) != prefix_sum.end()) {
                return true;
            } else {
                prefix_sum.insert(cur);
                prev = cur;
            }
        }

        return false;
    }
};


#ifdef RUN_TEST
TEST_CASE("523. Continuous Subarray Sum") {
    Solution s;

    CHECK_FALSE(s.checkSubarraySum({}, 1));
    CHECK_FALSE(s.checkSubarraySum({1}, 1));
    CHECK(s.checkSubarraySum({1, 1}, 1));
    CHECK(s.checkSubarraySum({1, 1}, 2));
    CHECK_FALSE(s.checkSubarraySum({2}, 2));
    CHECK_FALSE(s.checkSubarraySum({1, 1}, 3));
    CHECK(s.checkSubarraySum({0, 0}, 0));

    CHECK(s.checkSubarraySum({23, 2, 4, 6, 7}, 6));
    CHECK(s.checkSubarraySum({23, 2, 6, 4, 7}, 6));
    CHECK(s.checkSubarraySum({23, 2, 6, 4, 7}, -6));
}
#endif
