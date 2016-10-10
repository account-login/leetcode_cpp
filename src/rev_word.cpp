#include <string>
#include <algorithm>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/reverse-words-in-a-string/


using namespace std;


class Solution {
public:
    void reverseWords(string &s) {
        reverse(s.begin(), s.end());

        auto begin = s.begin();
        while (begin < s.end()) {
            auto end = begin + 1;
            while (*end != ' ' && *end != '\0') {
                end++;
            }
            reverse(begin, end);
            begin = end + 1;
        }

        auto src = s.begin();
        auto dst = s.begin();
        while (true) {
            if (*src == '\0') {
                s.resize(dst - s.begin());
                return;
            }

            if (src[0] == ' ' && (src[1] == ' ' || src[1] == '\0' || dst == s.begin())) {
                src++;
            } else {
                *dst = *src;
                dst++;
                src++;
            }
        }
    }

    string reversed(string s) {
        this->reverseWords(s);
        return s;
    }
};


#ifdef RUN_TEST
TEST_CASE("151. Reverse Words in a String") {
    Solution s;

    CHECK(s.reversed("") == "");
    CHECK(s.reversed("  ") == "");
    CHECK(s.reversed("  a") == "a");
    CHECK(s.reversed("a  ") == "a");
    CHECK(s.reversed("   a  ") == "a");
    CHECK(s.reversed("asdf") == "asdf");
    CHECK(s.reversed("asdf 123") == "123 asdf");
    CHECK(s.reversed("  12   34     56  1  ") == "1 56 34 12");
}
#endif
