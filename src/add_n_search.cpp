#include <string>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/add-and-search-word-data-structure-design/


using namespace std;


struct Trie {
    char ch = '\0';
    bool is_end = false;
    Trie *children[26] = {};

    explicit Trie(char ch) : ch(ch) {}

    ~Trie() {
        for (Trie *p : children) {
            delete p;
        }
    }
};


class WordDictionary {
public:
    Trie *root;

    WordDictionary() : root(new Trie('\0')) {}

    /// \todo Rule of Three

    ~WordDictionary() {
        delete root;
    }

    WordDictionary(const WordDictionary &dct) = delete;

    // Adds a word into the data structure.
    void addWord(const string &word) {
        Trie *cur = root;
        for (char ch : word) {
            assert('a' <= ch && ch <= 'z');

            if (cur->children[ch - 'a'] == nullptr) {
                cur->children[ch - 'a'] = new Trie(ch);
            }
            cur = cur->children[ch - 'a'];
        }
        cur->is_end = true;
    }

    // Returns if the word is in the data structure. A word could
    // contain the dot character '.' to represent any one letter.
    bool search(const string &word) const {
        return search(word.data(), root);
    }

    bool search(const char *word, Trie *cur) const {
        while (true) {
            if (*word == '.') {
                word++;

                if (*word == '\0') {
                    for (Trie *child : cur->children) {
                        if (child != nullptr) {
                            if (child->is_end) {
                                return true;
                            }
                        }
                    }

                    return false;
                } else {
                    for (Trie *child : cur->children) {
                        if (child != nullptr) {
                            bool ret = search(word, child);
                            if (ret) {
                                return true;
                            }
                        }
                    }

                    return false;
                }
            } else {
                assert('a' <= *word && *word <= 'z');
                cur = cur->children[*word - 'a'];
                if (cur == nullptr) {
                    return false;
                }

                word++;
                if (*word == '\0') {
                    return cur->is_end;
                }
            }
        }
    }
};

// Your WordDictionary object will be instantiated and called as such:
// WordDictionary wordDictionary;
// wordDictionary.addWord("word");
// wordDictionary.search("pattern");


#ifdef RUN_TEST
TEST_CASE("211. Add and Search Word - Data structure design") {
    WordDictionary dict;

    // test addWord()
    dict.addWord("a");
    CHECK(dict.root->children['a' - 'a']->ch == 'a');
    CHECK(dict.root->children['a' - 'a']->is_end == true);

    dict.addWord("ab");
    CHECK(dict.root->children['a' - 'a']->children['b' - 'a']->ch == 'b');
    CHECK(dict.root->children['a' - 'a']->children['b' - 'a']->is_end == true);

    // test search()
    CHECK(dict.search("a") == true);
    CHECK(dict.search("b") == false);
    CHECK(dict.search("ab") == true);
    CHECK(dict.search("abc") == false);

    CHECK(dict.search(".") == true);
    CHECK(dict.search("..") == true);
    CHECK(dict.search(".b") == true);
    CHECK(dict.search("a.") == true);

    dict.addWord("abc");
    dict.addWord("abx");
    CHECK(dict.search("..b") == false);
    CHECK(dict.search("..c") == true);
    CHECK(dict.search("ab.") == true);

    dict.addWord("xxx");
    CHECK(dict.search("xx") == false);
    CHECK(dict.search("x.") == false);

    dict.addWord(string(10000, 'z'));
    dict.addWord(string(9999, 'y'));
    dict.addWord(string(9998, 'x'));
    CHECK(dict.search(string(10000, '.')) == true);
    CHECK(dict.search(string(9997, '.')) == false);
}
#endif
