#include <map>
#include <algorithm>
#include <utility>
#include <functional>
#include <iostream>

#ifdef RUN_TEST
#   define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// select an implementation (define ALGO_heap or ALGO_map)
#ifdef ALGO_heap
#   define ALGO heap
#elif defined(ALGO_map)
#   define ALGO map
#else
#   define ALGO_heap
#   define ALGO heap
#endif

#define __SUFFIX(a, b) a##b
#define _SUFFIX(a, b) __SUFFIX(a, b)


// https://leetcode.com/problems/find-median-from-data-stream/


using namespace std;


class MedianFinder_map {
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


class MedianFinder_heap {
public:
    vector<int> max_heap;
    vector<int> min_heap;

    // Adds a number into the data structure.
    void addNum(int num) {
        if (max_heap.size() == 0 && min_heap.size() == 0) {
            max_heap.push_back(num);
        } else if (max_heap.size() == 1 && min_heap.size() == 0) {
            min_heap.push_back(num);
            if (min_heap[0] < max_heap[0]) {
                swap(min_heap[0], max_heap[0]);
            }
        } else if (max_heap.size() == 0 && min_heap.size() == 1) {
            max_heap.push_back(num);
            if (min_heap[0] < max_heap[0]) {
                swap(min_heap[0], max_heap[0]);
            }
        } else {
            bool use_max;
            if (num > min_heap[0]) {
                // must use min_heap
                use_max = false;
            } else if (num < max_heap[0]) {
                // must use max_heap
                use_max = true;
            } else {
                // use the smaller heap
                use_max = (max_heap.size() < min_heap.size());
            }

            if (use_max) {
                max_heap.push_back(num);
                push_heap(max_heap.begin(), max_heap.end());
            } else {
                min_heap.push_back(num);
                push_heap(min_heap.begin(), min_heap.end(), greater<int>());
            }

            // balance
            if (max_heap.size() > min_heap.size() + 1) {
                pop_heap(max_heap.begin(), max_heap.end());
                int popped = max_heap.back();
                max_heap.pop_back();

                min_heap.push_back(popped);
                push_heap(min_heap.begin(), min_heap.end(), greater<int>());
            } else if (min_heap.size() > max_heap.size() + 1) {
                pop_heap(min_heap.begin(), min_heap.end(), greater<int>());
                int popped = min_heap.back();
                min_heap.pop_back();

                max_heap.push_back(popped);
                push_heap(max_heap.begin(), max_heap.end());
            }
        }
    }

    // Returns the median of current data stream
    double findMedian() {
        assert(!(max_heap.size() == 0 && min_heap.size() == 0));

        if (max_heap.size() > min_heap.size()) {
            return max_heap[0];
        } else if (max_heap.size() < min_heap.size()) {
            return min_heap[0];
        } else {    // max_heap.size() == min_heap.size()
            return (max_heap[0] + min_heap[0]) / 2.0;
        }
    }
};


typedef _SUFFIX(MedianFinder_, ALGO) MedianFinder;


// Your MedianFinder object will be instantiated and called as such:
// MedianFinder mf;
// mf.addNum(1);
// mf.findMedian();


#ifdef RUN_TEST
TEST_CASE("295. Find Median from Data Stream") {
#   ifdef ALGO_map
    auto mf = MedianFinder_map();

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

#   elif defined(ALGO_heap)
    auto mf = MedianFinder_heap();
    mf.addNum(5);
    CHECK(mf.findMedian() == Approx(5));

    mf.addNum(5);
    CHECK(mf.findMedian() == Approx(5));

    mf.addNum(7);
    CHECK(mf.findMedian() == Approx(5));

    mf.addNum(9);
    CHECK(mf.max_heap.size() == 2);
    CHECK(mf.min_heap.size() == 2);
    CHECK(mf.findMedian() == Approx(6));

    mf.addNum(9);
    CHECK(mf.findMedian() == Approx(7));

    mf.addNum(1);
    CHECK(mf.findMedian() == Approx(6));

#   else
    assert(0);
#   endif

    // common test
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
