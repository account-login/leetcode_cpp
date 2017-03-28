#include <algorithm>
#include <tuple>
#include <limits>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#   define private public
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/reverse-pairs/


using namespace std;


struct AVLNode {
    int32_t data;
    int16_t depth;
    uint16_t size;
    AVLNode *left;
    AVLNode *right;

    AVLNode(int32_t data)
        : data(data), depth(1), size(1), left(nullptr), right(nullptr)
    {}
};


int avl_depth(AVLNode *node) {
    if (node == nullptr) {
        return 0;
    } else {
        return node->depth;
    }
}


int avl_size(AVLNode *node) {
    if (node == nullptr) {
        return 0;
    } else {
        return node->size;
    }
}


AVLNode *avl_adjust(AVLNode *node) {
    node->depth = 1 + max(avl_depth(node->left), avl_depth(node->right));
    node->size = 1 + avl_size(node->left) + avl_size(node->right);
    return node;
}


AVLNode *avl_rotate_left(AVLNode *node) {
    AVLNode *root = node->right;
    node->right = root->left;
    root->left = avl_adjust(node);
    return avl_adjust(root);
}


AVLNode *avl_rotate_right(AVLNode *node) {
    AVLNode *root = node->left;
    node->left = root->right;
    root->right = avl_adjust(node);
    return avl_adjust(root);
}


AVLNode *avl_fix_left(AVLNode *node) {
    if (avl_depth(node->left->right) > avl_depth(node->left->left)) {
        node->left = avl_rotate_left(node->left);
    }
    return avl_rotate_right(node);
}


AVLNode *avl_fix_right(AVLNode *node) {
    if (avl_depth(node->right->left) > avl_depth(node->right->right)) {
        node->right = avl_rotate_right(node->right);
    }
    return avl_rotate_left(node);
}


AVLNode *avl_insert(AVLNode *root, AVLNode *node) {
    if (root == nullptr) {
        return node;
    } else if (node->data < root->data) {
        root->left = avl_insert(root->left, node);
        if (avl_depth(root->left) - avl_depth(root->right) == 2) {
            return avl_fix_left(root);
        }
    } else {
        root->right = avl_insert(root->right, node);
        if (avl_depth(root->right) - avl_depth(root->left) == 2) {
            return avl_fix_right(root);
        }
    }
    return avl_adjust(root);
}


int avl_count_gt(AVLNode *root, int data) {
    if (root == nullptr) {
        return 0;
    } else if (data < root->data) {
        return avl_count_gt(root->left, data) + 1 + avl_size(root->right);
    } else {
        return avl_count_gt(root->right, data);
    }
}


void avl_free(AVLNode *node) {
    if (node != nullptr) {
        avl_free(node->left);
        avl_free(node->right);
        delete node;
    }
}


struct AVLTree {
    AVLNode *root = nullptr;

    ~AVLTree() {
        avl_free(this->root);
    }

    void insert(int data) {
        AVLNode *node = new AVLNode(data);
        this->root = avl_insert(this->root, node);
    }

    size_t size() const {
        return avl_size(this->root);
    }

    int count_gt(int data) const {
        return avl_count_gt(this->root, data);
    }
};


class Solution {
public:
    int reversePairs(const vector<int> &nums) {
        AVLTree tree;
        int ans = 0;
        for (int n : nums) {
            int64_t test_num = 2 * (int64_t)n;
            if (test_num >= numeric_limits<int>::max()) {
                // noop
            } else if (test_num < numeric_limits<int>::min()) {
                ans += tree.size();
            } else {
                ans += tree.count_gt(test_num);
            }
            tree.insert(n);
        }
        return ans;
    }
};


#ifdef RUN_TEST
tuple<int, int> avl_verify(AVLNode *node) {
    if (node == nullptr) {
        return make_tuple(0, 0);
    }

    int ldepth, lsize, rdepth, rsize;
    tie(ldepth, lsize) = avl_verify(node->left);
    tie(rdepth, rsize) = avl_verify(node->right);

    if (node->left) {
        REQUIRE(node->left->data < node->data);
    }
    if (node->right) {
        REQUIRE(node->right->data >= node->data);
    }

    REQUIRE(node->depth == 1 + max(avl_depth(node->left), avl_depth(node->right)));
    REQUIRE(abs(ldepth - rdepth) <= 1);

    REQUIRE(node->size == 1 + avl_size(node->left) + avl_size(node->right));

    return make_tuple(avl_depth(node), avl_size(node));
}


TEST_CASE("AVLTree::insert() sequence", "[tree]") {
    AVLTree tree;
    for (int i = 0; i < 100; i++) {
        tree.insert(i);
        avl_verify(tree.root);
    }
}


TEST_CASE("AVLTree::insert() random", "[tree]") {
    AVLTree tree;
    for (int i = 0; i < 100; i++) {
        tree.insert(rand());
        avl_verify(tree.root);
    }
}


TEST_CASE("AVLTree::count_gt()", "[tree]") {
    AVLTree tree;
    CHECK(tree.count_gt(0) == 0);
    CHECK(tree.count_gt(100) == 0);

    for (int i = 0; i < 100; i++) {
        tree.insert(i);
        tree.insert(i);
    }

    CHECK(tree.count_gt(-1) == 200);
    for (int i = 0; i < 100; i++) {
        CHECK(tree.count_gt(i) == (100 - i - 1) * 2);
    }
    CHECK(tree.count_gt(100) == 0);
}


TEST_CASE("493. Reverse Pairs", "[solution]") {
    Solution s;
    CHECK(s.reversePairs({1, 3, 2, 3, 1}) == 2);
    CHECK(s.reversePairs({2, 4, 3, 5, 1}) == 3);

    CHECK(s.reversePairs({}) == 0);
    CHECK(s.reversePairs({1}) == 0);
    CHECK(s.reversePairs({2, 1}) == 0);

    const int MIN = numeric_limits<int>::min();
    const int MAX = numeric_limits<int>::max();
    CHECK(s.reversePairs({MAX, MAX}) == 0);
    CHECK(s.reversePairs({MAX, MAX, MIN, MIN, MIN, MAX}) == 9);
    CHECK(s.reversePairs({MIN, MIN / 2}) == 0);
    CHECK(s.reversePairs({MIN + 1, MIN / 2}) == 1);
}
#endif
