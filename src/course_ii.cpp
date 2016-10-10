#include <vector>
#include <algorithm>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/course-schedule-ii/


using namespace std;


class Solution {
public:
    vector<int> findOrder(int numCourses, const vector<pair<int, int>> &prerequisites) {
        // course -> prereq
        vector<vector<int>> graph(numCourses);
        for (const auto &p : prerequisites) {
            int course = p.first;
            int prereq = p.second;
            graph[course].push_back(prereq);
        }

        vector<int> ans;
        vector<char> visited(numCourses, false);
        bool found;
        do {
            found = false;
            for (int course = 0; course < numCourses; course++) {
                if (!visited[course]) {
                    const auto &prereqs = graph[course];
                    if (all_of(prereqs.begin(), prereqs.end(),
                        [&](int i) { return visited[i]; }))
                    {
                        found = true;
                        ans.push_back(course);
                        visited[course] = true;
                    }
                }
            }
        } while (found);

        if (ans.size() == numCourses) {
            return ans;
        } else {
            return {};
        }
    }

    bool verify(int num, const vector<pair<int, int>> &prerequisites, const vector<int> &orders) {
        // course -> prereq
        vector<vector<int>> graph(num);
        for (const auto &p : prerequisites) {
            int course = p.first;
            int prereq = p.second;
            graph[course].push_back(prereq);
        }

        for (auto it = orders.begin(); it != orders.end(); it++) {
            int course = *it;
            const auto &prereqs = graph[course];
            if (!all_of(prereqs.begin(), prereqs.end(),
                [&](int p) { return count(orders.begin(), it, p) == 1; } ))
            {
                return false;
            }
        }

        return true;
    }
};


#ifdef RUN_TEST
TEST_CASE("210. Course Schedule II") {
    Solution s;

    vector<int> orders = s.findOrder(0, {});
    CHECK(s.verify(0, {}, orders));

    orders = s.findOrder(1, {});
    CAPTURE(orders);
    CHECK(s.verify(1, {}, orders));

    orders = s.findOrder(2, { {1, 0} });
    CAPTURE(orders);
    CHECK(s.verify(2, { {1, 0} }, orders));


    orders = s.findOrder(4, { {1,0}, {2,0}, {3,1}, {3,2} });
    CAPTURE(orders);
    CHECK(s.verify(4, { {1,0}, {2,0}, {3,1}, {3,2} }, orders));
}
#endif
