#include <iostream>
#include <cmath>
#include <cassert>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

// https://leetcode.com/problems/max-points-on-a-line/


using namespace std;


struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int a, int b) : x(a), y(b) {}
};

bool operator != (const Point &p1, const Point &p2) {
    return !( p1.x == p2.x && p1.y == p2.y );
}


inline bool fcmp(double f1, double f2) {
    if (f1 == 0.0) {
        return f2 == 0.0;
    } else {
        double r = fabs(f2 / f1 - 1.0d);
        return r <= 1e-5;
    }
}

bool same_line(const Point &p1, const Point &p2, const Point &p3) {
    double dx1 = p1.x - p2.x;
    double dy1 = p1.y - p2.y;
    double dx2 = p2.x - p3.x;
    double dy2 = p2.y - p3.y;

    if (dx1 == 0 && dy1 == 0) {
        return true;
    }
    if (dx2 == 0 && dy2 == 0) {
        return true;
    }

    if (dx1 == 0) {
        return dx2 == 0;
    } else {
        if (dx2 == 0) {
            return false;
        } else {
            return fcmp(dy1 / dx1, dy2 / dx2);
        }
    }
    if (dy1 == 0) {
        return dy2 == 0;
    } else {
        if (dy2 == 0) {
            return false;
        } else {
            return fcmp(dx1 / dy1, dx2 / dy2);
        }
    }
}


class Line {
public:
    const Point *p1;
    const Point *p2;
    int count;

    Line(const Point &p1) {
        this->count = 1;
        this->p1 = &p1;
        this->p2 = nullptr;
    }

    Line(const Point &p1, const Point &p2) {
        this->count = 2;
        this->p1 = &p1;
        this->p2 = &p2;
    }

    bool contains(const Point &p) const {
        if (this->p2 == nullptr) {
            return true;
        } else {
            return same_line(*(this->p1), *(this->p2), p);
        }
    }

    void add(const Point &p) {
        assert(this->contains(p));

        if (this->p2 == nullptr) {
            if (*(this->p1) != p) {
                this->p2 = &p;
            }
        }

        this->count++;
    }
};


class Solution {
public:
    int maxPoints(vector<Point>& points) {
        if (points.size() <= 2) {
            return points.size();
        }

        vector<Line> lines;
        lines.push_back(Line(points[0]));

        for (auto &&it = points.begin() + 1; it != points.end(); it++) {
            auto &point = *it;
            size_t line_end = lines.size();
            for (size_t i = 0; i < line_end; i++) {
                if (lines[i].contains(point)) {
                    if (lines[i].p2 == nullptr) {
                        // copy single point line
                        lines.push_back(lines[i]);
                    }
                    lines[i].add(point);
                }
            }

            lines.push_back(Line(point));
        }

        int ans = 0;
        for (auto &line : lines) {
            if (line.count > ans) {
                ans = line.count;
            }
        }
        return ans;
    }
};


TEST_CASE( "Same line" ) {
    CHECK(same_line(Point(0, 0), Point(0, 0), Point(0, 0)) == true);

    CHECK(same_line(Point(0, 0), Point(0, 0), Point(1, 0)) == true);
    CHECK(same_line(Point(0, 0), Point(1, 0), Point(0, 0)) == true);
    CHECK(same_line(Point(1, 0), Point(0, 0), Point(0, 0)) == true);

    CHECK(same_line(Point(0, 1), Point(0, 0), Point(0, 0)) == true);
    CHECK(same_line(Point(0, 0), Point(0, 1), Point(0, 0)) == true);
    CHECK(same_line(Point(0, 0), Point(0, 0), Point(0, 1)) == true);

    CHECK(same_line(Point(2, 3), Point(4, 5), Point(8, 9)) == true);
    CHECK(same_line(Point(-8, -582), Point(-9, -651), Point(9, 591)) == true);

    CHECK(same_line(Point(0, 1), Point(0, 1), Point(0, 3)) == true);
    CHECK(same_line(Point(2, 0), Point(5, 1), Point(8, 0)) == false);
    CHECK(same_line(Point(1, 2), Point(2, 4), Point(8, 16)) == true);

    CHECK(same_line(Point(0, 1), Point(0, 0), Point(1, 0)) == false);
    CHECK(same_line(Point(2, 4), Point(2, 1), Point(6, 8)) == false);
    CHECK(same_line(Point(5, 2), Point(0, 7), Point(1, 1)) == false);
}

TEST_CASE( "Max Points on a Line" ) {
    Solution s;

    vector<Point> points = { Point(0, 1), Point(2, 3) };
    CHECK(s.maxPoints(points) == 2);

    points = { Point(1, 2), Point(2, 4), Point(0, 0), Point(1, 1) };
    CHECK(s.maxPoints(points) == 3);

    points = { Point(1, 2), Point(2, 4), Point(0, 0), Point(1, 1), Point(2, 9) };
    CHECK(s.maxPoints(points) == 3);

    points = { Point(1, 2), Point(2, 4), Point(0, 0), Point(1, 1), Point(5, 10), Point(2, 9) };
    CHECK(s.maxPoints(points) == 4);

    points = { Point(0, 1), Point(2, 3), Point(5, 8), Point(8, 31), Point(-5, 9), Point(11, 3) };
    CHECK(s.maxPoints(points) == 2);

    points = { Point(0, 1) };
    CHECK(s.maxPoints(points) == 1);

    points = { Point(0, 1), Point(0, 1), Point(0, 1), Point(0, 1) };
    CHECK(s.maxPoints(points) == 4);

    points = { Point(1, 2), Point(2, 4), Point(0, 0), Point(1, 1), Point(2, 4) };
    CHECK(s.maxPoints(points) == 4);

    points = { Point(-4, -4), Point(-8, -582), Point(-3, 3), Point(-9, -651), Point(9, 591) };
    CHECK(s.maxPoints(points) == 3);
}

