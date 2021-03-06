#include <string>
#include <vector>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/simplify-path/


using namespace std;


class Solution {
public:
    string simplifyPath(const string &path) {
        vector<string> stk;

        bool is_abs = path[0] == '/';
        int word_begin = 0;
        for (size_t i = 0; i < path.size() + 1; i++) {
            if (path[i] == '/' || path[i] == '\0' ) {
                if (i == word_begin + 1 && path[i - 1] == '.') {
                    // pass .
                } else if (i == word_begin + 2
                           && path[i - 1] == '.' && path[i - 2] == '.')
                {
                    // pop ..
                    if (stk.empty()) {
                        if (!is_abs) {  // /.. -> /
                            stk.push_back("..");    // can not pop
                        }
                    } else {
                        if (stk.back() == "..") {
                            stk.push_back("..");    // can not pop
                        } else {
                            stk.pop_back();
                        }
                    }
                } else if (i > word_begin) {
                    // push word
                    stk.push_back(path.substr(word_begin, i - word_begin));
                }
                word_begin = i + 1;
            }
        }

        if (stk.empty()) {
            if (is_abs) {
                return "/";
            } else {
                return ".";
            }
        } else {
            string ans = "";
            for (const auto &str : stk) {
                if (is_abs) {
                    ans += "/";
                } else {    // skip leading '/' if not absolute path
                    is_abs = true;
                }
                ans += str;
            }
            return ans;
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("71. Simplify Path") {
    Solution s;

    CHECK(s.simplifyPath("/home/") == "/home");
    CHECK(s.simplifyPath("/a/./b/../../c/") == "/c");

    CHECK(s.simplifyPath(".") == ".");
    CHECK(s.simplifyPath("./asdf/../.") == ".");
    CHECK(s.simplifyPath("/") == "/");
    CHECK(s.simplifyPath("/asdf/../") == "/");

    CHECK(s.simplifyPath("..") == "..");
    CHECK(s.simplifyPath("./asdf/../../..") == "../..");
    CHECK(s.simplifyPath("./.././") == "..");

    CHECK(s.simplifyPath("../a") == "../a");
    CHECK(s.simplifyPath("../a/../../bcd") == "../../bcd");
    CHECK(s.simplifyPath("../../a/b/../bcd") == "../../a/bcd");
    CHECK(s.simplifyPath("asdf/d/../../../asd") == "../asd");

    CHECK(s.simplifyPath("asdf/asdf/../../cda/.") == "cda");
    CHECK(s.simplifyPath("./asdf/asdf/../../cda/.") == "cda");

    CHECK(s.simplifyPath("/..") == "/");
    CHECK(s.simplifyPath("/../../../advd") == "/advd");
    CHECK(s.simplifyPath("/adf/../../../advd") == "/advd");
}
#endif
