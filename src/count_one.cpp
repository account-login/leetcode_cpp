#include <utility>
#include <cmath>
#include <string>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   define private public
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/number-of-digit-one/


using namespace std;


class Solution {

private:
    int power10[10];
    int count_one_d[10];

public:
    Solution() {
        for (int i = 0; i < 10; i++) {
            this->power10[i] = (int)pow(10, i);
        }

        this->count_one_d[0] = 0;
        for (int i = 1; i < 10; i++) {
            this->count_one_d[i] = this->count_one_d[i - 1] * 10 + this->power10[i - 1];
        }
    }

    int countDigitOne(int n) {
        if (n <= 0) {
            return 0;
        }
        string digits = to_string(n);
        int first_digit = digits[0] - '0';
        int len = digits.size();

        int ones = count_one_d[len - 1];

        int total_ones = 0;
        total_ones += first_digit * ones;           // zxxx...
        if (first_digit > 1) {
            total_ones += this->power10[len - 1];   // 1xxx...
        }

        // yxxx..., deal with xxx...
        int sub = n - first_digit * this->power10[len - 1];
        if (sub != 0) {
            assert(to_string(sub).size() < len);
        }

        int ones_sub = countDigitOne(sub);
        if (first_digit == 1) {
            total_ones += ones_sub + sub + 1;
        } else {
            total_ones += ones_sub;
        }

        return total_ones;
    }
};


namespace std {
    template<typename T1, typename T2>
    std::ostream &operator << ( std::ostream &os, const pair<T1, T2> &p ) {
        os << "( " << p.first << ", " << p.second << " )";
        return os;
    }
}


#ifdef RUN_TEST
TEST_CASE("233. Number of Digit One") {
    Solution s;

    CHECK(s.count_one_d[0] == 0);
    CHECK(s.count_one_d[1] == 1);
    CHECK(s.count_one_d[2] == 20);

    CHECK(s.countDigitOne(0) == 0);
    CHECK(s.countDigitOne(1) == 1);
    CHECK(s.countDigitOne(9) == 1);
    CHECK(s.countDigitOne(13) == 6);
    CHECK(s.countDigitOne(99) == 20);
}
#endif
