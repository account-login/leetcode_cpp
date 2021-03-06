#include <vector>
#include <queue>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/the-skyline-problem/


using namespace std;


struct Building {
    int left;
    int right;
    int height;

    explicit Building(const vector<int> &triplet)
        : left(triplet[0]), right(triplet[1]), height(triplet[2])
    {}

    Building(int left, int right, int height)
        : left(left), right(right), height(height)
    {}
};

bool operator < (const Building &b1, const Building &b2) {
    return b1.height < b2.height;
}


class Solution {
public:
    vector<pair<int, int>> getSkyline(const vector<vector<int>> &buildings) {
        vector<pair<int, int>> ans;
        using queue_t = priority_queue<Building>;
        queue_t q;

        for (auto it = buildings.begin(); !q.empty() || it != buildings.end(); ) {
            if (q.empty()) {    // enter building from ground
                q.emplace(*it);
                ans.push_back({ (*it)[0], (*it)[2] });
                it++;
            } else if (it != buildings.end() && (*it)[0] <= q.top().right) {
                // building.left <= top.right, overlapping with current top building.
                if ((*it)[2] > q.top().height) {
                    // building.height > top.height, higher than top building.
                    if ((*it)[0] == q.top().left) {
                        ans.pop_back(); // override previous top building.
                    }
                    ans.push_back({ (*it)[0], (*it)[2] });
                }
                q.emplace(*it);
                it++;
            } else {    // top building exited
                int down_pos = q.top().right;
                int orig_height = q.top().height;
                do {
                    q.pop();
                } while (!q.empty() && q.top().right <= down_pos);   // pop exited building

                if (q.empty()) {    // no lower buliding, ground reached.
                    ans.push_back({ down_pos, 0 });
                } else {    // lower building
                    if (q.top().height < orig_height) { // is building really lower than current?
                        ans.push_back({ down_pos, q.top().height });
                    }
                }
            }
        }

        return ans;
    }
};


#ifdef RUN_TEST
namespace Catch {
    template<> struct StringMaker<pair<int, int>> {
        static std::string convert( pair<int, int> const &value ) {
            return "( " + to_string(value.first) + ", " + to_string(value.second) + " )";
        }
    };
}

TEST_CASE("218. The Skyline Problem") {
    Solution s;

    vector<vector<int>> buildings = {
        { 2,  9, 10},
        { 3,  7, 15},
        { 5, 12, 12},
        {15, 20, 10},
        {19, 24,  8}
    };
    vector<pair<int, int>> skyline = { {2, 10}, {3, 15}, {7, 12}, {12, 0}, {15, 10}, {20, 8}, {24, 0} };
    CHECK(s.getSkyline(buildings) == skyline);

    buildings = {};
    skyline = {};
    CHECK(s.getSkyline(buildings) == skyline);

    buildings = {
        { 2,  9, 10},
        { 3,  7, 15},
        { 5, 12, 12},
        {12, 20, 10},
        {19, 24,  8}
    };
    skyline = { {2, 10}, {3, 15}, {7, 12}, {12, 10}, {20, 8}, {24, 0} };
    CHECK(s.getSkyline(buildings) == skyline);

    buildings = {
        { 2,  9, 10},
        { 3,  7, 15},
        { 4,  7, 14},
        { 5, 12, 12},
        {15, 20, 10},
        {19, 24,  8}
    };
    skyline = { {2, 10}, {3, 15}, {7, 12}, {12, 0}, {15, 10}, {20, 8}, {24, 0} };
    CHECK(s.getSkyline(buildings) == skyline);

    buildings = {
        { 2, 3, 10 },
        { 3, 5, 10 }
    };
    skyline = { {2, 10}, {5, 0} };
    CHECK(s.getSkyline(buildings) == skyline);

    buildings = {
        { 1, 2, 1 },
        { 1, 2, 2 },
        { 1, 2, 3 }
    };
    skyline = { {1, 3}, {2, 0} };
    CHECK(s.getSkyline(buildings) == skyline);
}
#endif
