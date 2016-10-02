#ifndef LIST_NODE_H
#define LIST_NODE_H

#include <vector>

using std::vector;

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;

    ListNode(int x) : val(x), next(nullptr) {}
    ~ListNode() {
        delete this->next;
    }

    vector<int> to_vector() {
        vector<int> ans;
        for (ListNode *cur = this; cur != nullptr; cur = cur->next) {
            ans.push_back(cur->val);
        }

        return ans;
    }

    static ListNode *from_vec(const vector<int> &vec) {
        auto head = ListNode(0);
        ListNode *tail = &head;
        for (auto i : vec) {
            tail->next = new ListNode(i);
            tail = tail->next;
        }

        auto ret = head.next;
        head.next = nullptr;
        return ret;
    }
};

#endif // LIST_NODE_H
