#include <iostream>
#include <string>
#include <cctype>
#include <climits>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// select an implementation
#ifdef ALGO_FA
#   undef ALGO_REC
#   define isNumber_fa isNumber
#else
#   define ALGO_REC
#   undef ALGO_FA
#   define isNumber_rec isNumber
#endif
// https://leetcode.com/problems/valid-number/


using namespace std;


class Solution {
public:
    bool isNumber_rec(const string &s) {
        const char *str = s.data();
        // leading spaces
        while (isspace(str[0])) {
            str++;
        }

        int num_len = scan_sci_float(str);
        if(num_len == 0) {
            return false;
        } else {
            str += num_len;
        }

        // trailing spaces
        while (isspace(str[0])) {
            str++;
        }
        // is str ended?
        if (str[0] != '\0') {
            return false;
        } else {
            return true;
        }
    }

    int scan_sign(const char *str) {
        if (str[0] == '+' || str[0] == '-') {
            return 1;
        } else {
            return 0;
        }
    }

    /// @return return 0 on fail
    int scan_unsigned_integer(const char *str) {
        const char *origin_str = str;
        while (isdigit(*str)) {
            str++;
        }
        return str - origin_str;
    }

    int scan_integer(const char *str) {
        int sign_len = scan_sign(str);
        str += sign_len;

        int uint_len = scan_unsigned_integer(str);
        if (uint_len == 0) {
            return 0;
        } else {
            return uint_len + sign_len;
        }
    }

    int scan_simple_float(const char *str) {
        const char *origin_str = str;

        str += scan_sign(str);
        int uint1_len = scan_unsigned_integer(str);
        str += uint1_len;

        if (uint1_len == 0) {
            if (str[0] == '.') {
                str++;
                int uint2_len = scan_unsigned_integer(str);
                if (uint2_len == 0) {
                    return 0;
                } else {
                    str += uint2_len;
                }
            } else {
                return 0;
            }
        } else {
            if (str[0] == '.') {
                str++;
                str += scan_unsigned_integer(str);
            }
        }

        return str - origin_str;
    }

    int scan_sci_float(const char *str) {
        const char *origin_str = str;

        int simple_float_len = scan_simple_float(str);
        if (simple_float_len == 0) {
            return 0;
        }
        str += simple_float_len;

        if (str[0] == 'e' || str[0] == 'E') {
            str++;

            int exp_len = scan_integer(str);
            if (exp_len == 0) {
                return 0;
            }
            str += exp_len;
        }

        return str - origin_str;
    }

    bool isNumber_fa(const string &s) {
        enum class ST {
            BEGIN,              // 0
            SIGNED,             // 1
            HAS_DIGIT,          // 2
            DOTTED,             // 3
            DOTTED_NEED_DIGIT,  // 4
            EXP_BEGIN,          // 5
            EXP_SIGNED,         // 6
            EXP_HAS_DIGIT,      // 7
            PRE_END             // 8
        };

        ST st = ST::BEGIN;
        for (const char *p = s.data(); ; p++) {
            char ch = *p;
            if (st == ST::BEGIN) {                      // 0
                if (ch == '\0') {
                    return false;
                } else if (isspace(ch)) {
                    // pass
                } else if (ch == '.') {
                    st = ST::DOTTED_NEED_DIGIT;
                } else if (ch == '+' || ch == '-') {
                    st = ST::SIGNED;
                } else if (isdigit(ch)) {
                    st = ST::HAS_DIGIT;
                } else {
                    return false;
                }
            } else if (st == ST::SIGNED) {              // 1
                if (isdigit(ch)) {
                    st = ST::HAS_DIGIT;
                } else if (ch == '.') {
                    st = ST::DOTTED_NEED_DIGIT;
                } else {
                    return false;
                }
            } else if (st == ST::HAS_DIGIT) {           // 2
                if (isdigit(ch)) {
                    // pass
                } else if (ch == '.') {
                    st = ST::DOTTED;
                } else if (ch == 'e' || ch == 'E') {
                    st = ST::EXP_BEGIN;
                } else if (ch == '\0') {
                    return true;
                } else if (isspace(ch)) {
                    st = ST::PRE_END;
                } else {
                    return false;
                }
            } else if (st == ST::DOTTED) {              // 3
                if (isdigit(ch)) {
                    // pass
                } else if (ch == 'e' || ch == 'E') {
                    st = ST::EXP_BEGIN;
                } else if (ch == '\0') {
                    return true;
                } else if (isspace(ch)) {
                    st = ST::PRE_END;
                } else {
                    return false;
                }
            } else if (st == ST::DOTTED_NEED_DIGIT) {   // 4
                if (isdigit((ch))) {
                    st = ST::DOTTED;
                } else {
                    return false;
                }
            } else if (st == ST::EXP_BEGIN) {           // 5
                if (ch == '+' || ch == '-') {
                    st = ST::EXP_SIGNED;
                } else if (isdigit(ch)) {
                    st = ST::EXP_HAS_DIGIT;
                } else {
                    return false;
                }
            } else if (st == ST::EXP_SIGNED) {          // 6
                if (isdigit(ch)) {
                    st = ST::EXP_HAS_DIGIT;
                } else {
                    return false;
                }
            } else if (st == ST::EXP_HAS_DIGIT) {       // 7
                if (isdigit(ch)) {
                    // pass
                } else if (ch == '\0') {
                    return true;
                } else if (isspace(ch)) {
                    st = ST::PRE_END;
                } else {
                    return false;
                }
            } else if (st == ST::PRE_END) {             // 8
                if (ch == '\0') {
                    return true;
                } else if (isspace(ch)) {
                    // pass
                } else {
                    return false;
                }
            } else {
                assert(false);
            }
        }

        assert(false);
    }
};


