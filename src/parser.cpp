#include <iostream>
#include <cctype>
#include <cstdlib>
#include <vector>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#   include "parser.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/mini-parser/


using namespace std;


/**
 * // This is the interface that allows for creating nested lists.
 * // You should not implement it, or speculate about its implementation
 * class NestedInteger {
 *   public:
 *     // Constructor initializes an empty nested list.
 *     NestedInteger();
 *
 *     // Constructor initializes a single integer.
 *     NestedInteger(int value);
 *
 *     // Return true if this NestedInteger holds a single integer, rather than a nested list.
 *     bool isInteger() const;
 *
 *     // Return the single integer that this NestedInteger holds, if it holds a single integer
 *     // The result is undefined if this NestedInteger holds a nested list
 *     int getInteger() const;
 *
 *     // Set this NestedInteger to hold a single integer.
 *     void setInteger(int value);
 *
 *     // Set this NestedInteger to hold a nested list and adds a nested integer to it.
 *     void add(const NestedInteger &ni);
 *
 *     // Return the nested list that this NestedInteger holds, if it holds a nested list
 *     // The result is undefined if this NestedInteger holds a single integer
 *     const vector<NestedInteger> &getList() const;
 * };
 */
class Solution {
public:
    NestedInteger deserialize(const string &str) {
        const char *s = str.data();
        auto ni = parse(s);
        assert(*s == '\0');
        return ni;
    }

    NestedInteger parse(const char *&str) {
        if (isdigit(str[0]) || str[0] == '-') {
            int value = atoi(str);
            if (str[0] == '-') {
                str++;
            }
            while (isdigit(*str)) {
                str++;
            }
            return NestedInteger(value);
        } else {
            assert(str[0] == '[');
            auto ni = NestedInteger();
            str++;

            while (*str != ']') {
                auto sub = parse(str);
                ni.add(sub);
                if (*str == ',') {
                    str++;
                } else {
                    assert(*str == ']');
                }
            }
            assert(*str == ']');
            str++;
            return ni;
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("385. Mini Parser") {
    Solution s;

    NestedInteger ni = s.deserialize("[123,[456,[789]]]");
    CHECK(ni.isInteger() == false);
    CHECK(ni.getList()[0].getInteger() == 123);
    CHECK(ni.getList()[1].isInteger() == false);
    CHECK(ni.getList()[1].getList()[0].getInteger() == 456);
    CHECK(ni.getList()[1].getList()[1].getList()[0].getInteger() == 789);

    ni = s.deserialize("1234");
    CHECK(ni.getInteger() == 1234);

    ni = s.deserialize("[]");
    CHECK(ni.getList().size() == 0);

    ni = s.deserialize("[[],[[]]]");
    CHECK(ni.getList()[1].getList()[0].getList().size() == 0);

    int N = 5000;
    ni = s.deserialize(string(N, '[') + string(N, ']'));
    auto lst = &ni.getList();
    for (int i = 0; i < N - 1; i++) {
        lst = &(*lst)[0].getList();
    }
    CHECK(lst->size() == 0);
}
#endif
