#include <vector>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/range-sum-query-mutable/


using namespace std;


static inline int heap_parent(int child) {
    return (child + 1) / 2 - 1;
}

static inline int heap_left(int parent) {
    return parent * 2 + 1;
}

static inline int heap_right(int parent) {
    return parent * 2 + 2;
}


class NumArray {
public:
    vector<int> sum_heap;
    int nums_len;
    int height;

    explicit NumArray(const vector<int> &nums)
        : nums_len(nums.size())
    {
        // 2 ^ n >= nums.size()
        height = 0;
        while ((1 << height) < nums_len) {
            height++;
        }

        int heap_len = (1 << (height + 1)) - 1;
        sum_heap.resize(heap_len, 0);

        for (int i = 0; i < nums_len; i++) {
            sum_heap[i + heap_parent(heap_len)] = nums[i];
        }

        for (int parent = heap_parent(heap_len) - 1; parent >= 0; parent--) {
            sum_heap[parent] = sum_heap[heap_left(parent)] + sum_heap[heap_right(parent)];
        }
    }

    void update(int i, int val) {
        assert(i < nums_len);

        int arr_start = heap_parent(sum_heap.size());
        int index = arr_start + i;

        int delta = val - sum_heap[index];
        if (delta != 0) {
            sum_heap[index] = val;
            index = heap_parent(index);

            while (index >= 0) {
                sum_heap[index] += delta;
                index = heap_parent(index);
            }
        }
    }

    int sumRange(int i, int j) const {
        j++;
        // extend right boundary to leverage higher node.
        if (j == nums_len) {
            j = 1 << height;
        }
        return sum(i, j, 0, 1 << height, 0);
    }

    int sum(int x, int y, int begin, int end, int index) const {
        assert(begin <= x && y <= end);

        if (x == begin && y == end) {
            return sum_heap[index];
        } else {
            int mid = (begin + end) / 2;
            int ret = 0;
            if (x < mid) {
                int ny;
                if (y > mid) {
                    ny = mid;
                } else {
                    ny = y;
                }
                ret += sum(x, ny, begin, mid, heap_left(index));
            }
            if (y > mid) {
                int nx;
                if (x < mid) {
                    nx = mid;
                } else {
                    nx = x;
                }
                ret += sum(nx, y, mid, end, heap_right(index));
            }
            return ret;
        }
    }
};


// Your NumArray object will be instantiated and called as such:
// NumArray numArray(nums);
// numArray.sumRange(0, 1);
// numArray.update(1, 10);
// numArray.sumRange(1, 2);


#ifdef RUN_TEST
TEST_CASE("Test heap") {
    auto na = NumArray({ 1 });
    vector<int> heap = { 1 };
    CHECK(na.sum_heap == heap);

    na = NumArray({});
    heap = { 0 };
    CHECK(na.sum_heap == heap);

    na = NumArray({ 1, 2 });
    heap = { 3, 1, 2 };
    CHECK(na.sum_heap == heap);

    na = NumArray({ 1, 2, 3 });
    heap = { 6, 3, 3, 1, 2, 3, 0 };
    CHECK(na.sum_heap == heap);
}

TEST_CASE("Test update") {
    auto na = NumArray({ 1 });
    vector<int> heap = { 2 };
    na.update(0, 2);
    CHECK(na.sum_heap == heap);

    na = NumArray({ 1, 2, 3 });
    na.update(0, 4);
    heap = { 9, 6, 3, 4, 2, 3, 0 };
    CHECK(na.sum_heap == heap);
}

TEST_CASE("Test sum") {
    auto na = NumArray({ 1 });
    CHECK(na.sumRange(0, 0) == 1);

    na = NumArray({ 1, 2 });
    CHECK(na.sumRange(0, 0) == 1);
    CHECK(na.sumRange(1, 1) == 2);
    CHECK(na.sumRange(0, 1) == 3);

    na = NumArray({ 1, 2, 3 });
    CHECK(na.sumRange(0, 0) == 1);
    CHECK(na.sumRange(2, 2) == 3);
    CHECK(na.sumRange(0, 2) == 6);
    CHECK(na.sumRange(0, 1) == 3);
    CHECK(na.sumRange(1, 2) == 5);
}

TEST_CASE("307. Range Sum Query - Mutable") {
    auto na = NumArray({ 1, 2, 3 });
    CHECK(na.sumRange(1, 2) == 5);

    na.update(0, 8);
    CHECK(na.sumRange(1, 2) == 5);

    na.update(2, 5);
    CHECK(na.sumRange(1, 2) == 7);
    CHECK(na.sumRange(0, 2) == 15);
}
#endif
