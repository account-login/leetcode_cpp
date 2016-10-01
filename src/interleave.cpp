#include <string>
#include <queue>
#include <utility>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/interleaving-string/


using namespace std;


class Solution {
public:
    bool isInterleave(const string &s1, const string &s2, const string &s3) {
        if (s1.size() + s2.size() != s3.size()) {
            return false;
        }

        int s3_pos = 0;
        queue<pair<int, int>> q;
        q.push({ 0, 0 });

        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                pair<int, int> &p = q.front();
                if (p.first == s1.size() && p.second == s2.size()) {
                    return true;
                }

                if (p.first < s1.size() && s1[p.first] == s3[s3_pos]) {
                    pair<int, int> new_pair = { p.first + 1, p.second };
                    // avoid duplicates
                    if (q.back() != new_pair) {
                        q.push(move(new_pair));
                    }
                }

                if (p.second < s2.size() && s2[p.second] == s3[s3_pos]) {
                    q.push({ p.first, p.second + 1 });
                }

                q.pop();
            }

            s3_pos++;
        }

        return false;
    }
};


#ifdef RUN_TEST
TEST_CASE("97. Interleaving String") {
    Solution s;

    CHECK(s.isInterleave("aabcc", "dbbca", "aadbbcbcac"));
    CHECK_FALSE(s.isInterleave("aabcc", "dbbca", "aadbbbaccc"));

    CHECK(s.isInterleave("", "", ""));
    CHECK(s.isInterleave("", "a", "a"));
    CHECK(s.isInterleave("a", "", "a"));
    CHECK(s.isInterleave("a", "b", "ab"));
    CHECK(s.isInterleave("b", "a", "ab"));
    CHECK(s.isInterleave("aaaa", "aaaa", "aaaaaaaa"));

    CHECK(s.isInterleave("asdf", "df12as", "asdf12dfas"));
    CHECK(s.isInterleave("asdf", "df12as", "asdf12asdf"));
    CHECK_FALSE(s.isInterleave("asdf", "df12asas", "asdf12asdfdf"));
}
#endif
