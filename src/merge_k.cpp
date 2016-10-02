#include <queue>
#include <algorithm>
#include <functional>
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


static inline int heap_left(int i) {
    return (i * 2) + 1;
}

static inline int heap_right(int i) {
    return (i * 2) + 2;
}

static inline int heap_parent(int i) {
    return (i + 1) / 2 - 1;
}


template<typename ElemType, typename IterType, typename Comp>
void heap_decrease_top(
    IterType begin, IterType end, const ElemType &top,
    Comp less_op = less<const ElemType &>())
{
    int len = end - begin;
    assert(len > 0);

    begin[0] = top;
    for (int i = 0; ;) {
        // no left
        if (heap_left(i) >= len) {
            return;
        }

        int swap_idx = i;
        // left
        if (less_op(top, begin[heap_left(i)])) {
            swap_idx = heap_left(i);
        }
        // right
        if (heap_right(i) < len) {
            if (less_op(top, begin[heap_right(i)])
                && less_op(begin[heap_left(i)], begin[heap_right(i)]))
            {
                swap_idx = heap_right(i);
            }
        }

        // swap
        if (swap_idx != i) {
            swap(begin[i], begin[swap_idx]);
            i = swap_idx;
        } else {
            return;
        }
    }
}


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
    ListNode* mergeKLists_stl(const vector<ListNode *> &lists) {
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

    ListNode* mergeKLists(const vector<ListNode *> &lists) {
        vector<ListNode *> heap;    ///< copy of lists without nullptr
        copy_if(lists.begin(), lists.end(), back_inserter(heap), [](const ListNode *node) {
            return node != nullptr;
        });

        auto cmp = [](const ListNode *n1, const ListNode *n2) {
            return n1->val > n2->val;
        };

        ListNode head(0);
        ListNode *tail = &head;

        make_heap(heap.begin(), heap.end(), cmp);
        while (!heap.empty()) {
            tail->next = heap.front();
            tail = tail->next;

            if (tail->next) {
                assert(tail->next->val >= heap[0]->val);
                heap_decrease_top(heap.begin(), heap.end(), tail->next, cmp);
            } else {
                pop_heap(heap.begin(), heap.end(), cmp);
                heap.pop_back();
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

    lists = { ListNode::from_vec({ 1, 4 }), ListNode::from_vec({ 4, 4 }), ListNode::from_vec({ -5, 4 }) };
    sorted = s.mergeKLists(lists);
    ans = { -5, 1, 4, 4, 4, 4 };
    CHECK(sorted->to_vector() == ans);
    delete sorted;
}
#endif
