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


class NumArray_heap {
public:
    vector<int> sum_heap;
    int nums_len;
    int height;

    explicit NumArray_heap(const vector<int> &nums)
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


int bit_parent(int index) {
    return index | (index + 1);
}


void bit_add(vector<int> &bit, int index, int value) {
    assert(0 <= index && index < bit.size());
    for (; index < bit.size(); index = bit_parent(index)) {
        bit[index] += value;
    }
}


int bit_prev(int index) {
    return (index & (index + 1)) - 1;
}


int bit_sum_prefix(const vector<int> &bit, int prefixlen) {
    assert(0 <= prefixlen && prefixlen <= bit.size());
    int ans = 0;
    for (int index = prefixlen - 1; index >= 0; index = bit_prev(index)) {
        ans += bit[index];
    }
    return ans;
}


int bit_sum(const vector<int> &bit, int start, int stop) {
    assert(0 <= start && start <= stop);
    // return bit_sum_prefix(bit, stop) - bit_sum_prefix(bit, start);
    int ans = 0;
    int long_idx;
    for (long_idx = stop - 1; long_idx > start - 1; long_idx = bit_prev(long_idx)) {
        ans += bit[long_idx];
    }
    for (int short_idx = start - 1; short_idx > long_idx; short_idx = bit_prev(short_idx)) {
        ans -= bit[short_idx];
    }
    return ans;
}


int bit_get(const vector<int> &bit, int index) {
    assert(0 <= index && index < bit.size());
    return bit_sum(bit, index, index + 1);
}


void bit_update(vector<int> &bit, int index, int value) {
    int diff = value - bit_get(bit, index);
    bit_add(bit, index, diff);
}


class NumArray_bit {
private:
    vector<int> bit;

public:
    explicit NumArray_bit(const vector<int> &nums) {
        this->bit.resize(nums.size(), 0);
        for (size_t i = 0; i < nums.size(); i++) {
            bit_add(this->bit, i, nums[i]);
        }
    }

    void update(int i, int val) {
        bit_update(this->bit, i, val);
    }

    int sumRange(int i, int j) {
        return bit_sum(this->bit, i, j + 1);
    }
};


#ifdef ALGO_heap
typedef NumArray_heap NumArray;
#else
typedef NumArray_bit NumArray;
#endif


// Your NumArray object will be instantiated and called as such:
// NumArray numArray(nums);
// numArray.sumRange(0, 1);
// numArray.update(1, 10);
// numArray.sumRange(1, 2);


#ifdef RUN_TEST
TEST_CASE("Test heap", "[heap]") {
    auto na = NumArray_heap({ 1 });
    vector<int> heap = { 1 };
    CHECK(na.sum_heap == heap);

    na = NumArray_heap({});
    heap = { 0 };
    CHECK(na.sum_heap == heap);

    na = NumArray_heap({ 1, 2 });
    heap = { 3, 1, 2 };
    CHECK(na.sum_heap == heap);

    na = NumArray_heap({ 1, 2, 3 });
    heap = { 6, 3, 3, 1, 2, 3, 0 };
    CHECK(na.sum_heap == heap);
}

TEST_CASE("Test update", "[heap]") {
    auto na = NumArray_heap({ 1 });
    vector<int> heap = { 2 };
    na.update(0, 2);
    CHECK(na.sum_heap == heap);

    na = NumArray_heap({ 1, 2, 3 });
    na.update(0, 4);
    heap = { 9, 6, 3, 4, 2, 3, 0 };
    CHECK(na.sum_heap == heap);
}

void check_bit_prefix_sum(const vector<int> &bit, const vector<int> &arr) {
    int sum = 0;
    for (size_t i = 0; i < arr.size(); i++) {
        CHECK(bit_sum_prefix(bit, i) == sum);
        sum += arr[i];
    }
    CHECK(bit_sum_prefix(bit, arr.size()) == sum);
}

void check_bit_sum(const vector<int> &bit, const vector<int> &arr) {
    vector<int> prefix_sum = arr;
    for (size_t i = 1; i < arr.size(); i++) {
        prefix_sum[i] += prefix_sum[i - 1];
    }

    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = i; j < arr.size(); j++) {
            CHECK(bit_sum(bit, i + 1, j + 1) == prefix_sum[j] - prefix_sum[i]);
        }
    }
}

TEST_CASE("Test BIT", "[bit]") {
    vector<int> test_data = {1, 10, 100, 1000, 10000, 100000};

    vector<int> bit(test_data.size(), 0);
    for (size_t i = 0; i < test_data.size(); i++) {
        bit_add(bit, i, test_data[i]);
        CHECK(bit_get(bit, i) == test_data[i]);
    }

    check_bit_prefix_sum(bit, test_data);

    vector<int> test_data_add = {3, 33, 333, 333, 3333, 33333};
    for (size_t i = 0; i < test_data_add.size(); i++) {
        test_data[i] += test_data_add[i];
        bit_add(bit, i, test_data_add[i]);

        CHECK(bit_get(bit, i) == test_data[i]);
        check_bit_prefix_sum(bit, test_data);
        check_bit_sum(bit, test_data);
    }
}

TEST_CASE("Test sum", "[sol]") {
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

TEST_CASE("307. Range Sum Query - Mutable", "[sol]") {
    auto na = NumArray({ 1, 2, 3 });
    CHECK(na.sumRange(1, 2) == 5);

    na.update(0, 8);
    CHECK(na.sumRange(1, 2) == 5);

    na.update(2, 5);
    CHECK(na.sumRange(1, 2) == 7);
    CHECK(na.sumRange(0, 2) == 15);
}
#endif
