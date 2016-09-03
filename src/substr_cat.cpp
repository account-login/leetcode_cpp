#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

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

        unordered_map<string, int> word2id;
        vector<int> word_count;
        int uniq_word_count = 0;
        for (size_t i = 0; i < words.size(); i++) {
            if (word2id.count(words[i]) == 0) {
                word2id[words[i]] = uniq_word_count;
                uniq_word_count++;
                word_count.push_back(1);
            } else {
                int word_id = word2id[words[i]];
                word_count[word_id]++;
            }
        }

        vector<int> ans;

        // O( len(word) * (len(s) / len(word)) * len(word) )
        //    ^ start     ^ word_idx             ^ hash
        // = O( len(word) * len(s) )
        for (int start = 0; start < word_len && start <= (int)s.size() - block_len; start++) {
            // counter
            vector<int> visited(uniq_word_count, 0);
            // changes in sync with visited
            vector<bool> ok_word(uniq_word_count, false);
            size_t queue_size = 0;
            size_t word_idx = 0;

            while (start + (word_idx + 1) * word_len <= s.size()) {
                int word_pos = start + word_idx * word_len;
                string word = s.substr(word_pos, word_len);

                auto found = word2id.find(word);
                if (found == word2id.end()) {
                    // not matched, clear queue & counter
                    queue_size = 0;
                    fill(visited.begin(), visited.end(), 0);
                    fill(ok_word.begin(), ok_word.end(), false);
                } else {
                    // new matched word
                    int id = found->second;
                    queue_size++;
                    visited[id]++;

                    // visited counter modified, adjust ok_word set
                    if (visited[id] == word_count[id]) {
                        ok_word[id] = true;
                    } else {
                        ok_word[id] = false;
                    }

                    // queue too long, drop front
                    if (queue_size > words.size()) {
                        assert(queue_size == words.size() + 1);
                        int index = word_pos - (queue_size - 1) * word_len;
                        string queue_front = s.substr(index, word_len);
                        int front_id = word2id[queue_front];

                        // decrease found word count
                        visited[front_id]--;

                        // visited counter modified, adjust ok_word set
                        if (visited[front_id] == word_count[front_id]) {
                            ok_word[front_id] = true;
                        } else {
                            ok_word[front_id] = false;
                        }

                        queue_size--;
                        assert(queue_size == words.size());
                    }

                    // anwser found
                    if (queue_size == words.size()
                        && count(ok_word.begin(), ok_word.end(), true) == uniq_word_count)
                    {
                        int index = word_pos - (queue_size - 1) * word_len;
                        ans.push_back(index);
                    }
                }

                word_idx++;
            }
        }

        return ans;
    }

    vector<int> findSubstring_naive(const string &s, const vector<string> &words) {
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

        vector<int> ans;
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
