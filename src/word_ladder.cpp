#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <cassert>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#endif

// https://leetcode.com/problems/word-ladder-ii/


using namespace std;


class Node {
public:
    string word;
    unordered_set<Node *> links;

    Node(string word) : word(word), links()
    {
        //
    }
};


void link(Node *n1, Node *n2) {
    n1->links.insert(n2);
    n2->links.insert(n1);
}


typedef vector<vector<string>> Results;
typedef vector<Node *> Path;
typedef vector<Path> PathList;
typedef unordered_map<string, int> DistMap;
typedef unordered_map<string, PathList> PathListMap;


void add_path(
    DistMap &dist_map,
    PathListMap &path_list_map,
    const PathList &from_path_list,
    const Node &node,
    int new_dist
) {
    dist_map[node.word] = new_dist;

    if (path_list_map.find(node.word) == path_list_map.end()) {
        path_list_map[node.word] = PathList();
    }
    PathList &target = path_list_map.find(node.word)->second;
    for (auto &path : from_path_list) {
        auto new_path = Path(path);
        new_path.push_back((Node *)&node);

        // check redundant path
        bool redundant = false;
        for (auto &old_target_path : target) {
            if (old_target_path == new_path) {
                redundant = true;
                break;
            }
        }
        if (!redundant) {
            target.push_back(new_path);
        }
    }
}


vector<vector<string>> shortest_path(Node &begin, Node &end) {
    // skip special case
    if (begin.word == end.word) {
        return {
            { begin.word }
        };
    }

    unordered_map<string, int> distances;
    unordered_map<string, PathList> pathes;
    distances[begin.word] = 0;
    pathes[begin.word] = {
        { &begin }
    };

    while (true) {
        bool brk = false;

        for (auto &path_list_kv : pathes) {
            auto path_list = path_list_kv.second;
            auto front_node = path_list.begin()->back();
            int dist = distances.find(front_node->word)->second;
            for (auto next : front_node->links) {
                if (next->word == end.word) {
                    brk = true;
                }
                int new_dist = dist + 1;
                auto found = distances.find(next->word);
                if (found == distances.end()) {
                    // add new path
                    add_path(distances, pathes, path_list, *next, new_dist);
                } else {
                    int prev_dist = found->second;
                    if (prev_dist == new_dist) {
                        // add new path
                        add_path(distances, pathes, path_list, *next, new_dist);
                    } else if (prev_dist > new_dist) {
                        FAIL("should not reach here");
                        // empty previous path and add new path
                        pathes[front_node->word] = PathList();
                        add_path(distances, pathes, path_list, *next, new_dist);
                    }
                }
            }
        }

        if (brk) {
            break;
        }
    }

    auto pathes_to_end = pathes[end.word];
    vector<vector<string>> ans;
    for (auto path : pathes_to_end) {
        vector<string> words;
        for (auto node : path) {
            words.push_back(node->word);
        }
        ans.push_back(words);
    }

    return ans;
}


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


pair<Node *, Node *> make_graph(string &begin_word, string &end_word, unordered_set<string> &wordList) {
    vector<Node *> nodes;
    Node *begin = nullptr;
    Node *end = nullptr;

    for (auto word : wordList) {
        auto new_node = new Node(word);
        for (auto node : nodes) {
            if (word_cmp(word, node->word)) {
                link(node, new_node);
            }
        }
        nodes.push_back(new_node);
        if (new_node->word == begin_word) {
            begin = new_node;
        }
        // begin_word may equals end_word
        if (new_node->word == end_word) {
            end = new_node;
        }
    }

    return make_pair(begin, end);
}


void destroy_graph(Node *begin) {
    unordered_set<Node *> visited_nodes;
    stack<Node *> to_explore;

    to_explore.push(begin);
    while (!to_explore.empty()) {
        auto node = to_explore.top();
        to_explore.pop();
        auto found = visited_nodes.find(node) ;
        if (found != visited_nodes.end()) {
            // found
        } else {
            // not found
            visited_nodes.insert(node);
            for (auto child : node->links) {
                to_explore.push(child);
            }
            delete node;
        }
    }
}


class Solution {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, unordered_set<string> &wordList) {
        Node *begin;
        Node *end;
        auto extended_words = wordList;
        extended_words.insert(beginWord);
        extended_words.insert(endWord);
        tie(begin, end) = make_graph(beginWord, endWord, extended_words);

        vector<vector<string>> ans = shortest_path(*begin, *end);
        destroy_graph(begin);
        return ans;
    }
};


#ifdef RUN_TEST
TEST_CASE( "126. Word Ladder II" ) {
    CHECK(word_cmp("asdf", "asdb") == true);
    CHECK(word_cmp("asdf", "asdf") == false);
    CHECK(word_cmp("asdf", "asas") == false);

    Solution s;
    unordered_set<string> words = {"hot", "dot", "dog", "lot", "log"};

    Node *begin;
    Node *end;
    string begin_word = "hit";
    string end_word = "cog";
    unordered_set<string> extended_words = words;
    extended_words.insert(begin_word);
    extended_words.insert(end_word);

    tie(begin, end) = make_graph(begin_word, end_word, extended_words);
    CHECK(begin->links.size() == 1);

    auto node_hot = *(begin->links.begin());
    CHECK(node_hot->word == "hot");
    CHECK(node_hot->links.size() == 3);

    Results results = s.findLadders("hit", "cog", words);
    Results expects = {
        {"hit", "hot", "dot", "dog", "cog"},
        {"hit", "hot", "lot", "log", "cog"}
    };
    // BUG: should use set comparison
    CHECK( results == expects );

    words = {"hit", "hot"};
    expects = {
        {"hit", "hot"}
    };
    CHECK(s.findLadders("hit", "hot", words) == expects);

    words = {"hit"};
    expects = {
        {"hit"}
    };
    CHECK(s.findLadders("hit", "hit", words) == expects);
}
#endif
