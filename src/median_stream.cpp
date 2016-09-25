#include <iostream>
#include <map>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/find-median-from-data-stream/


using namespace std;


class MedianFinder {
public:
    // { num, count }
    map<int, int> tree;
    map<int, int>::const_iterator mid;
    int pos = 0;
    int size = 0;

    inline void add_to_tree(int num) {
        if (this->tree.count(num) == 0) {
            this->tree[num] = 1;
        } else {
            this->tree[num]++;
        }
        this->size++;
    }

    inline void pointer_back() {
        if (this->pos != 0) {
            this->pos--;
        } else {
            this->mid--;
            this->pos = this->mid->second - 1;
        }
    }

    inline void pointer_forward() {
        if (this->pos == this->mid->second - 1) {
            this->mid++;
            this->pos = 0;
        } else {
            this->pos++;
        }
    }

    // Adds a number into the data structure.
    void addNum(int num) {
        this->add_to_tree(num);

        if (this->tree.size() == 1) {
            // initialization of this->mid
            this->mid = this->tree.begin();
        } else {
            int cur = this->mid->first;
            if (num < cur) {
                if (this->size % 2 == 0) {
                    // prevous size is odd
                    this->pointer_back();
                }
            } else {    // num >= cur
                if (this->size % 2 == 1) {
                    // prevous size is even
                    this->pointer_forward();
                }
            }
        }
    }

    // Returns the median of current data stream
    double findMedian() {
        assert(this->size != 0);

        if (this->size % 2 == 0) {
            int count = this->mid->second;
            if (this->pos == count - 1) {
                auto next = this->mid;
                next++;
                return ((double)this->mid->first + next->first) / 2;
            } else {
                return this->mid->first;
            }
        } else {
            return this->mid->first;
        }
    }
};

// Your MedianFinder object will be instantiated and called as such:
// MedianFinder mf;
// mf.addNum(1);
// mf.findMedian();


#ifdef RUN_TEST
TEST_CASE("295. Find Median from Data Stream") {
    auto mf = MedianFinder();

    mf.addNum(5);
    CHECK(mf.tree[5] == 1);
    CHECK(mf.mid->first == 5);
    CHECK(mf.pos == 0);
    CHECK(mf.findMedian() == Approx(5));

    mf.addNum(5);
    CHECK(mf.tree[5] == 2);
    CHECK(mf.mid->first == 5);
    CHECK(mf.pos == 0);
    CHECK(mf.findMedian() == Approx(5));

    mf.addNum(7);
    CHECK(mf.tree[7] == 1);
    CHECK(mf.mid->first == 5);
    CHECK(mf.pos == 1);
    CHECK(mf.findMedian() == Approx(5));

    mf.addNum(9);
    CHECK(mf.mid->first == 5);
    CHECK(mf.pos == 1);
    CHECK(mf.findMedian() == Approx(6));

    mf.addNum(9);
    CHECK(mf.mid->first == 7);
    CHECK(mf.pos == 0);
    CHECK(mf.findMedian() == Approx(7));

    mf = MedianFinder();
    int N = 100000;
    for (int i = 0; i < N; i++) {
        mf.addNum(i);
    }
    CHECK(mf.findMedian() == Approx((double)N / 2 - 0.5));

    mf = MedianFinder();
    for (int i = 0; i < N; i++) {
        mf.addNum(888);
    }
    CHECK(mf.findMedian() == Approx(888));
}
#endif
