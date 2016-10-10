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
        char *str = (char *)s.data();
        int len = s.size();

        // reverse all
        for (int j = 0; j < len / 2; j++) {
            swap(str[j], str[len - j - 1]);
        }

        // reverse word
        for (int i = 0, begin = 0; i < len + 1; i++) {
            if (str[i] == ' ' || str[i] == '\0') {
                for (int j = 0; j < (i - begin) / 2; j++) {
                    swap(str[begin + j], str[i - j - 1]);
                }
                begin = i + 1;
            }
        }

        // remove space
        int src = 0;
        int dst = 0;
        while (str[src] != '\0') {
            if (str[src] == ' ' && (str[src + 1] == ' ' || str[src + 1] == '\0' || dst == 0)) {
                src++;
            } else {
                str[dst] = str[src];
                dst++;
                src++;
            }
        }

        // return
        s.resize(dst);
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
