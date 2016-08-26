#include <iostream>
#include <utility>
#include <tuple>
#include <climits>
#include <algorithm>
#include <random>
#include <cassert>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#endif

// https://leetcode.com/problems/median-of-two-sorted-arrays/


using namespace std;


class Solution {
public:
    double findMedianSortedArrays(const vector<int>& nums1, const vector<int>& nums2) {
        if (nums1.size() == 0) {
            return median(nums2);
        } else if (nums2.size() == 0) {
            return median(nums1);
        }

        size_t size_sum = (nums1.size() + nums2.size()) / 2;
        size_t r2 = min(nums1.size(), size_sum);
        size_t r1 = size_sum - min(nums2.size(), size_sum);

        bisect(nums1, nums2, r1, r2);

        for (size_t rm = r1; rm <= r2; rm++) {
            size_t sm = size_sum - rm;
            if (rm == 0) {
                if (nums1.at(rm) < nums2.at(sm - 1)) {
                    continue;
                }
            } else if (sm == 0) {
                if (nums2.at(sm) < nums1.at(rm - 1)) {
                    continue;
                }
            } else if (rm == nums1.size()) {
                if (nums2.at(sm) < nums1.at(rm - 1)) {
                    continue;
                }
            } else if (sm == nums2.size()) {
                if (nums1.at(rm) < nums2.at(sm - 1)) {
                    continue;
                }
            } else {
                if (nums1.at(rm) < nums2.at(sm - 1)) {
                    continue;
                }
            }

            // valid
            int a1, a2, b1, b2;

            if (rm == 0) {
                a1 = INT_MIN;
            } else {
                a1 = nums1.at(rm - 1);
            }

            if (rm == nums1.size()) {
                a2 = INT_MAX;
            } else {
                a2 = nums1.at(rm);
            }

            if (sm == 0) {
                b1 = INT_MIN;
            } else {
                b1 = nums2.at(sm - 1);
            }

            if (sm == nums2.size()) {
                b2 = INT_MAX;
            } else {
                b2 = nums2.at(sm);
            }

            if ((nums1.size() + nums2.size()) % 2 == 1) {
                // odd
                return min(a2, b2);
            } else {
                // even
                return (max(a1, b1) + min(a2, b2)) / 2.0;
            }
        }

        assert(false);
    }

    double median(const vector<int> &nums) {
        assert(nums.size() > 0);

        if (nums.size() == 1) {
            return nums[0];
        }

        int half = nums.size() / 2;
        if (nums.size() % 2 == 0) {
            return (nums.at(half - 1) + nums.at(half)) / 2.0;
        } else {
            return nums.at(half);
        }
    }

    void bisect(const vector<int> &a1, const vector<int> &a2, size_t &r1, size_t &r2) {
        assert(r2 > r1);
        const int size_sum = (a1.size() + a2.size()) / 2;

        while (true) {
            if (r2 - r1 <= 1) {
                return;
            }

            assert(size_sum >= 2);

            size_t rm = (r1 + r2) / 2;
            size_t sm = size_sum - rm;
            assert(rm != 0);
            assert(sm != 0);

            if (a1.at(rm - 1) > a2.at(sm)) {
                r2 = rm;
            } else {
                r1 = rm;
            }
        }
    }

    double solve_naive(const vector<int>& nums1, const vector<int>& nums2) {
        vector<int> cat(nums1);
        for (auto i : nums2) {
            cat.push_back(i);
        }

        sort(cat.begin(), cat.end());
        return median(cat);
    }
};


#ifdef RUN_TEST
TEST_CASE("4. Median of Two Sorted Arrays") {
    Solution s;

    CHECK(s.findMedianSortedArrays({ 0 }, { 1 }) == Approx(0.5));
    CHECK(s.findMedianSortedArrays({ 0 }, { 1, 2 }) == Approx(1.0));
    CHECK(s.findMedianSortedArrays({ 1, 5 }, { 2 }) == Approx(2.0));
    CHECK(s.findMedianSortedArrays({}, { 0, 1 }) == Approx(0.5));
    CHECK(s.findMedianSortedArrays({}, { 0 }) == 0);
    CHECK(s.findMedianSortedArrays({ 1, 2, 3}, {}) == Approx(2.0));

    CHECK(s.findMedianSortedArrays({ 1, 3, 5, 7 }, { 2, 4, 6, 8 }) == Approx(4.5));
    CHECK(s.findMedianSortedArrays({ 1, 3, 5 }, { 10, 12, 14, 18 }) == Approx(10.0));
    CHECK(s.findMedianSortedArrays({ 1, 3, 5 }, { 4, 6, 10 }) == Approx(4.5));
    CHECK(s.findMedianSortedArrays({ 1, 3, 8 }, { 4, 5, 6, 7, 8 }) == Approx(5.5));
    CHECK(s.findMedianSortedArrays({ 6, 7, 8, 9 }, { 1, 2, 3 }) == Approx(6.0));

    CHECK(s.findMedianSortedArrays({ 1, 1, 1, 1 }, { 1, 1, 1 }) == Approx(1.0));

    auto print_vector = [](vector<int> &v) {
        cout << "{ ";
        string sep = "";
        for (auto i : v) {
            cout << sep;
            cout << i;
            sep = ", ";
        }
        cout << " }" << endl;
    };

    auto random_case = [](int max_len = 100, int max_num = 500) -> tuple<vector<int>, vector<int>> {
        random_device rd;
        uniform_int_distribution<int> vector_size(1, max_len);
        uniform_int_distribution<int> content(0, max_num);

        vector<int> v1;
        int len = vector_size(rd);
        for (int i = 0; i < len; i++) {
            v1.push_back(content(rd));
        }
        sort(v1.begin(), v1.end());

        vector<int> v2;
        len = vector_size(rd);
        for (int i = 0; i < len; i++) {
            v2.push_back(content(rd));
        }
        sort(v2.begin(), v2.end());

        return make_tuple(v1, v2);
    };

    int case_count = 0;
    for (auto tup : {
         make_tuple(4, 10, 1024),
         make_tuple(100, 40, 1024 * 10),
         make_tuple(100, 500, 1024 * 10)
    }) {
        int max_len, max_num, ncase;
        tie(max_len, max_num, ncase) = tup;

        for (int i = 0; i < ncase; i++) {
            vector<int> v1, v2;
            tie(v1, v2) = random_case(max_len, max_num);
            INFO("Test random case " << case_count++
                 << ", max_len = " << max_len << ", max_num = " << max_num);
            print_vector(v1);
            print_vector(v2);
            CHECK(s.findMedianSortedArrays(v1, v2) == Approx(s.solve_naive(v1, v2)));
        }
    }
}
#endif
