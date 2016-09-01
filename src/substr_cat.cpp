#include <iostream>
#include <unordered_set>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/substring-with-concatenation-of-all-words/


using namespace std;


class Solution {
public:
    vector<int> findSubstring(const string &s, const vector<string> &words) {
        if (words.size() == 0) {
            return {};
        }

        int word_len = words[0].size();
        int block_len = word_len * words.size();

        unordered_multiset<string> word_set;
        unordered_set<string> uniq_word_set;
        for (auto &word : words) {
            word_set.insert(word);
            uniq_word_set.insert(word);
        }
        size_t uniq_word_count = uniq_word_set.size();

        vector<int> ans;

        /* naive search
        // O( len(s) * len(words) * len(word) )
        //    ^ i      ^ word_idx   ^ hash
        for (int i = 0; i <= (int)s.size() - block_len; i++) {
            bool ok = true;
            unordered_multiset<string> visited;

            for (size_t word_idx = 0; word_idx < words.size(); word_idx++) {
                string word = s.substr(i + word_idx * word_len, word_len);
                if (word_set.count(word) == 0) {
                    // not matched
                    ok = false;
                    break;
                } else {
                    if (visited.count(word) < word_set.count(word)) {
                        // matched, not fully visited
                        visited.insert(word);
                    } else {
                        // matched, visited
                        ok = false;
                        break;
                    }
                }
            }

            if (ok) {
                ans.push_back(i);
            }
        }
        */

        // O( len(word) * (len(s) / len(word)) * len(word) )
        //    ^ start     ^ word_idx             ^ hash
        // = O( len(word) * len(s) )
        for (int start = 0; start < word_len && start <= (int)s.size() - block_len; start++) {
            unordered_multiset<string> visited;
            size_t queue_size = 0;
            size_t word_idx = 0;
            unordered_set<string> ok_word;

            while (start + (word_idx + 1) * word_len <= s.size()) {
                int word_pos = start + word_idx * word_len;
                string word = s.substr(word_pos, word_len);
                if (word_set.count(word) == 0) {
                    // not matched, clear queue & set
                    queue_size = 0;
                    visited.clear();
                    ok_word.clear();
                } else {
                    // new matched word
                    queue_size++;
                    visited.insert(word);

                    // visited counter modified, adjust ok_word set
                    if (visited.count(word) == word_set.count(word)) {
                        ok_word.insert(word);
                    } else {
                        ok_word.erase(word);
                    }

                    // queue too long, drop front
                    if (queue_size > words.size()) {
                        assert(queue_size == words.size() + 1);
                        int index = word_pos - (queue_size - 1) * word_len;
                        string queue_front = s.substr(index, word_len);

                        // decrease found word count
                        visited.erase(visited.equal_range(queue_front).first);

                        // visited counter modified, adjust ok_word set
                        if (visited.count(queue_front) == word_set.count(queue_front)) {
                            ok_word.insert(queue_front);
                        } else {
                            ok_word.erase(queue_front);
                        }

                        queue_size--;
                        assert(queue_size == words.size());
                    }

                    // anwser found
                    if (queue_size == words.size() && ok_word.size() == uniq_word_count) {
                        int index = word_pos - (queue_size - 1) * word_len;
                        ans.push_back(index);
                    }
                }

                word_idx++;
            }
        }

        return ans;
    }
};


#ifdef RUN_TEST
TEST_CASE("30. Substring with Concatenation of All Words") {
    Solution s;

    vector<int> ans = { 0, 9 };
    CHECK(s.findSubstring("barfoothefoobarman", { "foo", "bar" }) == ans);

    ans = { 0 };
    CHECK(s.findSubstring("adsf", { "sf", "ad" }) == ans);

    ans = {};
    CHECK(s.findSubstring("asdfasdf", { "asd", "ljk", "234" }) == ans);

    ans = {};
    CHECK(s.findSubstring("", { "a" }) == ans);

    ans = {};
    CHECK(s.findSubstring("adsf", {}) == ans);

    ans = { 8 };
    CHECK(s.findSubstring("wordgoodgoodgoodbestword", { "word", "good", "best", "good" }) == ans);

    string str = "";
    for (int i = 0; i < 5000; i++) {
        str += "ab";
    }
    vector<string> words;
    for (int i = 0; i < 100; i++) {
        words.push_back("ab");
        words.push_back("ba");
    }
    ans = {};
    CHECK(s.findSubstring(str, words) == ans);
}
#endif
