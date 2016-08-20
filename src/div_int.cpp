#include <iostream>
#include <limits>
#include <utility>
#include <cmath>
#include <cassert>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

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

        if (dividend > 0) {
            unsigned int div = this->div_pos(dividend, abs(divisor));
            if (divisor < 0) {
                div = -div;
            }

            return div;
        } else {
            unsigned int div, mod;
            tie(div, mod) = this->div_mod_pos(abs(dividend), abs(divisor));
            if (mod != 0) {
                div += 1;
            }
            if (divisor > 0) {
                div = -div;
            }

            return div;
        }
    }

    pair<unsigned int, unsigned int> div_mod_pos(unsigned int a, unsigned int b) {
        assert(b != 0);
        unsigned int shift = 0;
        unsigned int sub = b;
        while (sub < a) {
            sub <<= 1;
            shift += 1;
        }

        int ans = 0;
        while (a >= b) {
            if (a >= sub) {
                a -= sub;
                ans += 1 << shift;
            }
            sub >>= 1;
            shift -= 1;
        }

        return make_pair(ans, a);
    }

    unsigned int div_pos(unsigned int a, unsigned int b) {
        return this->div_mod_pos(a, b).first;
    }
};


TEST_CASE("Divide Two Integers") {
    Solution s;

    CHECK(s.div_pos(4, 2) == 2);
    CHECK(s.div_pos(19, 3) == 6);
    CHECK(s.div_pos(18, 3) == 6);

    CHECK(s.divide(4, 0) == MAX_INT);
    CHECK(s.divide(7, 3) == 2);
    CHECK(s.divide(0, 8) == 0);

    CHECK(s.divide(-7, 3) == -3);
    CHECK(s.divide(-9, 3) == -3);
    CHECK(s.divide(-9, -3) == 3);
    CHECK(s.divide(-10, -3) == 4);
    CHECK(s.divide(11, -3) == -3);
}
