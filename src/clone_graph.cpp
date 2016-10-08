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
    UndirectedGraphNode *cloneGraph(const UndirectedGraphNode *node) {
        if (node == nullptr) {
            return nullptr;
        }

        unordered_set<const UndirectedGraphNode *> visited;
        unordered_map<const UndirectedGraphNode *, UndirectedGraphNode *> old2new;

        auto visitor = [&](const UndirectedGraphNode *node) {
            auto new_node = new UndirectedGraphNode(node->label);
            old2new[node] = new_node;
        };
        travel(node, visited, visitor);

        for (auto node : visited) {
            auto new_node = old2new[node];
            for (auto old_neighbor : node->neighbors) {
                new_node->neighbors.push_back(old2new[old_neighbor]);
            }
        }

        return old2new[node];
    }

    void travel(
        const UndirectedGraphNode *node,
        unordered_set<const UndirectedGraphNode *> &visited,
        function<void (const UndirectedGraphNode *)> callback)
    {
        if (visited.count(node) == 0) {
            callback(node);
            visited.insert(node);
            for (auto ptr : node->neighbors) {
                travel(ptr, visited, callback);
            }
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("133. Clone Graph") {
    Solution s;
}
#endif
