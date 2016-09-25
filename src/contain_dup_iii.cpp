#include <iostream>
#include <algorithm>
#include <limits>
#include <numeric>
#include <cstdlib>
#include <vector>
#include <set>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// select an implementation
#define ALGO sort       // sort tree naive
#define __SUFFIX(a, b) a##b
#define _SUFFIX(a, b) __SUFFIX(a, b)


// https://leetcode.com/problems/contains-duplicate-iii/


using namespace std;


class Solution {
public:
    bool containsNearbyAlmostDuplicate(const vector<int> &nums, int dist_lim, int val_lim) {
        return _SUFFIX(containsNearbyAlmostDuplicate_, ALGO)(nums, dist_lim, val_lim);
    }

    bool containsNearbyAlmostDuplicate_sort(const vector<int> &nums, int dist_lim, int val_lim) {
        assert(dist_lim >= 0);
        if (val_lim < 0) {
            return false;
        }
        if (dist_lim <= 0) {
            return false;
        }

        vector<int> indexes;
        for (int i = 0; i < nums.size(); i++) {
            indexes.push_back(i);
        }

        auto comp = [&](int ia, int ib) {
            return nums[ia] < nums[ib];
        };
        sort(indexes.begin(), indexes.end(), comp);

        for (int start = 0; start < indexes.size(); start++) {
            int v0 = nums[indexes[start]];
            for (int i = start + 1;
                 i < indexes.size() && (long long)nums[indexes[i]] - v0 <= val_lim;
                 i++)
            {
                if (abs(indexes[i] - indexes[start]) <= dist_lim) {
                    return true;
                }
            }
        }

        return false;
    }

    bool containsNearbyAlmostDuplicate_tree(const vector<int> &nums, int dist_lim, int val_lim) {
        assert(dist_lim >= 0);
        if (val_lim < 0) {
            return false;
        }
        if (dist_lim <= 0) {
            return false;
        }

        multiset<int> tree;
        int end = 0;
        while (end < nums.size()) {
            end++;
            int mid = nums[end - 1];

            auto it = tree.insert(mid);
            if (it != tree.begin()) {
                it--;
                int left = *it;
                it++;
                if ((long long)mid - left <= val_lim) {
                    return true;
                }
            }

            it++;
            if (it != tree.end()) {
                int right = *it;
                if ((long long)right - mid <= val_lim) {
                    return true;
                }
            }

            // tree is full, remove oldest one
            if (tree.size() == dist_lim + 1) {
                int begin = end - (dist_lim + 1);
                assert(begin >= 0);
                auto it = tree.find(nums[begin]);
                assert(it != tree.end());
                tree.erase(it);
            }
        }

        return false;
    }

    bool containsNearbyAlmostDuplicate_naive(const vector<int> &nums, int dist_lim, int val_lim) {
        assert(dist_lim >= 0);
        if (val_lim < 0) {
            return false;
        }
        if (dist_lim <= 0) {
            return false;
        }

        for (int start = 0; start < nums.size(); start++) {
            for (int i = start + 1; i < min(start + dist_lim + 1, (int)nums.size()); i++) {
                if (llabs((long long)nums[start] - nums[i]) <= val_lim) {
                    return true;
                }
            }
        }

        return false;
    }
};


#ifdef RUN_TEST
TEST_CASE("220. Contains Duplicate III") {
    Solution s;

    CHECK(s.containsNearbyAlmostDuplicate({ 1, 3, 5, 7 }, 3, 1) == false);
    CHECK(s.containsNearbyAlmostDuplicate({ 0, 3, 0, 5, 9 }, 2, 2) == true);
    CHECK(s.containsNearbyAlmostDuplicate({ 0, 3, 10, 6, 0, 9 }, 3, 2) == true);
    CHECK(s.containsNearbyAlmostDuplicate({ 0, 3, 7, 5, 9 }, 2, 2) == true);
    CHECK(s.containsNearbyAlmostDuplicate({ 0, 3, 0, 5, 9 }, 1, 2) == false);
    CHECK(s.containsNearbyAlmostDuplicate({}, 1, 1) == false);
    CHECK(s.containsNearbyAlmostDuplicate({ 0 }, 2, 1) == false);
    CHECK(s.containsNearbyAlmostDuplicate({ 0, 2 }, 2, 1) == false);
    CHECK(s.containsNearbyAlmostDuplicate({ 0, 2 }, 1, 2) == true);

    CHECK(s.containsNearbyAlmostDuplicate({ 7, 1, 3 }, 2, 3) == true);

    CHECK(s.containsNearbyAlmostDuplicate(
        { -1, numeric_limits<int>::max() }, 1, numeric_limits<int>::max()) == false);
}
#endif
