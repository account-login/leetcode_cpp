#include <algorithm>
#include <vector>

#ifdef RUN_TEST
#   include "catch.hpp"
#include "stringify.h"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/dungeon-game/


using namespace std;


class Solution {
public:
    int calculateMinimumHP(vector<vector<int>> dungeon) {
        const int xlen = static_cast<int>(dungeon.size());
        const int ylen = static_cast<int>(dungeon[0].size());

        dungeon[xlen - 1][ylen - 1] = max(1, 1 - dungeon[xlen - 1][ylen - 1]);
        for (int x = xlen - 1; x > 0; --x) {
            dungeon[x - 1][ylen - 1] = max(1, dungeon[x][ylen - 1] - dungeon[x - 1][ylen - 1]);
        }
        for (int y = ylen - 1; y > 0; --y) {
            dungeon[xlen - 1][y - 1] = max(1, dungeon[xlen - 1][y] - dungeon[xlen - 1][y - 1]);
        }

        for (int z = (xlen - 1) + (ylen - 1) - 1; z >= 0; --z) {
            int x0 = max(0, z - ((ylen - 1) - 1));
            int xm = min((xlen - 1) - 1, z);
            for (int x = x0; x <= xm; ++x) {
                int y = z - x;
                // assert(0 <= x && x < xlen - 1);
                // assert(0 <= y && y < ylen - 1);
                int cur = dungeon[x][y];
                dungeon[x][y] = max(
                    1, min(dungeon[x + 1][y] - cur, dungeon[x][y + 1] - cur)
                );
            }
        }

        return dungeon[0][0];
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
    CHECK(s.calculateMinimumHP(board) == 5);

    board = {
        { 0 }
    };
    CHECK(s.calculateMinimumHP(board) == 1);

    board = {
        { 5 }
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

    board = {
        {  0, -74, -47, -20, -23, -39, -48},
        { 37, -30,  37, -65, -82,  28, -27},
        {-76, -33,   7,  42,   3,  49, -93},
        { 37, -41,  35, -16, -96, -56,  38},
        {-52,  19, -37,  14, -65, -42,   9},
        {  5, -26, -30, -65,  11,   5,  16},
        {-60,   9,  36, -36,  41, -47, -86},
        {-22,  19,  -5, -41,  -8, -96, -95}
    };
    CHECK(s.calculateMinimumHP(board) == 30);
}
#endif