#ifdef RUN_TEST
// 1. decimal numbers

// pos_integer:
// [digit]+

// integer:
// [sign]?[pos_integer]

// simple_float:
// [sign]?[.[pos_integer]]
// [sign]?[pos_integer][.[pos_integer]?]?
//         __ uint1__   _____uint2_____

// sci_float:
// [simple_float][E[integer]]?

// 2. other radixes

// [sign]?0x[digit]+

#ifdef ALGO_REC
TEST_CASE("Valid decimal integer") {
    Solution s;

    CHECK(s.scan_integer("") == 0);
    CHECK(s.scan_integer("asdf") == 0);
    CHECK(s.scan_integer("0") == 1);
    CHECK(s.scan_integer("123a") == 3);
    CHECK(s.scan_integer("+1") == 2);
    CHECK(s.scan_integer("-123b") == 4);
    CHECK(s.scan_integer("+") == 0);
    CHECK(s.scan_integer("-") == 0);
}

TEST_CASE("Valid float") {
    Solution s;

    CHECK(s.scan_simple_float("") == 0);
    CHECK(s.scan_simple_float("+") == 0);
    CHECK(s.scan_simple_float("+.") == 0);
    CHECK(s.scan_simple_float("-.e4") == 0);
    CHECK(s.scan_simple_float(".1asdf") == 2);
    CHECK(s.scan_simple_float("1.") == 2);
    CHECK(s.scan_simple_float("-1.234") == 6);
    CHECK(s.scan_simple_float(".") == 0);
    CHECK(s.scan_simple_float("123") == 3);

    CHECK(s.scan_sci_float("") == 0);
    CHECK(s.scan_sci_float(".e3") == 0);
    CHECK(s.scan_sci_float("1.e-2asdf") == 5);
    CHECK(s.scan_sci_float("234") == 3);
    CHECK(s.scan_sci_float("1.23e-") == 0);
}
#endif  // ALGO_REC

TEST_CASE("65. Valid Number") {
    Solution s;

    CHECK(s.isNumber("") == false);
    CHECK(s.isNumber(" asdf") == false);
    CHECK(s.isNumber(" 0123b") == false);
    CHECK(s.isNumber(" 0123 b") == false);

    CHECK(s.isNumber(" 123 ") == true);
    CHECK(s.isNumber("+1.123e-100  ") == true);

    CHECK(s.isNumber("0") == true);
    CHECK(s.isNumber("+0") == true);
    CHECK(s.isNumber("+") == false);
    CHECK(s.isNumber(".0") == true);
    CHECK(s.isNumber("0.") == true);
    CHECK(s.isNumber("0. ") == true);
    CHECK(s.isNumber("0.E") == false);
    CHECK(s.isNumber("0.+") == false);
    CHECK(s.isNumber("0.0") == true);
    CHECK(s.isNumber("+.0") == true);
    CHECK(s.isNumber(".") == false);
    CHECK(s.isNumber("+.") == false);
    CHECK(s.isNumber("0.E1") == true);
    CHECK(s.isNumber(".0E1") == true);
    CHECK(s.isNumber(".E1") == false);
    CHECK(s.isNumber("0E0") == true);
    CHECK(s.isNumber("0E0.") == false);
    CHECK(s.isNumber("0E") == false);
    CHECK(s.isNumber("0E+") == false);
    CHECK(s.isNumber("0E+0") == true);
    CHECK(s.isNumber("0E+.1") == false);
    CHECK(s.isNumber("E0") == false);
}
#endif

