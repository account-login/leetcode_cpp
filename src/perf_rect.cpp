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
};


#ifdef RUN_TEST
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
