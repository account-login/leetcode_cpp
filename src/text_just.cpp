#include <iostream>
#include <vector>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/text-justification/


using namespace std;


class Solution {
public:
    vector<string> fullJustify(const vector<string> &words, size_t maxWidth) {
        vector<string> ans;

        size_t begin = 0;
        while (begin < words.size()) {
            int min_len = words[begin].size();
            size_t end = begin + 1;
            while (end < words.size() && words[end].size() + 1 + min_len <= maxWidth) {
                min_len += words[end].size() + 1;
                end++;
            }

            if (end == words.size()) {
                // last line
                string line = "";
                for (size_t i = begin; i < words.size(); i++) {
                    line += words[i];
                    if (i != words.size() - 1) {
                        line += " ";
                    }
                }
                line += string(maxWidth - line.size(), ' ');
                assert(line.size() == maxWidth);

                ans.push_back(line);
            } else {
                int word_count = end - begin;
                assert(word_count > 0);
                int total_spaces = maxWidth - min_len + (word_count - 1);
                assert(total_spaces >= 0);

                if (word_count == 1) {  // only one word, left justify it.
                    string line = words[begin] + string(total_spaces, ' ');
                    assert(line.size() == maxWidth);

                    ans.push_back(line);
                } else {
                    int avg_spaces = (total_spaces / (word_count - 1));
                    int plus_one_num = total_spaces - avg_spaces * (word_count - 1);

                    string line = "";
                    for (size_t i = begin; i < end; i++) {
                        line += words[i];
                        if (i != end - 1) {
                            if (plus_one_num > 0) {
                                line += " ";
                                plus_one_num--;
                            }
                            line += string(avg_spaces, ' ');
                        }
                    }
                    assert(line.size() == maxWidth);

                    ans.push_back(line);
                }
            }

            begin = end;
        }

        return ans;
    }
};


#ifdef RUN_TEST
TEST_CASE("68. Text Justification") {
    Solution s;

    vector<string> ans = {
        "This    is    an",
        "example  of text",
        "justification.  "
    };
    vector<string> words = { "This", "is", "an", "example", "of", "text", "justification." };
    CHECK(s.fullJustify(words, 16) == ans);

    words = { "asdf", "bbb" };
    ans = {
        "asdf   ",
        "bbb    "
    };
    CHECK(s.fullJustify(words, 7) == ans);
}
#endif
