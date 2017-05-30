#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <vector>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#   define private public
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/count-of-range-sum/


using namespace std;


enum class RBColor : uint8_t { R, B };


template<class T>
struct RBNode {
    explicit RBNode(int data) : data(data) {}

    RBColor color = RBColor::R;
    uint16_t size = 1;
    T data;
    RBNode<T> *left = nullptr;
    RBNode<T> *right = nullptr;
};


template<class T>
uint16_t rb_size(RBNode<T> *node) {
    return node != nullptr ? node->size : 0;
}


template<class T>
RBColor rb_color(RBNode<T> *node) {
    return node != nullptr ? node->color : RBColor::B;
}


template<class T>
RBNode<T> *rb_rotate_left(RBNode<T> *node) {
    RBNode<T> *top = node->right;
    node->right = top->left;
    top->left = node;
    top->size = top->left->size;
    top->left->size = 1 + rb_size(top->left->left) + rb_size(top->left->right);
    return top;
}


template<class T>
RBNode<T> *rb_rotate_right(RBNode<T> *node) {
    RBNode<T> *top = node->left;
    node->left = top->right;
    top->right = node;
    top->size = top->right->size;
    top->right->size = 1 + rb_size(top->right->left) + rb_size(top->right->right);
    return top;
}


template<class T>
RBNode<T> *rb_fix_left(RBNode<T> *node) {
    if (node->color == RBColor::B && node->left->color == RBColor::R) {
        if (rb_color(node->left->right) == RBColor::R) {
            node->left = rb_rotate_left(node->left);
        }
        if (rb_color(node->left->left) == RBColor::R) {
            node = rb_rotate_right(node);
            node->left->color = RBColor::B;
        }
    }
    return node;
}


template<class T>
RBNode<T> *rb_fix_right(RBNode<T> *node) {
    if (node->color == RBColor::B && node->right->color == RBColor::R) {
        if (rb_color(node->right->left) == RBColor::R) {
            node->right = rb_rotate_right(node->right);
        }
        if (rb_color(node->right->right) == RBColor::R) {
            node = rb_rotate_left(node);
            node->right->color = RBColor::B;
        }
    }
    return node;
}


template<class T>
RBNode<T> *rb_insert(RBNode<T> *root, RBNode<T> *node) {
    if (root == nullptr) {
        return node;
    } else if (node->data < root->data) {
        root->left = rb_insert(root->left, node);
        root->size++;
        return rb_fix_left(root);
    } else {
        root->right = rb_insert(root->right, node);
        root->size++;
        return rb_fix_right(root);
    }
}


template<class T>
uint16_t rb_count_lower(const RBNode<T> *node, T upper) {
    if (node == nullptr) {
        return 0;
    } else if (node->data > upper) {
        return rb_count_lower(node->left, upper);
    } else {
        return rb_size(node->left) + 1 + rb_count_lower(node->right, upper);
    }
}


template<class T>
uint16_t rb_count_upper(const RBNode<T> *node, T lower) {
    if (node == nullptr) {
        return 0;
    } else if (node->data < lower) {
        return rb_count_upper(node->right, lower);
    } else {
        return rb_size(node->right) + 1 + rb_count_upper(node->left, lower);
    }
}


template<class T>
uint16_t rb_count_range(RBNode<T> *node, T lower, T upper) {
    if (node == nullptr) {
        return 0;
    } else if (node->data < lower) {
        return rb_count_range(node->right, lower, upper);
    } else if (node->data > upper) {
        return rb_count_range(node->left, lower, upper);
    } else {
        return 1 + rb_count_upper(node->left, lower) + rb_count_lower(node->right, upper);
    }
}


template<class T>
void rb_delete(RBNode<T> *root) {
    if (root != nullptr) {
        rb_delete(root->left);
        rb_delete(root->right);
        delete root;
    }
}


#ifdef RUN_TEST
template<class T>
void rb_verify_color(RBNode<T> *node) {
    if (node != nullptr && node->color == RBColor::R) {
        REQUIRE(rb_color(node->left) == RBColor::B);
        REQUIRE(rb_color(node->right) == RBColor::B);
        rb_verify_color(node->left);
        rb_verify_color(node->right);
    }
}


template<class T>
int rb_verify_depth(RBNode<T> *node) {
    if (node == nullptr) {
        return 0;
    } else {
        int dl = rb_verify_depth(node->left);
        int dr = rb_verify_depth(node->left);
        REQUIRE(dl == dr);
        return dl + (node->color == RBColor::B);
    }
}


template<class T>
void rb_verify_size(RBNode<T> *node) {
    if (node != nullptr) {
        rb_verify_size(node->left);
        rb_verify_size(node->right);
        REQUIRE(node->size == 1 + rb_size(node->left) + rb_size(node->right));
    }
}


template<class T>
void rb_root_verify(RBNode<T> *root) {
    REQUIRE(rb_color(root) == RBColor::B);
    rb_verify_color(root);
    rb_verify_depth(root);
    rb_verify_size(root);
}
#endif


template<class T>
class RBTree {
public:
    ~RBTree() {
        rb_delete(this->root);
    }

    void add(T data) {
        this->root = rb_insert(this->root, new RBNode<T>(data));
        this->root->color = RBColor::B;
    }

    int count_range(T lower, T upper) {
        return rb_count_range(this->root, lower, upper);
    }

#ifdef RUN_TEST
    void verify() {
        rb_root_verify(this->root);
    }
#endif

private:
    RBNode<T> *root = nullptr;
};


