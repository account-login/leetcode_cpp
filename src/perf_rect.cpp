#include <iostream>
#include <limits>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/perfect-rectangle/


using namespace std;


class Solution {
public:
    bool isRectangleCover(const vector<vector<int>> &rectangles) {
        for (size_t i = 0; i < rectangles.size() - 1; i++) {
            for (size_t j = i + 1; j < rectangles.size(); j++) {
                if (is_overlap(rectangles[i], rectangles[j])) {
                    return false;
                }
            }
        }

        int l = numeric_limits<int>::max();
        int b = numeric_limits<int>::max();
        int r = numeric_limits<int>::min();
        int t = numeric_limits<int>::min();

        int area_sum = 0;

        for (auto &rect : rectangles) {
            int ll = rect[0];
            int bb = rect[1];
            int rr = rect[2];
            int tt = rect[3];

            area_sum += (tt - bb) * (rr - ll);

            if (bb < b) {
                b = bb;
            }
            if (tt > t) {
                t = tt;
            }
            if (ll < l) {
                l = ll;
            }
            if (rr > r) {
                r = rr;
            }
        }

        return area_sum == (t - b) * (r - l);
    }

    bool is_overlap(const vector<int> &r1, const vector<int> &r2) {
        int la = r1[0];
        int ba = r1[1];
        int ra = r1[2];
        int ta = r1[3];

        int lb = r2[0];
        int bb = r2[1];
        int rb = r2[2];
        int tb = r2[3];

        int lc = max(la, lb);
        int rc = min(ra, rb);
        int tc = min(ta, tb);
        int bc = max(ba, bb);

        if (lc < rc && tc > bc) {
            return true;
        } else {
            return false;
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("Test is_overlap()") {
    Solution s;

    CHECK(s.is_overlap({ 0, 0, 1, 1 }, { 1, 0, 2, 1 }) == false);
    CHECK(s.is_overlap({ 0, 0, 3, 3 }, { -1, 1, 5, 2 }) == true);
    CHECK(s.is_overlap({ 0, 0, 5, 5 }, { 1, 1, 2, 2 }) == true);
    CHECK(s.is_overlap({ 0, 0, 5, 5 }, { -1, 1, 1, 2 }) == true);
}

TEST_CASE("391. Perfect Rectangle") {
    Solution s;

    vector<vector<int>> rects = {
        {1,1,3,3},
        {3,1,4,2},
        {3,2,4,4},
        {1,3,2,4},
        {2,3,3,4}
    };
    CHECK(s.isRectangleCover(rects) == true);

    rects = {
        {1,1,2,3},
        {1,3,2,4},
        {3,1,4,2},
        {3,2,4,4}
    };
    CHECK(s.isRectangleCover(rects) == false);

    rects = {
        {1,1,3,3},
        {3,1,4,2},
        {1,3,2,4},
        {3,2,4,4}
    };
    CHECK(s.isRectangleCover(rects) == false);

    rects = {
        {1,1,3,3},
        {3,1,4,2},
        {1,3,2,4},
        {2,2,4,4}
    };
    CHECK(s.isRectangleCover(rects) == false);
}
#endif
