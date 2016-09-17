#include <iostream>
#include <vector>
#include <algorithm>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/rotate-array/


using namespace std;


class Solution {
public:
    void rotate(vector<int> &nums, int k) {
        if (nums.size() == 0) {
            return;
        }

        k %= nums.size();
        if (k < 0) {
            k = nums.size() + k;
        }

        rotate_right(nums.begin(), nums.end(), k);
    }

    void rotate_right(vector<int>::iterator begin, vector<int>::iterator end, int len) {
        assert(len >= 0);
        if (len == 0) {
            return;
        }

        bool right_dir = true;
        while (true) {
            if (end - begin - len >= len) {
                swap_ranges(begin, begin + len, end - len);
                if (right_dir) {
                    begin += len;
                } else {
                    end -= len;
                }
            } else if (end - begin == len) {
                return;
            } else {
                len = end - begin - len;
                right_dir = !right_dir;
            }
        }
    }

    vector<int> rotate_vec(vector<int> nums, int k) {
        rotate(nums, k);
        return nums;
    }

    vector<int> rotate_naive(vector<int> nums, int k) {
        for (int i = 0; i < k; i++) {
            int sb = nums[nums.size() - 1];
            for (int i = nums.size() - 1; i > 0; i--) {
                nums[i] = nums[i - 1];
            }
            nums[0] = sb;
        }

        return nums;
    }
};


#ifdef RUN_TEST
TEST_CASE("189. Rotate Array") {
    Solution s;

    vector<int> arr = { 1, 2, 3, 4, 5 };
    for (int i = 0; i < arr.size() * 2; i++) {
        CAPTURE(i);
        CHECK(s.rotate_vec(arr, i) == s.rotate_naive(arr, i));
    }

    arr = {};
    CHECK(s.rotate_vec(arr, 2) == arr);
}
#endif
