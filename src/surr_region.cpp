#include <iostream>
#include <queue>
#include <utility>
#include <vector>
#include <initializer_list>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/surrounded-regions/


using namespace std;


class Solution {
public:
    void solve(vector<vector<char>> &board) {
        int xlen = board.size();
        if (xlen == 0) {
            return;
        }
        int ylen = board[0].size();
        if (ylen == 0) {
            return;
        }

        for (int x = 0; x < xlen; x++) {
            for (int y = 0; y < ylen; y++) {
                if (board[x][y] == 'O') {
                    bool result = fill(board, x, y, '#');
                    if (result) {
                        fill(board, x, y, 'X');
                    }
                }
            }
        }

        for (int x = 0; x < xlen; x++) {
            for (int y = 0; y < ylen; y++) {
                if (board[x][y] == '#') {
                    fill(board, x, y, 'O');
                }
            }
        }
    }

    bool fill(vector<vector<char>> &board, int x, int y, char color) {
        int xlen = board.size();
        int ylen = board[0].size();
        char orig_color = board[x][y];

        bool ret = true;

        queue<pair<int, int>> q;
        q.push({ x, y });

        while (!q.empty()) {
            auto &xy = q.front();
            int cx = xy.first;
            int cy = xy.second;
            q.pop();

            // visited, skip
            if (board[cx][cy] == color) {
                continue;
            }
            board[cx][cy] = color;

            // edge reached
            if (cx == 0 || cx == xlen - 1
                || cy == 0 || cy == ylen - 1)
            {
                ret = false;
            }

            for (const auto &neighbor : initializer_list<pair<int, int>> {
                    { cx - 1, cy },
                    { cx + 1, cy },
                    { cx, cy - 1 },
                    { cx, cy + 1 }
                }
            ) {
                int nx = neighbor.first;
                int ny = neighbor.second;

                if (0 <= nx && nx < xlen
                    && 0 <= ny && ny < ylen)
                {
                    if (board[nx][ny] == orig_color) {
                        q.push({ nx, ny });
                    }
                }
            }
        }

        return ret;
    }
};


#ifdef RUN_TEST
TEST_CASE("130. Surrounded Regions") {
    Solution s;

    vector<vector<char>> board = {
        { 'X', 'X', 'X', 'X' },
        { 'X', 'O', 'O', 'X' },
        { 'X', 'X', 'O', 'X' },
        { 'X', 'O', 'X', 'X' },
    };
    vector<vector<char>> ans = {
        { 'X', 'X', 'X', 'X' },
        { 'X', 'X', 'X', 'X' },
        { 'X', 'X', 'X', 'X' },
        { 'X', 'O', 'X', 'X' },
    };

    s.solve(board);
    CHECK(board == ans);

    board = {
        { 'O' }
    };
    ans = {
        { 'O' }
    };

    s.solve(board);
    CHECK(board == ans);

    int N = 300;
    board = {};
    for (int i = 0; i < N; i++) {
        board.emplace_back(N, 'O');
    }
    for (int i = 1; i < N - 1; i++) {
        board[1][i] = 'X';
        board[N - 2][i] = 'X';
        board[i][1] = 'X';
        board[i][N - 2] = 'X';
    }

    ans = {};
    for (int i = 0; i < N; i++) {
        ans.emplace_back(N, 'X');
    }
    for (int i = 0; i < N; i++) {
        ans[i][0] = 'O';
        ans[i][N - 1] = 'O';
        ans[0][i] = 'O';
        ans[N - 1][i] = 'O';
    }

    s.solve(board);
    CHECK(board == ans);
}
#endif
