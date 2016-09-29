#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <tuple>
#include <vector>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/3sum/


using namespace std;


using Triplet = tuple<int, int, int>;

namespace std {
    template<>
    struct hash<Triplet> {
        size_t operator ()(const Triplet &t) const {
            return get<0>(t) ^ (get<1>(t) << 5) ^ (get<2>(t) << 11);
        }
    };
}


class Solution {
public:
    vector<vector<int>> threeSum(vector<int> nums) {
        if (nums.size() < 3) {
            return {};
        }

        sort(nums.begin(), nums.end());

        vector<vector<int>> ans;

        // O(n^2)
        int begin = 0;
        int end = nums.size();
        while (begin <= end - 3) {
            // maximum end optimization
            if (nums[begin] + nums[begin + 1] + nums[end - 1] > 0) {
                end--;
                continue;
            }

            // minimum begin optimiaztion
            if (nums[begin] + nums[end - 2] + nums[end - 1] < 0) {
                begin++;
                continue;
            }

            int j = begin + 1;
            int k = end - 1;
            while (j < k) {
                int sum = nums[begin] + nums[j] + nums[k];
                if (sum == 0) {
                    ans.push_back({ nums[begin], nums[j], nums[k] });

                    j++;
                    // ensure no duplicates
                    while (nums[j] == nums[j - 1] && j < k) {
                        j++;
                    }
                    k--;
                    // this is not necessary
                    // while (nums[k] == nums[k + 1] && j < k) {
                    //     k--;
                    // }
                } else if (sum < 0) {
                    j++;
                } else {    // sum > 0
                    k--;
                }
            }

            begin++;
            // ensure no duplicates
            while (nums[begin] == nums[begin - 1] && begin <= end - 3) {
                begin++;
            }
        }

        return ans;
    }

    unordered_set<Triplet> threeSum_set(const vector<int> &nums) {
        vector<vector<int>> ans_vec = threeSum(nums);

        unordered_set<Triplet> ans_set;
        for (const auto &v : ans_vec) {
            ans_set.insert(Triplet { v[0], v[1], v[2] });
        }
        assert(ans_set.size() == ans_vec.size());

        return ans_set;
    }
};


namespace std {
    ostream &operator << (ostream &os, const Triplet &t) {
        os << "Triplet { " << get<0>(t) << ", " << get<1>(t) << ", " << get<2>(t) << " }";
        return os;
    }

    ostream &operator << (ostream &os, const unordered_set<Triplet> &s) {
        vector<Triplet> vec(s.begin(), s.end());
        sort(vec.begin(), vec.end());

        os << "{ ";

        string sep = "";
        for (const auto &e : vec) {
            os << sep;
            os << e;
            sep = ", ";
        }

        os << " }";

        return os;
    }
}


#ifdef RUN_TEST
TEST_CASE("15. 3Sum") {
    Solution s;

    unordered_set<Triplet> ans = {
        Triplet {-1,  0, 1},
        Triplet {-1, -1, 2}
    };
    CHECK(s.threeSum_set({ -1, 0, 1, 2, -1, -4 }) == ans);

    ans = {
        Triplet { -1, 0, 1 }
    };
    CHECK(s.threeSum_set({ -1, 0, 1 }) == ans);

    // no solutions
    ans = {};
    CHECK(s.threeSum_set({ 0 }) == ans);

    CHECK(s.threeSum_set({ 0, 0, 1}) == ans);

    // no duplicates
    ans = {
        Triplet { 0, 0, 0 }
    };
    CHECK(s.threeSum_set({ 0, 0, 0 }) == ans);
    CHECK(s.threeSum_set({ 0, 0, 0, 0 }) == ans);
    CHECK(s.threeSum_set({ 0, 0, 0, 0, 0 }) == ans);

    // no duplicates
    ans = {
        Triplet { -1, 0, 1 }
    };
    CHECK(s.threeSum_set({ -8, -8, -1, -1, -1, 0, 0, 1, 1, 1, 5, 5, 10 }) == ans);

    // maximum end optimization
    vector<int> lst = {};
    lst.reserve(50000);
    for (int i = -10000; i < -100; i++) {
        lst.push_back(i);
    }
    for (int i : { -1, 0, 1 }) {
        lst.push_back(i);
    }
    for (int i = 20000; i < 50000; i++) {
        lst.push_back(i);
    }
    CHECK(s.threeSum_set(lst) == ans);

    // minimum begin optimiaztion
    lst.clear();
    for (int i = -50000; i < -20000; i++) {
        lst.push_back(i);
    }
    for (int i : { -1, 0, 1 }) {
        lst.push_back(i);
    }
    for (int i = 100; i < 10000; i++) {
        lst.push_back(i);
    }
    CHECK(s.threeSum_set(lst) == ans);

    // maximum end optimization & minimum begin optimiaztion combined
    // reduce 1/2 run time
    ans = {};
    lst.clear();
    for (int i = -50000; i < -30000; i += 2) {
        lst.push_back(i);
    }
    for (int i = 50001; i > 30001; i -= 2) {
        lst.push_back(i);
    }
    CHECK(s.threeSum_set(lst) == ans);
}
#endif
