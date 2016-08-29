#include <iostream>
#include <unordered_set>
#include <cstring>
#include <vector>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/word-break-ii/


using namespace std;


class Solution {
public:
    vector<string> wordBreak(const string &s, const unordered_set<string> &wordDict) {
        if (s.size() == 0) {
            return {};
        }

        vector<int> link_back[s.size() + 1];
        for (size_t i = 1; i <= s.size(); i++) {
            for (auto &word : wordDict) {
                int last_word_pos = i - word.size();
                if (last_word_pos >= 0) {
                    if (last_word_pos == 0 || link_back[last_word_pos].size() != 0) {
                        if (memcmp(s.data() + last_word_pos, word.data(), word.size()) == 0) {
                            link_back[i].push_back(last_word_pos);
                        }
                    }
                }
            }
        }

        return gen_answer(s, link_back, s.size());
    }

    vector<string> gen_answer(const string &s, const vector<int> *link_back, int index) {
        vector<string> ans_str;
        for (int last_index : link_back[index]) {
            if (last_index == 0) {
                ans_str.push_back(s.substr(0, index));
            } else {
                for (auto &str : gen_answer(s, link_back, last_index)) {
                    str += " " + s.substr(last_index, index - last_index);
                    ans_str.push_back(str);
                }
            }
        }

        return ans_str;
    }

    unordered_set<string> word_break_set(const string &s, const unordered_set<string> &wordDict) {
       unordered_set<string> ans;
       for (auto &str : wordBreak(s, wordDict)) {
           ans.insert(str);
       }
       return ans;
    }
};


namespace std {
    ostream &operator << (ostream &os, const unordered_set<string> &s) {
        os << "{ ";
        string sep = "";
        for (auto &str : s) {
            os << sep;
            os << "\"" << str << "\"";
            sep = ", ";
        }
        os << " }";
        return os;
    }
}


#ifdef RUN_TEST
TEST_CASE("140. Word Break II") {
    Solution s;

    unordered_set<string> ans = { "cats and dog", "cat sand dog" };
    CHECK(s.word_break_set("catsanddog", { "cat", "cats", "and", "sand", "dog" }) == ans);

    ans = { "asdf" };
    CHECK(s.word_break_set("asdf", { "asdf" }) == ans);

    ans = {};
    CHECK(s.word_break_set("asdf", {}) == ans);
}
#endif
