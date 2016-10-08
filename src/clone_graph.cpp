#include <vector>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/clone-graph/


using namespace std;


#ifdef RUN_TEST
// Definition for undirected graph.
struct UndirectedGraphNode {
    int label;
    vector<UndirectedGraphNode *> neighbors;
    UndirectedGraphNode(int x) : label(x) {}
};
#endif


class Solution {
public:
    UndirectedGraphNode *cloneGraph(UndirectedGraphNode *node) {
        if (node == nullptr) {
            return nullptr;
        }

        unordered_set<UndirectedGraphNode *> visited;
        vector<UndirectedGraphNode *> visited_vec;  // track nodes in vector for faster iteration

        auto visitor = [&](UndirectedGraphNode *node) {
            visited_vec.push_back(node);
            auto new_node = new UndirectedGraphNode(node->label);
            node->neighbors.push_back(new_node);    // create link to new node
        };
        travel(node, visited, visitor);

        for (auto node : visited_vec) {
            auto new_node = node->neighbors.back();
            for (auto it = node->neighbors.begin(); it < node->neighbors.end() - 1; it++) {
                auto old_neighbor = *it;
                // convert neighbors from old node to new node
                new_node->neighbors.push_back(old_neighbor->neighbors.back());
            }
        }

        auto ans = node->neighbors.back();
        for (auto node : visited_vec) {
            node->neighbors.pop_back();     // remove link to new node
        }

        return ans;
    }

    void travel(
        UndirectedGraphNode *node,
        unordered_set<UndirectedGraphNode *> &visited,
        function<void (UndirectedGraphNode *)> callback)
    {
        if (visited.count(node) == 0) {
            visited.insert(node);
            for (auto ptr : node->neighbors) {
                travel(ptr, visited, callback);
            }
            callback(node);
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("133. Clone Graph") {
    Solution s;

    CHECK(s.cloneGraph(nullptr) == nullptr);
}
#endif
