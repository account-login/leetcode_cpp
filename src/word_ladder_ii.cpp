#include <algorithm>
#include <unordered_set>
#include <limits>
#include <iterator>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/word-ladder-ii/


using namespace std;


bool word_cmp(const string &s1, const string &s2)
{
    int count = 0;
    assert(s1.size() == s2.size());
    for (size_t i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i]) {
            count++;
            if (count > 1) {
                return false;
            }
        }
    }
    return count == 1;
}


vector<vector<int>> make_graph(const vector<string> &words) {
    vector<vector<int>> ans(words.size());
    for (size_t j = 1; j < words.size(); ++j) {
        for (size_t i = 0; i < j; ++i) {
            if (word_cmp(words[i], words[j])) {
                ans[i].push_back(static_cast<int>(j));
                ans[j].push_back(static_cast<int>(i));
            }
        }
    }
    return ans;
}


vector<vector<int>> shortest_path(const vector<vector<int>> &graph, int begin, int end) {
    vector<int> dist(graph.size(), numeric_limits<int>::max());
    dist[begin] = 0;
    vector<vector<int>> back_graph(graph.size());

    vector<int> cur_level;
    cur_level.push_back(begin);
    vector<int> next_level;

    while (!cur_level.empty()) {
        for (int cur : cur_level) {
            int cur_dist = dist[cur];
            for (int next : graph[cur]) {
                if (dist[next] == numeric_limits<int>::max()) {
                    next_level.push_back(next);
                }
                if (cur_dist + 1 <= dist[next]) {
                    back_graph[next].push_back(cur);
                    dist[next] = cur_dist + 1;
                }
            }
        }

        if (find(next_level.begin(), next_level.end(), end) != next_level.end()) {
            return back_graph;
        }
        cur_level.clear();
        cur_level.swap(next_level);
    }

    // no path from begin to end
    return {};
}


vector<vector<string>>
back_graph_to_path(
    const vector<vector<int>> &back_graph, int begin, int end, const vector<string> &words)
{
    if (begin == end) {
        return {
            {words[end]},
        };
    } else {
        vector<vector<string>> ans;
        for (int prev : back_graph[end]) {
            vector<vector<string>> sub = back_graph_to_path(back_graph, begin, prev, words);
            ans.reserve(ans.size() + sub.size());
            for (vector<string> &subpath : sub) {
                ans.emplace_back(move(subpath));
            }
        }
        for (vector<string> &path : ans) {
            path.emplace_back(words[end]);
        }
        return ans;
    }
}


template<class T>
int vector_unique_add(vector<T> &vec, const T &value) {
    auto it = find(vec.cbegin(), vec.cend(), value);
    if (it == vec.cend()) {
        vec.emplace_back(value);
        return static_cast<int>(vec.size() - 1);
    } else {
        return static_cast<int>(distance(vec.cbegin(), it));
    }
}


class Solution {
public:
    vector<vector<string>>
    findLadders(const string &begin_word, const string &end_word, vector<string> word_list) {
        int begin = vector_unique_add(word_list, begin_word);
        int end = static_cast<int>(
            find(word_list.cbegin(), word_list.cend(), end_word) - word_list.cbegin()
        );
        if (end == static_cast<int>(word_list.size())) {
            return {};
        }

        vector<vector<int>> graph = make_graph(word_list);
        vector<vector<int>> back_graph = shortest_path(graph, begin, end);
        if (back_graph.size() == 0) {
            return {};
        } else {
            return back_graph_to_path(back_graph, begin, end, word_list);
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("126. Word Ladder II") {
    CHECK(word_cmp("asdf", "asdb"));
    CHECK(!word_cmp("asdf", "asdf"));
    CHECK(!word_cmp("asdf", "asas"));

    Solution s;
    vector<vector<string>> expects;

    auto solve = [&](
        const string &word_begin, const string &word_end, const vector<string> &words)
    {
        auto ans = s.findLadders(word_begin, word_end, words);
        sort(ans.begin(), ans.end());
        return ans;
    };

    expects = {};
    CHECK(solve("ab", "ba", {}) == expects);
    CHECK(solve("ab", "ba", {"zz"}) == expects);
    CHECK(solve("a", "b", {"zz"}) == expects);

    expects = {
        {"hit", "hot", "dot", "dog", "cog"},
        {"hit", "hot", "lot", "log", "cog"}
    };
    CHECK(solve("hit", "cog", {"hot", "dot", "dog", "lot", "log", "cog"}) == expects);

    expects = {
        {"hit", "hot"}
    };
    CHECK(solve("hit", "hot", {"hit", "hot"}) == expects);
    CHECK(solve("hit", "hot", {"hot"}) == expects);
    CHECK(solve("hit", "hot", {"xxx", "hot"}) == expects);

    expects = {
        {"xxxa", "xxxy", "xxzy", "xbzy", "abzy"},
    };
    CHECK(solve("xxxa", "abzy", {"xbzy", "xxzy", "xxxy", "abzy"}) == expects);

    expects = {
        {"xxxx", "xxxa", "xxca", "yxca", "yxaa", "zxaa", "zzaa", "zzya", "zzyy"},
        {"xxxx", "xxxa", "xxca", "yxca", "yxcb", "zxcb", "zzcb", "zzyb", "zzyy"},
        {"xxxx", "xxxb", "xxab", "yxab", "yxaa", "zxaa", "zzaa", "zzya", "zzyy"},
        {"xxxx", "xxxb", "xxab", "yxab", "yxcb", "zxcb", "zzcb", "zzyb", "zzyy"},
    };
    CHECK(solve("xxxx", "zzyy", {
        "xxxa", "xxxb",
        "xxca", "xxab", "yxca", "yxab",
        "yxcb", "yxaa", "zxcb", "zxaa",
        "zzcb", "zzaa",
        "zzyb", "zzya",
        "zzyy"
    }) == expects);
}
#endif
