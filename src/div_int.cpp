#include <iostream>
#include <limits>
#include <utility>
#include <cmath>
#include <cstdint>
#include <cassert>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#endif

// https://leetcode.com/problems/divide-two-integers/


#ifndef MAX_INT
#   define MAX_INT std::numeric_limits<int>::max()
#endif


using namespace std;


class Solution {
public:
    int divide(int dividend, int divisor) {
        if (divisor == 0) {
            return MAX_INT;
        }

        int64_t a = dividend;
        int64_t b = divisor;
        int64_t ans = this->div_pos(abs(a), abs(b));
        if (a * b < 0) {
            ans = -ans;
        }
        if (ans > std::numeric_limits<int>::max() || ans < std::numeric_limits<int>::min()) {
            return MAX_INT;
        } else {
            return ans;
        }
    }

    pair<uint64_t, uint64_t> div_mod_pos(uint64_t a, uint64_t b) {
        assert(b != 0);
        unsigned int shift = 0;
        uint64_t sub = b;
        // TODO: test a bisect search
        while (sub < a) {
            sub <<= 1;
            shift += 1;
        }

        uint64_t ans = 0;
        while (a >= b) {
            if (a >= sub) {
                a -= sub;
                ans += ((uint64_t)1 << shift);
            }
            sub >>= 1;
            shift -= 1;
        }

        return make_pair(ans, a);
    }

    uint64_t div_pos(uint64_t a, uint64_t b) {
        return this->div_mod_pos(a, b).first;
    }
};


#ifdef RUN_TEST
TEST_CASE("Divide Two Integers") {
    Solution s;

    CHECK(s.div_pos(4, 2) == 2);
    CHECK(s.div_pos(19, 3) == 6);
    CHECK(s.div_pos(18, 3) == 6);

    CHECK(s.divide(4, 0) == MAX_INT);
    CHECK(s.divide(7, 3) == 2);
    CHECK(s.divide(0, 8) == 0);

    CHECK(s.divide(-7, 3) == -2);
    CHECK(s.divide(-9, 3) == -3);
    CHECK(s.divide(-9, -3) == 3);
    CHECK(s.divide(-10, -3) == 3);
    CHECK(s.divide(11, -3) == -3);

    // overflowed
    CHECK(s.divide(-2147483648, -1) == MAX_INT);
}
#endif
