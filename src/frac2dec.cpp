#include <iostream>
#include <unordered_map>
#include <string>
#include <cmath>
#include <cassert>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

// https://leetcode.com/problems/fraction-to-recurring-decimal/


using namespace std;


class Solution {
public:
    string fractionToDecimal(int numerator, int denominator) {
        int sign = numerator * denominator;
        numerator = abs(numerator);
        denominator = abs(denominator);

        int integer = numerator / denominator;
        int mod = numerator % denominator;
        mod *= 10;

        unordered_map<int, size_t> mod2pos;
        vector<int> digits;
        size_t pos = 0;

        while (true) {
            if (mod == 0) {
                break;
            }

            auto found = mod2pos.find(mod);
            if (found != mod2pos.end()) {
                break;
            }

            mod2pos[mod] = pos;

            int digit = mod / denominator;
            digits.push_back(digit);

            mod %= denominator;
            mod *= 10;
            pos++;
        }

        string ret;
        if (sign < 0) {
            ret = "-";
        }

        ret += to_string(integer);

        if (digits.size() != 0) {
            ret += ".";
            if (mod == 0) {
                for (int digit : digits) {
                    ret += to_string(digit);
                }
            } else {
                size_t start_pos = mod2pos[mod];
                for (size_t i = 0; i < start_pos; i++) {
                    ret += to_string(digits[i]);
                }
                ret += "(";
                for (size_t i = start_pos; i < digits.size(); i++) {
                    ret += to_string(digits[i]);
                }
                ret += ")";
            }
        }

        return ret;
    }
};


TEST_CASE("Fraction to Recurring Decimal") {
    Solution s;

    CHECK(s.fractionToDecimal(0, 2) == "0");
    CHECK(s.fractionToDecimal(2, 1) == "2");
    CHECK(s.fractionToDecimal(1, 2) == "0.5");
    CHECK(s.fractionToDecimal(2, 3) == "0.(6)");
    CHECK(s.fractionToDecimal(25, 13) == "1.(923076)");
    CHECK(s.fractionToDecimal(11, 6) == "1.8(3)");
    CHECK(s.fractionToDecimal(11, -6) == "-1.8(3)");
}
