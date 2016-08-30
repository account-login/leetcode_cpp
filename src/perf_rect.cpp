#include <iostream>
#include <limits>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
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
        vector<vector<int>> tested = { rectangles[0] };
        for (size_t i = 1; i < rectangles.size(); i++) {
            bool merged = false;
            auto &rect = rectangles[i];
            for (auto &rect_tested : tested) {
                if (is_overlap(rect_tested, rect)) {
                    return false;
                } else {
                    merged = try_merge(rect_tested, rect);
                }
            }
            if (!merged) {
                tested.push_back(rect);
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

    ///
    /// \brief is r1 and r2 overlaps
    /// \param r1
    /// \param r2
    /// \return
    ///
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

    ///
    /// \brief merge r2 into r1 if posible
    /// \param r1
    /// \param r2
    /// \return merged?
    ///
    bool try_merge(vector<int> &r1, const vector<int> &r2) {
        assert(!is_overlap(r1, r2));

        int &la = r1[0];
        int &ba = r1[1];
        int &ra = r1[2];
        int &ta = r1[3];

        int lb = r2[0];
        int bb = r2[1];
        int rb = r2[2];
        int tb = r2[3];

        if (la == lb && ra == rb) {
            if (ta == bb) {
                ta = tb;
                return true;
            }
            if (ba == tb) {
                ba = bb;
                return true;
            }
        }

        if (ta == tb && ba == bb) {
            if (ra == lb) {
                ra = rb;
                return true;
            }
            if (la == rb) {
                la = lb;
                return true;
            }
        }

        return false;
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

TEST_CASE("Test try_merge()") {
    Solution s;

    vector<int> rect = { 0, 0, 1, 1 };
    CHECK(s.try_merge(rect, { 0, 1, 1, 2 }) == true);
    vector<int> ans = { 0, 0, 1, 2 };
    CHECK(rect == ans);

    rect = { 0, 0, 5, 5 };
    ans = rect;
    CHECK(s.try_merge(rect, { 0, 5, 4, 8 }) == false);
    CHECK(rect == ans);
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

    rects = {};
    for (int i = 0; i <= 9999; i++) {
        rects.push_back({ 0, i - 1, 1, i });
    }
    CHECK(s.isRectangleCover(rects) == true);
}
#endif
