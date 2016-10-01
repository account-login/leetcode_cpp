#include <algorithm>
#include <limits>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "tree_node.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/binary-tree-maximum-path-sum/


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
    int maxPathSum(TreeNode *root) {
        if (root) {
            int ans = numeric_limits<int>::min();
            travel(root, ans);
            return ans;
        } else {
            return 0;
        }
    }

    int travel(TreeNode *root, int &ans) {
        int left_sum = 0;
        if (root->left) {
            left_sum = travel(root->left, ans);
        }

        int right_sum = 0;
        if (root->right) {
            right_sum = travel(root->right, ans);
        }

        int new_ans = root->val;
        if (left_sum > 0) {
            new_ans += left_sum;
        }
        if (right_sum > 0) {
            new_ans += right_sum;
        }

        ans = max(ans, new_ans);
        return max({ left_sum, right_sum, 0 }) + root->val;
    }
};


#ifdef RUN_TEST
TEST_CASE("124. Binary Tree Maximum Path Sum") {
    Solution s;

    CHECK(s.maxPathSum(nullptr) == 0);

    TreeNode *tree = new TreeNode(new TreeNode(2), 1, new TreeNode(3));
    CHECK(s.maxPathSum(tree) == 6);
    delete tree;

    tree = new TreeNode(new TreeNode(new TreeNode(2), 1, new TreeNode(3)), 1, nullptr);
    CHECK(s.maxPathSum(tree) == 6);
    delete tree;

    tree = new TreeNode(new TreeNode(new TreeNode(2), 1, new TreeNode(3)), 6, nullptr);
    CHECK(s.maxPathSum(tree) == 10);
    delete tree;

    tree = new TreeNode(-3);
    CHECK(s.maxPathSum(tree) == -3);
    delete tree;

    tree = new TreeNode(new TreeNode(-2), -3, new TreeNode(-1));
    CHECK(s.maxPathSum(tree) == -1);
    delete tree;

    tree = new TreeNode(new TreeNode(2), -1, new TreeNode(3));
    CHECK(s.maxPathSum(tree) == 4);
    delete tree;

    tree = new TreeNode(new TreeNode(2), -3, new TreeNode(3));
    CHECK(s.maxPathSum(tree) == 3);
    delete tree;
}
#endif
