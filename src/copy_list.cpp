#include <vector>
#include <string>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/copy-list-with-random-pointer/


using namespace std;


#ifdef RUN_TEST
// Definition for singly-linked list with a random pointer.
struct RandomListNode {
    int label;
    RandomListNode *next, *random;
    RandomListNode(int x)
        : label(x), next(nullptr), random(nullptr)
    {}

    ~RandomListNode() {
        delete this->next;
    }

    bool value_eq(const RandomListNode *other) const {
        auto n1 = this, n2 = other;
        for (; n1 && n2; n1 = n1->next, n2 = n2->next)
        {
            if (n1->label != n2->label) {
                return false;
            }
            if ((n1->random || n2->random) && !(n1->random && n2->random)) {
                // null and none null
                return false;
            }
            if (n1->random && n1->random->label != n2->random->label) {
                return false;
            }
        }

        return !n1 && !n2;  // n1 = n2 = nullptr
    }

    string repr() const {
        string link_line = "L";
        string random_line = "R";
        for (auto node = this; node != nullptr; node = node->next) {
            link_line += " -> " + to_string(node->label);
            random_line += "   ^";
            if (node->random != nullptr) {
                random_line += to_string(node->random->label);
            }
            int align = link_line.size() - random_line.size();
            if (align > 0) {
                random_line += string(' ', align);
            } else if (align < 0) {
                link_line += string(' ', -align);
            }
        }

        return link_line + "\n" + random_line + "\n";
    }

    static RandomListNode *from_links(const vector<int> &links) {
        vector<RandomListNode *> ptrs;
        for (int i = 0; i < links.size(); i++) {
            auto node = new RandomListNode(i);
            if (!ptrs.empty()) {
                ptrs.back()->next = node;
            }
            ptrs.push_back(node);
        }

        for (int i = 0; i < links.size(); i++) {
            int link = links[i];
            assert(link < links.size() && link >= 0);
            ptrs[i]->random = ptrs[link];
        }

        if (ptrs.empty()) {
            return nullptr;
        } else {
            return ptrs[0];
        }
    }
};

namespace std {
    std::ostream &operator << ( std::ostream &os, const RandomListNode *value ) {
        os << value->repr();
        return os;
    }
}

namespace Catch {
    template<> struct StringMaker<RandomListNode *> {
        static std::string convert( RandomListNode *value ) {
            return value->repr();
        }
    };
}
#endif


class Solution {
public:
    RandomListNode *copyRandomList(RandomListNode *head) {
        RandomListNode dock = RandomListNode(0);
        RandomListNode *tail = &dock;

        for (auto node = head; node != nullptr; node = node->next) {
            auto new_node = new RandomListNode(node->label);
            new_node->next = node->random;  // save random pointer to next pointer of new node
            node->random = new_node;        // dock new node to random pointer of old node
        }

        for (auto node = head; node != nullptr; node = node->next) {
            auto new_node = node->random;
            new_node->random = new_node->next;
            // correct old random pointer to new node
            if (new_node->random != nullptr) {
                new_node->random = new_node->random->random;
            }
        }

        for (auto node = head; node != nullptr; node = node->next) {
            auto new_node = node->random;
            node->random = new_node->next;  // restore old random pointer
            // grow new list
            new_node->next = nullptr;
            tail->next = new_node;
            tail = tail->next;
        }

        // return new list
        auto ans = dock.next;
        dock.next = nullptr;
        return ans;
    }
};


#ifdef RUN_TEST
TEST_CASE("138. Copy List with Random Pointer") {
    Solution s;

    auto list = RandomListNode::from_links({ 3, 2, 1, 0 });
    auto copy = s.copyRandomList(list);
    CHECK(list->value_eq(copy));
    delete list;
    delete copy;

    list = RandomListNode::from_links({ 0, 1, 2, 3 });
    copy = s.copyRandomList(list);
    CHECK(list->value_eq(copy));
    delete list;
    delete copy;

    list = RandomListNode::from_links({});
    copy = s.copyRandomList(list);
    CHECK(list->value_eq(copy));
    delete list;
    delete copy;
}
#endif
