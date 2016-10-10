#include <limits>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "tree_node.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/validate-binary-search-tree/


using namespace std;


/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    int prev;
    bool first;

    bool isValidBST(const TreeNode *root) {
        if (!root) {
            return true;
        }
        this->first = true;
        return travel(root);
    }

    bool travel(const TreeNode *root) {
        if (root->left) {
            bool ret = travel(root->left);
            if (!ret) {
                return false;
            }
        }

        if (!this->first && root->val <= this->prev) {
            return false;
        }
        this->prev = root->val;
        this->first = false;

        if (root->right) {
            bool ret = travel(root->right);
            if (!ret) {
                return false;
            }
        }

        return true;
    }
};


#ifdef RUN_TEST
TEST_CASE("98. Validate Binary Search Tree") {
    Solution s;

    // null tree
    CHECK(s.isValidBST(nullptr) == true);

    TreeNode *tree = new TreeNode(new TreeNode(1), 2, new TreeNode(3));
    CHECK(s.isValidBST(tree) == true);
    delete tree;

    tree = new TreeNode(new TreeNode(1), 2, new TreeNode(2));
    CHECK(s.isValidBST(tree) == false);
    delete tree;

    // reach int limits
    tree = new TreeNode(numeric_limits<int>::min());
    CHECK(s.isValidBST(tree) == true);
    delete tree;
}
#endif
