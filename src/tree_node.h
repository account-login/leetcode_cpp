#ifndef VALID_BST_H
#define VALID_BST_H


/// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}

    TreeNode(TreeNode *left, int x, TreeNode *right)
        : val(x), left(left), right(right) {}

    ~TreeNode() {
        delete this->left;
        delete this->right;
    }
};


#endif // VALID_BST_H
