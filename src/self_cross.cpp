#include <iostream>
#include <cstring>
#include <cstdint>
#include <limits>
#include <vector>
#include <algorithm>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/self-crossing/


using namespace std;


enum Direction {
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
};

struct Pos {
    int x = 0;
    int y = 0;
};


class Solution {
public:
    bool isSelfCrossing(const vector<int>& x) {
        int xlen = x.size();
        int box[4];
        bool outter = true;
        Pos cur;
        Pos prev;

        for (int i = 0; i < xlen ; i++) {
            int dir = i % 4;

            prev = cur;
            if (dir == UP) {
                cur.y += x[i];
            } else if (dir == LEFT) {
                cur.x -= x[i];
            } else if (dir == DOWN) {
                cur.y -= x[i];
            } else {    // dir == RIGHT
                cur.x += x[i];
            }

            if (i == 0) {
                ;
            } else if (i == 1) {
                ;
            } else if (i == 2) {
                if (cur.y >= 0) {
                    // ┌───┐
                    // │   │
                    //     │
                    outter = false;
                }
            } else if (i == 3) {
                if (outter) {
                    if (cur.x > 0) {
                        // ┌───┐
                        // │   │
                        // │
                        // └─────
                        box[LEFT] = prev.x;
                        box[RIGHT] = 0;
                        box[DOWN] = 0;
                        box[UP] = x[0];
                    } else if (cur.x == 0) {
                        outter = false;
                        box[LEFT] = prev.x;
                        box[RIGHT] = cur.x;
                        box[DOWN] = cur.y;
                        box[UP] = 0;
                    } else {    // cur.x < 0
                        // ┌───┐
                        // │   │
                        // │
                        // └──
                        outter = false;
                        box[LEFT] = prev.x;
                        box[RIGHT] = cur.x;
                        box[DOWN] = cur.y;
                        box[UP] = x[0];
                    }
                } else {    // inner
                    if (cur.x >= 0) {
                        // ┌───┐
                        // └───│─
                        //     │
                        return true;
                    } else {
                        // ┌───┐
                        // └─  │
                        //     │
                        box[RIGHT] = cur.x;
                        box[LEFT] = prev.x;
                        box[DOWN] = cur.y;
                        box[UP] = x[0];
                    }
                }
            } else {
                if (outter) {
                    if (dir == UP) {
                        // ┌───┐
                        // │   │ ║
                        // │     ║
                        // └─────╜
                        if (cur.y > box[UP]) {
                            box[DOWN] = prev.y;
                        } else {
                            outter = false;
                            if (cur.y >= box[DOWN]) {
                                box[LEFT] = box[RIGHT];
                            }
                            box[UP] = cur.y;
                            box[RIGHT] = cur.x;
                            box[DOWN] = prev.y;
                        }
                    } else if (dir == LEFT) {
                        if (cur.x < box[LEFT]) {
                            box[RIGHT] = prev.x;
                        } else {
                            outter = false;
                            if (cur.x <= box[RIGHT]) {
                                box[DOWN] = box[UP];
                            }
                            box[LEFT] = cur.x;
                            box[UP] = cur.y;
                            box[RIGHT] = prev.x;
                        }
                    } else if (dir == DOWN) {
                        if (cur.y < box[DOWN]) {
                            box[UP] = prev.y;
                        } else {
                            outter = false;
                            if (cur.y <= box[UP]) {
                                box[RIGHT] = box[LEFT];
                            }
                            box[DOWN] = cur.y;
                            box[LEFT] = cur.x;
                            box[UP] = prev.y;
                        }
                    } else {    // dir == RIGHT
                        if (cur.x > box[RIGHT]) {
                            box[LEFT] = prev.x;
                        } else {
                            outter = false;
                            if (cur.x >= box[LEFT]) {
                                box[UP] = box[DOWN];
                            }
                            box[LEFT] = prev.x;
                            box[DOWN] = cur.y;
                            box[RIGHT] = cur.x;
                        }
                    }
                } else {    // inner
                    if (dir == UP) {
                        if (cur.y >= box[UP]) {
                            return true;
                        } else {
                            box[UP] = cur.y;
                        }
                    } else if (dir == LEFT) {
                        if (cur.x <= box[LEFT]) {
                            return true;
                        } else {
                            box[LEFT] = cur.x;
                        }
                    } else if (dir == DOWN) {
                        if (cur.y <= box[DOWN]) {
                            return true;
                        } else {
                            box[DOWN] = cur.y;
                        }
                    } else {    // dir == RIGHT
                        if (cur.x >= box[RIGHT]) {
                            return true;
                        } else {
                            box[RIGHT] = cur.x;
                        }
                    }
                }
            }
        }

        return false;
    }
};


#ifdef RUN_TEST
TEST_CASE("335. Self Crossing") {
    Solution s;

    CHECK(s.isSelfCrossing({ 1, 1, 1, 1 }) == true);
    CHECK(s.isSelfCrossing({ 1, 2, 3, 4, 5 }) == false);
    CHECK(s.isSelfCrossing({ 1, 1, 2, 1, 1 }) == true);

    CHECK(s.isSelfCrossing({}) == false);
    CHECK(s.isSelfCrossing({ 1 }) == false);
    CHECK(s.isSelfCrossing({ 2, 1 }) == false);
    CHECK(s.isSelfCrossing({ 2, 1, 1 }) == false);

    CHECK(s.isSelfCrossing({ 2, 2, 1, 2 }) == true);
    CHECK(s.isSelfCrossing({ 2, 2, 1, 1 }) == false);

    CHECK(s.isSelfCrossing({ 6, 6, 5, 5, 4, 4, 3, 3, 2, 2 }) == false);
    CHECK(s.isSelfCrossing({ 6, 6, 5, 5, 4, 4, 3, 3, 2, 3 }) == true);

    CHECK(s.isSelfCrossing({ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 }) == false);

    CHECK(s.isSelfCrossing({ 1, 1, 2, 8, 8, 4, 4, 3, 3, 2, 2, 1, 1 }) == false);
    CHECK(s.isSelfCrossing({ 1, 1, 2, 8, 8, 4, 7, 5 }) == true);
    CHECK(s.isSelfCrossing({ 1, 1, 2, 8, 8, 7, 6 }) == true);
}
#endif
