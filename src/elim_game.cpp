#include <limits>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/elimination-game/


using namespace std;


class Solution {
public:
    static int lastRemaining(int n) {
        assert(n >= 1);
        int start = 1;
        int end = n;
        int step = 2;
        bool left = true;

        while (start < end) {
            if (left) {
                if ((end - start) % step == 0) {
                    end -= step / 2;
                }

                start += step / 2;
            } else {
                if ((end - start) % step == 0) {
                    start += step / 2;
                }

                end -= step / 2;
            }

            step *= 2;
            left = !left;
        }

        if (left) {
            // last direction is right
            return start;
        } else {
            return end;
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("390. Elimination Game") {
    Solution s;

    CHECK(s.lastRemaining(9) == 6);
    CHECK(s.lastRemaining(1) == 1);
    CHECK(s.lastRemaining(2) == 2);
    CHECK(s.lastRemaining(10) == 8);

    CHECK(s.lastRemaining(numeric_limits<int>::max()) == 0x40000000);
}
#endif
