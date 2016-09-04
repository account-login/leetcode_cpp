#include <iostream>
#include <unordered_set>
#include <queue>
#include <vector>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/dungeon-game/


using namespace std;


/// \brief Position
struct Pos {
    int x;
    int y;
    Pos(int x, int y) : x(x), y(y) {}
    Pos() : x(0), y(0) {}
};

bool operator == (const Pos &p1, const Pos &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

namespace std {
    template<>
    struct hash<Pos> {
        size_t operator() (const Pos &pos) const {
            return (pos.x + pos.y) ^ (pos.y << 11);
        }
    };
}


/// \brief The Path struct
struct Path {
    Pos pos;                    ///< current position
    int hp;                     ///< current hp
    int start_hp;               ///< minimum hp to start
    unordered_set<Pos> nodes;   ///< visited position

    Path(Pos pos, int start_hp, int hp = 1) {
        this->pos = pos;
        this->start_hp = start_hp;
        this->hp = hp;
        this->nodes.insert(pos);
    }
};

bool operator < (const Path &p1, const Path &p2) {
    return (p1.start_hp < p2.start_hp);
}


/// \brief The Solution class
class Solution {
public:
    int calculateMinimumHP(const vector<vector<int>> &dungeon) {
        int xlen = dungeon.size();
        int ylen = dungeon[0].size();

        auto cmp = [](const Path &p1, const Path &p2) -> bool {
            // shortest path on top
            return !(p1 < p2);
        };
        using queue_t = priority_queue<Path, vector<Path>, decltype(cmp)>;
        queue_t q(cmp, { Path(Pos(0, 0), 1 - dungeon[0][0]) });

        while (true) {
            Path path = q.top();
            q.pop();

            const Pos &cur = path.pos;
            // exit
            if (cur == Pos(xlen - 1, ylen - 1)) {
                return path.start_hp;
            }

            for (const Pos &neighbor : {
                    Pos(cur.x - 1, cur.y),
                    Pos(cur.x + 1, cur.y),
                    Pos(cur.x, cur.y - 1),
                    Pos(cur.x, cur.y + 1)
                }
            ) {
                int x = neighbor.x;
                int y = neighbor.y;

                if (0 <= x && x < xlen
                    && 0 <= y && y < ylen
                    && path.nodes.count(neighbor) == 0)
                {
                    Path new_path = path;
                    new_path.pos = neighbor;
                    new_path.nodes.insert(neighbor);

                    new_path.hp += dungeon[x][y];
                    if (new_path.hp <= 0) {
                        new_path.start_hp += 1 - new_path.hp;
                        new_path.hp = 1;
                    }

                    q.push(new_path);
                }
            }
        }

        assert(0);
        return 0;
    }
};


#ifdef RUN_TEST
TEST_CASE("174. Dungeon Game") {
    Solution s;

    vector<vector<int>> board = {
        { -2, -3, 3 },
        { -5, -10, 1 },
        { 10, 30, -5 }
    };
    CHECK(s.calculateMinimumHP(board) == 7);

    board = {
        {  0,  0,  0,  0,  0,  0, -1, -1 },
        { -1, -1, -1, -1, -1,  0, -1, -1 },
        { -1, -1, -1, -1, -1,  0, -1, -1 },
        { -1,  0,  0,  0,  0,  0, -1, -1 },
        { -1,  0, -1, -1, -1, -1, -1, -1 },
        { -1,  0, -1, -1, -1, -1, -1, -1 },
        { -1,  0, -1, -1, -1, -1, -1, -1 },
        { -1,  0,  0,  0,  0,  0,  0, -2 }
    };
    CHECK(s.calculateMinimumHP(board) == 3);

    board = {
        { 0 }
    };
    CHECK(s.calculateMinimumHP(board) == 1);

    board = {
        { 0, -1, 5 }
    };
    CHECK(s.calculateMinimumHP(board) == 2);

    board = {
        { 0,  0,  1 },
        { 0, -1,  0 },
        { 0,  0, -1 }
    };
    CHECK(s.calculateMinimumHP(board) == 1);

    board = {
        { 0,  0,  0 },
        { 0, -1,  0 },
        { 1,  0, -1 }
    };
    CHECK(s.calculateMinimumHP(board) == 1);

    board = {
        {  0,  0, -1,  5 },
        {  0, -1, -1,  0 },
        {  0, -1, -1,  0 },
        {  0,  0,  0, -3 }
    };
    CHECK(s.calculateMinimumHP(board) == 2);

    board = {
        {  0,  0,  0,  0 },
        {  0, -1, -1,  0 },
        { -1, -1, -1,  0 },
        {  5,  0,  0, -3 }
    };
    CHECK(s.calculateMinimumHP(board) == 2);
}
#endif
