#include <iostream>
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <tuple>
#include <vector>
#include <functional>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/4sum/


using namespace std;


using Quadruplet = tuple<int, int, int, int>;

namespace std {
    template<>
    struct hash<Quadruplet> {
        size_t operator ()(const Quadruplet &t) const {
            return get<0>(t) ^ (get<1>(t) << 5) ^ (get<2>(t) << 11) ^ (get<3>(t) << 17);
        }
    };
}


class Solution {
public:
    void twoSum_sorted(
        vector<int>::const_iterator begin,
        vector<int>::const_iterator end,
        int target,
        function<void (vector<int> &&)> found)
    {
        if (end - begin < 2) {
            return;
        }

        while (begin < end - 1) {
            int sum = begin[0] + end[-1];
            if (sum == target) {
                vector<int> v = { end[-1], begin[0] };
                found(move(v));

                begin++;
                // ensure no duplicates
                while (begin[0] == begin[-1] && begin < end - 1) {
                    begin++;
                }
                end--;
            } else if (sum > target) {
                end--;
            } else {    // sum < target
                begin++;
            }
        }
    }

    void nSum_sorted(
        vector<int>::const_iterator begin,
        vector<int>::const_iterator end,
        int n,
        int target,
        function<void (vector<int> &&)> found)
    {
        assert(n >= 2);
        if (end - begin < n) {
            return;
        }

        if (n == 2) {
            twoSum_sorted(begin, end, target, found);
        } else {
            while (begin <= end - n) {
                // maximum end optimization
                if (accumulate(begin, begin + n - 1, 0) + end[-1] > target) {
                    end--;
                    continue;
                }

                // minimum begin optimiaztion
                if (begin[0] + accumulate(end - n + 1, end, 0) < target) {
                    begin++;
                    continue;
                }

                int num = begin[0];
                auto sub_found = [&](vector<int> &&v) {
                    v.push_back(num);
                    found(move(v));
                };

                nSum_sorted(begin + 1, end, n - 1, target - num, sub_found);

                begin++;
                // ensure no duplicates
                while (begin[0] == begin[-1] && begin <= end - n) {
                    begin++;
                }
            }
        }
    }

    vector<vector<int>> nSum(vector<int> &nums, int n, int target) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> ans;

        auto found = [&ans](vector<int> &&v) {
            ans.push_back(move(v));
        };

        nSum_sorted(nums.begin(), nums.end(), n, target, found);
        return ans;
    }

    vector<vector<int>> fourSum(vector<int> &nums, int target) {
        return nSum(nums, 4, target);
    }

    unordered_set<Quadruplet> fourSum_set(vector<int> nums, int target) {
        vector<vector<int>> ans_vec = fourSum(nums, target);

        unordered_set<Quadruplet> ans_set;
        for (auto &v : ans_vec) {
            sort(v.begin(), v.end());
            ans_set.insert(Quadruplet { v[0], v[1], v[2], v[3] });
        }
        assert(ans_set.size() == ans_vec.size());

        return ans_set;
    }
};


namespace std {
    ostream &operator << (ostream &os, const Quadruplet &t) {
        os << "Quadruplet { "
           << get<0>(t) << ", " << get<1>(t) << ", "
           << get<2>(t) << ", " << get<3>(t) << " }";
        return os;
    }

    ostream &operator << (ostream &os, const unordered_set<Quadruplet> &s) {
        vector<Quadruplet> vec(s.begin(), s.end());
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
TEST_CASE("18. 4Sum") {
    Solution s;

    unordered_set<Quadruplet> ans = {
        Quadruplet {-1,  0, 0, 1},
        Quadruplet {-2, -1, 1, 2},
        Quadruplet {-2,  0, 0, 2}
    };
    CHECK(s.fourSum_set({ 1, 0, -1, 0, -2, 2 }, 0) == ans);

    ans = {
        Quadruplet { -1, 0, 0, 1 }
    };
    CHECK(s.fourSum_set({ -1, 0, 1, 0 }, 0) == ans);

    // no solutions
    ans = {};
    CHECK(s.fourSum_set({ 0 }, 0) == ans);

    CHECK(s.fourSum_set({ 0, 0, 0, 1}, 0) == ans);

    // no duplicates
    ans = {
        Quadruplet { 0, 0, 0, 0 }
    };
    CHECK(s.fourSum_set({ 0, 0, 0, 0 }, 0) == ans);
    CHECK(s.fourSum_set({ 0, 0, 0, 0, 0 }, 0) == ans);
    CHECK(s.fourSum_set({ 0, 0, 0, 0, 0, 0 }, 0) == ans);

    // no duplicates
    ans = {
        Quadruplet { -1, -1, 0, 2 }
    };
    CHECK(s.fourSum_set({ -8, -8, -1, -1, -1, 0, 0, 2, 2, 2, 11, 15, 15 }, 0) == ans);

    // maximum end optimization
    ans = {
        Quadruplet { -1, -1, 0, 2 }
    };
    vector<int> nums = {};
    nums.reserve(50000);
    for (int i = -10000; i < -100; i++) {
        nums.push_back(i);
    }
    for (int i : { -1, -1, 0, 2 }) {
        nums.push_back(i);
    }
    for (int i = 30000; i < 60000; i++) {
        nums.push_back(i);
    }
    CHECK(s.fourSum_set(nums, 0) == ans);

    // minimum begin optimiaztion
    nums.clear();
    for (int i = -60000; i < -30000; i++) {
        nums.push_back(i);
    }
    for (int i : { -1, -1, 0, 2 }) {
        nums.push_back(i);
    }
    for (int i = 100; i < 10000; i++) {
        nums.push_back(i);
    }
    CHECK(s.fourSum_set(nums, 0) == ans);

    // maximum end optimization & minimum begin optimiaztion combined
    // reduce 1/2 run time
    ans = {};
    nums.clear();
    for (int i = -50000; i < -30000; i += 2) {
        nums.push_back(i);
    }
    for (int i = 170001; i > 150001; i -= 2) {
        nums.push_back(i);
    }
    CHECK(s.fourSum_set(nums, 0) == ans);
}
#endif