class Solution_tree {
public:
    int countRangeSum(const vector<int> &nums, int lower, int upper) {
        assert(lower <= upper);

        int ans = 0;
        int64_t prefix_sum = 0;
        RBTree<int64_t> tree;

        for (int data : nums) {
            tree.add(prefix_sum);
            prefix_sum += data;
            ans += tree.count_range(prefix_sum - upper, prefix_sum - lower);
        }

        return ans;
    }
};


size_t lowbit(size_t n) {
    return n & ~(n - 1);
}


template<class T>
void bit_add(vector<T> &data, size_t index, T delta) {
    assert(index < data.size());
    while (index < data.size()) {
        data[index] += delta;
        index = (index + 1) + lowbit(index + 1) - 1;
    }
}


template<class T>
T bit_prefix_sum(vector<T> &data, size_t len) {
    assert(len <= data.size());
    T ans = 0;
    while (len > 0) {
        ans += data[len - 1];
        len -= lowbit(len);
    }
    return ans;
}


template<class T>
T bit_sum_range(vector<T> &data, size_t i, size_t j) {
    assert(i < j);
    return bit_prefix_sum(data, j) - bit_prefix_sum(data, i);
}


class Solution_bit {
public:
    int countRangeSum(const vector<int> &nums, int lower, int upper) {
        // prefix sum
        vector<int64_t> prefixies;
        prefixies.reserve(nums.size() + 1);
        prefixies.push_back(0);
        for (size_t i = 0; i < nums.size(); ++i) {
            prefixies.push_back(prefixies.back() + nums[i]);
        }

        // sorted indices of prefix sum
        vector<size_t > indices;
        indices.reserve(prefixies.size());
        for (size_t i = 0; i < prefixies.size(); ++i) {
            indices.push_back(i);
        }
        sort(indices.begin(), indices.end(),
            [&](size_t i, size_t j) -> bool { return prefixies[i] < prefixies[j]; }
        );

        // sorted prefix sum
        vector<int64_t> sorted;
        sorted.reserve(prefixies.size());
        for (size_t i = 0; i < prefixies.size(); ++i) {
            sorted.push_back(prefixies[indices[i]]);
        }

        // order in sorted prefix sum
        vector<size_t> order(prefixies.size(), 0);
        size_t *prev = nullptr;
        size_t max_order = 0;
        for (size_t *cur = indices.data(); cur < indices.data() + indices.size(); ++cur) {
            if (prev != nullptr && prefixies[*cur] > prefixies[*prev]) {
                max_order++;
            }
            prev = cur;
            order[*cur] = max_order;
        }

        vector<int64_t> bit((size_t)(max_order + 1), 0);

        int ans = 0;
        for (size_t i = 0; i < prefixies.size(); ++i) {
            int64_t lo = prefixies[i] - upper;
            int64_t hi = prefixies[i] - lower;
            auto begin = lower_bound(sorted.begin(), sorted.end(), lo);
            auto end = upper_bound(sorted.begin(), sorted.end(), hi);
            if (begin != end) {
                size_t v1 = order[indices[begin - sorted.begin()]];
                size_t v2 = order[indices[end - sorted.begin() - 1]];
                ans += bit_sum_range(bit, v1, v2 + 1);
            }
            bit_add<int64_t>(bit, order[i], 1);
        }

        return ans;
    }
};


#ifdef ALGO_tree
typedef Solution_tree Solution;
#else
typedef Solution_bit Solution;
#endif


#ifdef RUN_TEST
TEST_CASE("RBTree::add() sequence", "[tree]") {
    RBTree<int> tree;
    for (int i = 0; i < 100; i++) {
        tree.add(i);
        tree.verify();
    }
}


TEST_CASE("RBTree::add() random", "[tree]") {
    RBTree<int> tree;
    for (int i = 0; i < 100; i++) {
        tree.add(abs(rand()) % 90);
        tree.verify();
    }
}


TEST_CASE("RBTree::add() duplicated", "[tree]") {
    RBTree<int> tree;
    for (int i = 0; i < 100; i++) {
        tree.add(i / 10);
        tree.verify();
    }
}


double get_time() {
    auto tm = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> d = tm.time_since_epoch();
    return d.count();
}


TEST_CASE("327. Count of Range Sum", "[solution]") {
    Solution s;

    CHECK(s.countRangeSum({1}, 1, 1) == 1);
    CHECK(s.countRangeSum({0, 0}, 0, 0) == 3);
    CHECK(s.countRangeSum({1, 0, 0, -5}, 0, 0) == 3);
    CHECK(s.countRangeSum({1, 5}, 5, 5) == 1);
    CHECK(s.countRangeSum({-2, 5, -1}, -2, 2) == 3);
    CHECK(s.countRangeSum({2147483647, -2147483648, -1, 0}, -1, 0) == 4);
    CHECK(s.countRangeSum({-1,-3,1,1,0,0}, -2, 1) == 15);

    const int N = 1000000;
    vector<int> data;
    data.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        if (i % 2 == 0) {
            data.push_back(1);
        } else {
            data.push_back(-1);
        }
    }
    double t1 = get_time();
    int result = s.countRangeSum(data, -100, 100);
    double t2 = get_time();
    CAPTURE(t2 - t1);
    CHECK(result != 0);
}
#endif
