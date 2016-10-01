#include <algorithm>
#include <limits>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "tree_node.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/first-missing-positive/


using namespace std;


class Solution {
public:
    int firstMissingPositive(vector<int> &nums) {
        // no one
        if (find(nums.begin(), nums.end(), 1) == nums.end()) {
            return 1;
        }

        int size = nums.size();
        for (int i = 0; i < size; i++) {
            int position = nums[i] - 1;
            while (0 <= position && position < size
                   && nums[i] != nums[position])
            {
                swap(nums[i], nums[position]);
                position = nums[i] - 1;
            }
        }

        for (int i = 0; i < size - 1; i++) {
            if (nums[i] + 1 != nums[i + 1]) {
                return nums[i] + 1;
            }
        }
        return nums[size - 1] + 1;
    }

    int first_missing(vector<int> nums) {
        return firstMissingPositive(nums);
    }
};


#ifdef RUN_TEST
TEST_CASE("41. First Missing Positive") {
    Solution s;
    CHECK(s.first_missing({ 1, 2, 0 }) == 3);
    CHECK(s.first_missing({ 3, 4, -1, 1 }) == 2);

    CHECK(s.first_missing({ 0 }) == 1);
    CHECK(s.first_missing({}) == 1);

    CHECK(s.first_missing({ 6, 3, 5, 4 }) == 1);
    CHECK(s.first_missing({ 4, 2, 1, 3 }) == 5);
    CHECK(s.first_missing({ 4, 2, 1, 5 }) == 3);

    CHECK(s.first_missing({ 1, 1 }) == 2);
    CHECK(s.first_missing({ 0, -1, 1, 2, 3, 3, 5 }) == 4);
}
#endif
