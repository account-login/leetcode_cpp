#include <vector>
#include <numeric>
#include <utility>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/candy/


using namespace std;


class Solution {
public:
    int candy(const vector<int> &ratings) {
        auto scores = this->score_list(ratings);
        return accumulate(scores.begin(), scores.end(), 0) + scores.size();
    }

    vector<int> score_list(const vector<int> &ratings) {
        int size = ratings.size();
        if (size == 0) {
            return {};
        }

        vector<int> scores;
        scores.push_back(0);
        for (int i = 1; i < size; i++) {
            if (ratings[i] > ratings[i - 1]) {
                scores.push_back(scores.back() + 1);
            } else if (ratings[i] < ratings[i - 1]) {
                scores.push_back(scores.back() - 1);
            } else {
                scores.push_back(scores.back());
            }
        }

        int delta = 0;
        bool up = true;
        bool has_platform;
        int down_pos;
        for (int i = 0; i < size; i++) {
            scores[i] += delta;

            if (up) {
                if (i + 1 == size) {
                    break;
                }

                if (ratings[i + 1] > ratings[i]) {
                    // climb up
                } else if (ratings[i + 1] == ratings[i]) {
                    // at platform
                    delta -= scores[i];
                } else {
                    down_pos = i;
                    up = false;
                    has_platform = (i - 1 < 0 || ratings[i - 1] == ratings[i]);
                }
            } else {
                if (i + 1 < size && ratings[i + 1] < ratings[i]) {
                    // climb down
                } else {
                    // at foot or platform
                    up = true;
                    delta -= scores[i];

                    // adjust foot score to zero
                    int back_delta = -scores[i];

                    int override_begin = down_pos;
                    if (!has_platform && scores[i] > 0) {
                        override_begin++;
                    }
                    for (int j = i; j >= override_begin; j--) {
                        scores[j] += back_delta;
                    }
                }
            }
        }

        return scores;
    }
};


#ifdef RUN_TEST
TEST_CASE("135. Candy") {
    Solution s;

    vector<int> ans =  {};
    CHECK(s.score_list({}) == ans);

    ans =              { 0 };
    CHECK(s.score_list({ 5 }) == ans);

    ans =              { 0, 1 };
    CHECK(s.score_list({ 1, 3 }) == ans);

    ans =              { 1, 0 };
    CHECK(s.score_list({ 3, 1 }) == ans);

    ans =              { 0, 1, 1, 0 };
    CHECK(s.score_list({ 1, 2, 2, 0 }) == ans);

    ans =              { 0, 1, 2, 0 };
    CHECK(s.score_list({ 1, 2, 5, 4 }) == ans);

    ans =              { 0, 2, 1, 0 };
    CHECK(s.score_list({ 1, 6, 5, 4 }) == ans);

    ans =              { 0, 1, 2, 1, 0, 1 };
    CHECK(s.score_list({ 1, 5, 6, 3, 0, 3 }) == ans);

    ans =              { 0, 1, 2, 3, 4, 0, 2, 1, 0 };
    CHECK(s.score_list({ 1, 2, 3, 5, 8, 8, 8, 7, 2 }) == ans);

    ans =              { 0, 1, 2, 0, 1, 0, 4, 3, 2, 1, 0 };
    CHECK(s.score_list({ 1, 2, 3, 3, 3, 2, 2, 1, 0, -1, -2 }) == ans);

    ans =              { 2, 1, 0, 1 };
    CHECK(s.score_list({ 5, 2, 1, 6 }) == ans);

    ans =              { 2, 1, 0, 1, 0, 0 };
    CHECK(s.score_list({ 5, 2, 1, 6, 4, 4 }) == ans);

    ans =              { 0, 3, 2, 1,  0,  0, 1, 0, 0, 1, 0, 1, 0 };
    CHECK(s.score_list({ 5, 5, 4, 0, -1, -1, 8, 2, 2, 4, 4, 9, 0 }) == ans);
}
#endif
