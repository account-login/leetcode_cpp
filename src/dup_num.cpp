#include <vector>
#include <algorithm>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/find-the-duplicate-number/


using namespace std;


class Solution {
public:
    int findDuplicate(const vector<int> &nums) {
        int n_plus = nums.size();
        assert(n_plus >= 2);

        int left = 1;
        int right = n_plus - 1;
        while (right - left >= 2) {
            int mid = (left + right) / 2;

            int count = count_if(nums.begin(), nums.end(), [&](int i) {
                return i <= mid;
            });
            if (count <= mid) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        if (left == right) {
            return left;
        } else {
            assert(right > left);
            if (count(nums.begin(), nums.end(), left) > 1) {
                return left;
            } else {
                assert(count(nums.begin(), nums.end(), right) > 1);
                return right;
            }
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("287. Find the Duplicate Number") {
    Solution s;

    CHECK(s.findDuplicate({ 1, 1 }) == 1);
    CHECK(s.findDuplicate({ 1, 1, 2 }) == 1);
    CHECK(s.findDuplicate({ 1, 2, 2 }) == 2);
    CHECK(s.findDuplicate({ 1, 1, 1, 1, 1, 1 }) == 1);

    CHECK(s.findDuplicate({ 1, 2, 2, 3 }) == 2);
    CHECK(s.findDuplicate({ 1, 2, 2, 3, 4 }) == 2);
    CHECK(s.findDuplicate({ 1, 2, 2, 3, 2 }) == 2);
    CHECK(s.findDuplicate({ 1, 2, 2, 2, 4 }) == 2);

    CHECK(s.findDuplicate({ 1, 2, 3, 5, 6, 7, 8, 9, 9, 9 }) == 9);
}
#endif
