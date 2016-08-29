#include <iostream>
#include <vector>
#include <unordered_set>
#include <cstring>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/word-search-ii/


using namespace std;


///
/// \brief The Position struct
///
struct Position {
    size_t x;
    size_t y;
    Position(size_t x, size_t y) : x(x), y(y) {}
};

bool operator == (const Position &p1, const Position &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

namespace std {
    template<>
    struct hash<Position> {
        size_t operator ()(const Position &point) const {
            return point.x ^ point.y;
        }
    };
}


///
/// \brief The Tree struct
///
enum NodeType { ROOT, NODE, END };
struct Tree {
    NodeType type;
    union {
        char letter;
        const char *str;
    };
    vector<Tree *> children;
};


///
/// \brief The Solution class
///
class Solution {
public:
    typedef vector<vector<char>> Board;

    void add_to_tree(Tree *tree, const string &word, size_t index = 0) {
        if (word[index] == '\0') {
            for (auto child : tree->children) {
                if (child->type == END && strcmp(child->str, word.data()) == 0) {
                    return;
                }
            }

            auto end = new Tree;
            end->type = END;
            end->str = word.data();
            tree->children.push_back(end);
            return;
        } else {
            for (auto child : tree->children) {
                if (child->type == NODE && child->letter == word[index]) {
                    return add_to_tree(child, word, index + 1);
                }
            }

            auto node = new Tree;
            node->type = NODE;
            node->letter = word[index];
            tree->children.push_back(node);
            return add_to_tree(node, word, index + 1);
        }
    }

    Tree *make_tree(const vector<string> &words) {
        auto root = new Tree;
        root->type = ROOT;
        for (auto &word : words) {
            add_to_tree(root, word);
        }

        return root;
    }

    void destroy_tree(Tree *tree) {
        for (auto child : tree->children) {
            destroy_tree(child);
        }
        delete tree;
    }

    vector<string> findWords_notree(const Board &board, const vector<string> &words) {
        unordered_set<string> ans_set;

        for (auto &word : words) {
            if (solve_word(board, word)) {
                ans_set.insert(word);
            }
        }

        vector<string> ans(ans_set.begin(), ans_set.end());
        return ans;
    }

    vector<string> findWords(const Board &board, const vector<string> &words) {
        unordered_set<const char *> ans_str;
        auto root = make_tree(words);

        size_t xlen = board.size();
        size_t ylen = board[0].size();

        bool path[xlen][ylen];
        for (size_t i = 0; i < xlen; i++) {
            for (size_t j = 0; j < ylen; j++) {
                path[i][j] = false;
            }
        }

        for (size_t x = 0; x < xlen; x++) {
            for (size_t y = 0; y < ylen; y++) {
                for (auto tree : root->children) {
                    if (tree->letter == board[x][y]) {
                        auto pos = Position(x, y);
                        path[x][y] = true;
                        search_tree(ans_str, board, tree, (bool *)path, pos);
                        path[x][y] = false;
                        break;
                    }
                }
            }
        }

        destroy_tree(root);

        vector<string> ans;
        for (auto str : ans_str) {
            ans.push_back(string(str));
        }

        return ans;
    }

    unordered_set<string> find_words_set(const Board &board, const vector<string> &words) {
        auto vec_ans = findWords(board, words);
        unordered_set<string> ans(vec_ans.begin(), vec_ans.end());

        return ans;
    }

    bool solve_word(const Board &board, const string &word) {
        size_t xlen = board.size();
        size_t ylen = board[0].size();

        for (size_t x = 0; x < xlen; x++) {
            for (size_t y = 0; y < ylen; y++) {
                if (board[x][y] == word[0]) {
                    auto pos = Position(x, y);
                    unordered_set<Position> path = { pos };
                    bool ret = search_word(board, word.data() + 1, path, pos);
                    if (ret) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool search_word(const Board &board, const char *word, unordered_set<Position> &path, Position last_pos) {
        if (word[0] == '\0') {
            return true;
        }

        for (auto &&point : {
            Position(last_pos.x - 1, last_pos.y),
            Position(last_pos.x + 1, last_pos.y),
            Position(last_pos.x, last_pos.y - 1),
            Position(last_pos.x, last_pos.y + 1)
        }) {
            if (0 <= point.x && point.x < board.size()
                && 0 <= point.y && point.y < board[0].size()
            ) {
                auto found = path.find(point);
                if (found == path.end()) {
                    if (board[point.x][point.y] == word[0]) {
                        auto res = path.insert(point);
                        assert(res.second == true);
                        bool ret = search_word(board, word + 1, path, point);
                        path.erase(res.first);

                        if (ret) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    void search_tree(
        unordered_set<const char *> &ans,
        const Board &board,
        Tree *tree,
        bool *path,
        Position pos
    ) {
        auto ylen = board[0].size();
        auto xlen = board.size();

        for (auto child : tree->children) {
            if (child->type == END) {
                ans.insert(child->str);
            } else {
                for (auto &&point : {
                    Position(pos.x - 1, pos.y),
                    Position(pos.x + 1, pos.y),
                    Position(pos.x, pos.y - 1),
                    Position(pos.x, pos.y + 1)
                }) {
                    auto x = point.x;
                    auto y = point.y;
                    if (0 <= x && x < xlen
                        && 0 <= y && y < ylen
                    ) {
                        if (!path[x * ylen + y]) {
                            if (board[x][y] == child->letter) {
                                path[x * ylen + y] = true;
                                search_tree(ans, board, child, path, point);
                                path[x * ylen + y] = false;
                            }
                        }
                    }
                }
            }
        }

        // remove found words
        auto size = tree->children.size();
        for (size_t i = 0; i < size; i++) {
            if (tree->children[i]->type == END) {
                tree->children.erase(tree->children.begin() + i);
                size--;
            }
            if (i < size) {
                if (tree->children[i]->children.size() == 0) {
                    tree->children.erase(tree->children.begin() + i);
                    size--;
                }
            }
        }
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
TEST_CASE("Test tree") {
    Solution s;

    auto tree = s.make_tree({});
    CHECK(tree->children.size() == 0);
    s.destroy_tree(tree);

    vector<string> words = { "asd", "apd", "asd" };
    tree = s.make_tree(words);
    CHECK(tree->children.size() == 1);
    CHECK(tree->children[0]->children.size() == 2);
    CHECK(tree->children[0]->children[0]->children[0]->letter == 'd');
    CHECK(tree->children[0]->children[0]->children[0]->children[0]->str == words[0].data());
    s.destroy_tree(tree);
}

TEST_CASE("212. Word Search II") {
    Solution s;

    Solution::Board board = {
        {'o','a','a','n'},
        {'e','t','a','e'},
        {'i','h','k','r'},
        {'i','f','l','v'}
    };
    vector<string> words = { "oath", "pea", "eat", "rain" };
    unordered_set<string> ans = { "eat", "oath" };
    CHECK(s.find_words_set(board, words) == ans);

    board = {
        {'a', 'b'},
        {'c', 'd'}
    };
    words = { "abcd" };
    ans = {};
    CHECK(s.find_words_set(board, words) == ans);

    board = {
        { 'a', 'a', 'a', 'a' },
        { 'a', 'a', 'a', 'a' },
        { 'a', 'a', 'a', 'a' },
        { 'a', 'a', 'a', 'a' },
        { 'b', 'c', 'd', 'e' },
        { 'f', 'g', 'h', 'i' },
        { 'j', 'k', 'l', 'm' },
        { 'n', 'o', 'p', 'q' },
        { 'r', 's', 't', 'u' },
        { 'v', 'w', 'x', 'y' },
        { 'z', 'z', 'z', 'z' }
    };
    words = {};
    ans = {};
    for (auto i = 'a'; i <= 'z'; i++) {
        for (auto j = 'a'; j <= 'z'; j++) {
            string str = "aaaaaaaaaaaaaa";
            str.push_back(i);
            str.push_back(j);
            words.push_back(str);
        }
    }

    CHECK(s.find_words_set(board, words) != ans);
}
#endif
