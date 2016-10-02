#include <queue>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "list_node.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/merge-k-sorted-lists/


using namespace std;


/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(nullptr) {}
 * };
 */
class Solution {
public:
    ListNode* mergeKLists(const vector<ListNode *> &lists) {
        vector<ListNode *> lists_nn;    ///< copy of lists without nullptr
        copy_if(lists.begin(), lists.end(), back_inserter(lists_nn), [](const ListNode *node) {
            return node != nullptr;
        });

        auto cmp = [](const ListNode *n1, const ListNode *n2) {
            return n1->val > n2->val;
        };

        ListNode head(0);
        ListNode *tail = &head;
        priority_queue<ListNode *, vector<ListNode *>, decltype(cmp)> q(cmp, move(lists_nn));
        while (!q.empty()) {
            tail->next = q.top();
            tail = tail->next;

            q.pop();
            if (tail->next) {
                q.push(tail->next);
            }
        }

        assert(tail->next == nullptr);
        auto ret = head.next;
        head.next = nullptr;
        return ret;
    }
};


#ifdef RUN_TEST
TEST_CASE("23. Merge k Sorted Lists") {
    Solution s;

    vector<ListNode *> lists = {};
    CHECK(s.mergeKLists({}) == nullptr);

    lists = { ListNode::from_vec({ 1, 3, 5, 7 }), ListNode::from_vec({ 2, 4, 6, 8 }) };

    ListNode *sorted = s.mergeKLists(lists);
    vector<int> ans = { 1, 2, 3, 4, 5, 6, 7, 8 };
    CHECK(sorted->to_vector() == ans);
    delete sorted;

    lists = { ListNode::from_vec({}), ListNode::from_vec({ 2, 4, 6, 8 }) };
    sorted = s.mergeKLists(lists);
    ans = { 2, 4, 6, 8 };
    CHECK(sorted->to_vector() == ans);
    delete sorted;

    lists = { ListNode::from_vec({ 1, 4 }), ListNode::from_vec({ 2, 8 }), ListNode::from_vec({ -5, 10 }) };
    sorted = s.mergeKLists(lists);
    ans = { -5, 1, 2, 4, 8, 10 };
    CHECK(sorted->to_vector() == ans);
    delete sorted;
}
#endif
