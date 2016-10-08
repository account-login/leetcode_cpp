#include <utility>
#include <cmath>
#include <string>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/number-of-digit-one/


using namespace std;


class Solution {
private:
    int power10[10];
public:
    Solution() {
        for (int i = 0; i < 10; i++) {
            this->power10[i] = (int)pow(10, i);
        }
    }

    int countDigitOne(int n) {
        if (n <= 0) {
            return 0;
        }
        string digits = to_string(n);
        int first_digit = digits[0] - '0';
        int len = digits.size();

        int ones, nums;
        tie(ones, nums) = count_one_d(len - 1);

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

    //  ones, nums
    pair<int, int> count_one_d(int d) {
        if (d == 0) {
            return { 0, 0 };
        } else {
            int ones, nums;
            tie(ones, nums) = count_one_d(d - 1);

            int ret_ones = (this->power10[d - 1] + ones) + ones * 8 +    ones;
            //             ^ 1xxx...                       ^ [2-9]xxx... ^ 0xxx...
            int ret_nums = nums * 8 +   nums +    this->power10[d - 1];
            //             ^[2-9]xxx... ^ 0xxx... ^ 1xxx...
            return { ret_ones, ret_nums };
        }
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

    CHECK(s.count_one_d(0) == make_pair(0, 0));
    CHECK(s.count_one_d(1) == make_pair(1, 1));
    CHECK(s.count_one_d(2) == make_pair(20, 19));

    CHECK(s.countDigitOne(0) == 0);
    CHECK(s.countDigitOne(1) == 1);
    CHECK(s.countDigitOne(9) == 1);
    CHECK(s.countDigitOne(13) == 6);
    CHECK(s.countDigitOne(99) == 20);
}
#endif
