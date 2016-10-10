#include <vector>
#include <queue>
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
        vector<int> prereq_count(numCourses, 0);
        // prereq -> course
        vector<vector<int>> nexts(numCourses);
        for (const auto &p : prerequisites) {
            int course = p.first;
            int prereq = p.second;
            prereq_count[course]++;
            nexts[prereq].push_back(course);
        }

        queue<int> q;
        for (int course = 0; course < numCourses; course++) {
            if (prereq_count[course] == 0) {
                q.push(course);
            }
        }

        vector<int> ans;
        while (!q.empty()) {
            int course = q.front();
            q.pop();
            ans.push_back(course);

            for (int next : nexts[course]) {
                prereq_count[next]--;
                assert(prereq_count[next] >= 0);
                if (prereq_count[next] == 0) {
                    q.push(next);
                }
            }
        }

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

        vector<char> visited(num, false);
        for (int course : orders) {
            const auto &prereqs = graph[course];
            if (any_of(prereqs.begin(), prereqs.end(),
                [&](int p) { return !visited[p]; } ))
            {
                return false;
            }
            visited[course] = true;
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

    // impossible to finish
    orders = s.findOrder(2, { {0, 1}, {1, 0} });
    vector<int> expected = {};
    CHECK(orders == expected);

    int N = 100000;
    vector<pair<int, int>> prereqs;
    for (int i = 1; i < N; i++) {
        prereqs.push_back({ i, i - 1 });
    }
    orders = s.findOrder(N, prereqs);
    CHECK(s.verify(N, prereqs, orders));

    prereqs.clear();
    for (int i = N - 1; i >= 1; i--) {
        prereqs.push_back({ i, i - 1 });
    }
    orders = s.findOrder(N, prereqs);
    CHECK(s.verify(N, prereqs, orders));
}
#endif
