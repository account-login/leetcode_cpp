#include <vector>
#include <iterator>
#include <functional>
#include <random>
#include <algorithm>
#include <limits>
#include <cmath>
#include <sstream>
#include <unordered_set>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#   define private public
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/insert-delete-getrandom-o1-duplicates-allowed/


using namespace std;


struct ValueWrapper {
    int value;
    bool flag = false;

    ValueWrapper()
        : value(0), flag(false)
    {}

    ValueWrapper(int value)
        : value(value), flag(true)
    {}
};


class RandomizedCollection {

private:
    vector<ValueWrapper> ctn;
    size_t _count;
    double _max_load_factor;
    hash<int> _hash_fn;

    /**
     * @brief _insert
     * @param ctn
     * @param val
     * @return Return true if \a ctn did not already contain \a val
     */
    bool _insert(vector<ValueWrapper> &ctn, int val) {
        int ans = false;

        size_t h = this->_hash_fn(val);
        h %= ctn.size();
        while (ctn[h].flag) {
            if (ctn[h].value == val) {
                ans = true;
            }
            h++;
            h %= ctn.size();
        }

        ctn[h] = ValueWrapper(val);
        return !ans;
    }

    size_t _next_capacity(int prev) {
        assert(prev >= 0);
        if (prev == 0) {
            return 1;
        } else if (prev == 1) {
            return 4;
        } else {
            return prev * 2;
        }
    }

public:
    class iterator : public std::iterator<forward_iterator_tag, int> {

    private:
        const vector<ValueWrapper> &ctn;
        size_t index;

    public:
        iterator(const vector<ValueWrapper> &ctn, size_t index)
            : ctn(ctn), index(index)
        {}

         // Pre-increment
        iterator &operator++ () {
            assert(this->index < this->ctn.size() && "WTF");

            this->index++;
            while (this->index < this->ctn.size() && !this->ctn[this->index].flag) {
                this->index++;
            }

            if (this->index == this->ctn.size()) {
                // end()
                this->index = numeric_limits<size_t>::max();
            }

            return *this;
        }

        // Post-increment
        iterator operator++ (int) {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        // two-way comparison: v.begin() == v.cbegin() and vice versa
        bool operator == (const iterator &rhs) const {
            return &this->ctn == &rhs.ctn && this->index == rhs.index;
        }

        bool operator != (const iterator &rhs) const {
            return !(*this == rhs);
        }

        const int &operator *() const {
            assert(this->index < this->ctn.size());
            assert(this->ctn[this->index].flag);
            return this->ctn[this->index].value;
        }

        const int &operator->() const {
            return this->operator *();
        }

        friend ostream &operator << ( ostream &os, const iterator &it ) {
            os << "<Iterator for 0x" << std::hex << &it.ctn << " index=" << it.index << ">";
            return os;
        }
    };

    /** Initialize your data structure here. */
    RandomizedCollection()
        : ctn(), _count(0), _max_load_factor(0.7)
    {}

    size_t size() const {
        return this->_count;
    }

    size_t capacity() const {
        return this->ctn.size();
    }

    double load_factor() const {
        if (this->capacity() == 0) {
            assert(this->size() == 0);
            return 0;
        } else {
            return (double)this->size() / this->capacity();
        }
    }

    double max_load_factor() const {
        return this->_max_load_factor;
    }

    void max_load_factor(double val) {
        assert(val < 1.0);
        this->_max_load_factor = val;
    }

    iterator begin() const {
        if (this->size() == 0) {
            return this->end();
        }

        iterator it(this->ctn, 0);
        if (this->ctn[0].flag) {
            return it;
        } else {
            ++it;
            return it;
        }
    }

    iterator end() const {
        return iterator(this->ctn, numeric_limits<size_t>::max());
    }

    /** Inserts a value to the collection.
     *
     * Returns true if the collection did not already contain the specified element.
     */
    bool insert(int val) {
        // rehash
        if (this->capacity() == 0 || this->load_factor() > this->max_load_factor()) {
            this->rehash(this->_next_capacity(this->capacity()));
        }
        assert(this->load_factor() < this->max_load_factor());

        this->_count++;
        return this->_insert(this->ctn, val);
    }

    void rehash(size_t new_cap) {
        assert(new_cap > this->size() / this->max_load_factor());
        vector<ValueWrapper> new_ctn(new_cap);

        for (const auto &vw : this->ctn) {
            if (vw.flag) {
                this->_insert(new_ctn, vw.value);
            }
        }

        swap(this->ctn, new_ctn);
    }

    /** Removes a value from the collection.
     *
     * Returns true if the collection contained the specified element.
     */
    bool remove(int val) {
        if (this->size() == 0) {    // capacity may be zero
            return false;
        }

        bool found = false;
        size_t last_found;

        int count = 0;
        size_t h = this->_hash_fn(val);
        h %= this->capacity();
        while (this->ctn[h].flag && count < this->capacity()) {
            if (this->ctn[h].value == val) {
                last_found = h;
                found = true;
            }

            h++;
            h %= this->capacity();

            count++;
        }

        if (!found) {
            return false;
        } else {
            this->ctn[last_found].flag = false;   // remove element

            // rehash next element
            vector<int> need_rehash;
            int count = 0;
            for (h = (last_found + 1) % this->capacity();
                 this->ctn[h].flag && count < this->capacity();
                 h = (h + 1) % this->capacity())
            {
                if (this->_hash_fn(this->ctn[h].value) % this->capacity() != h){
                    need_rehash.push_back(this->ctn[h].value);
                    this->ctn[h].flag = false;
                }
                count++;
            }

            for (int value : need_rehash) {
                this->_insert(this->ctn, value);
            }

            this->_count--;
            return true;
        }
    }

    /** Get a random element from the collection. */
    int getRandom() const {
        assert(this->size() > 0);   // capacity must not be zero
        random_device rd;
        uniform_int_distribution<int> dis(0, this->capacity() - 1);

        for (int i = dis(rd); ; i = dis(rd)) {
            if (this->ctn[i].flag) {
                return this->ctn[i].value;
            }
        }
        assert(false);
    }

    unordered_multiset<int> to_multi_set() const {
        unordered_multiset<int> ans(this->begin(), this->end());
        return ans;
    }

    vector<pair<int, bool>> dump() const {
        vector<pair<int, bool>> ans;
        for (auto &vw : this->ctn) {
            ans.push_back({ vw.value, vw.flag });
        }

        return ans;
    }
};

/**
 * Your RandomizedCollection object will be instantiated and called as such:
 * RandomizedCollection obj = new RandomizedCollection();
 * bool param_1 = obj.insert(val);
 * bool param_2 = obj.remove(val);
 * int param_3 = obj.getRandom();
 */


#ifdef RUN_TEST
TEST_CASE("Test insert/remove/iterator") {
    RandomizedCollection rc;
    CHECK(rc.capacity() == 0);
    CHECK(rc.size() == 0);
    CHECK(rc.begin() == rc.end());
    unordered_multiset<int> expected = {};
    CHECK(rc.to_multi_set() == expected);

    CHECK(rc.insert(1));
    CHECK(rc.capacity() >= 1);
    CHECK(rc.size() == 1);
    CHECK(rc.begin() != rc.end());
    CHECK(++rc.begin() == rc.end());
    CHECK(*rc.begin() == 1);

    // test iterator copy & assign
    auto it = rc.begin();
    auto it_copy = RandomizedCollection::iterator(it);
    CHECK(it == it_copy);
    auto it_copy_assign = it;
    CHECK(it == it_copy_assign);

    CHECK(rc.insert(2));
    CHECK(rc.capacity() >= 2);
    CHECK(rc.size() == 2);
    CHECK(rc.begin() != rc.end());
    CHECK(++rc.begin() != rc.end());
    CHECK(++++rc.begin() == rc.end());
    expected = { 1, 2 };
    CHECK(rc.to_multi_set() == expected);

    CHECK_FALSE(rc.insert(1));
    CHECK(rc.capacity() >= 3);
    CHECK(rc.size() == 3);
    expected = { 1, 1, 2 };
    CHECK(rc.to_multi_set() == expected);

    CHECK(rc.insert(3));
    CHECK(rc.insert(4));
    CHECK(rc.size() == 5);
    expected = { 1, 1, 2, 3, 4 };
    CHECK(rc.to_multi_set() == expected);

    // test removal
    CHECK_FALSE(rc.remove(5));
    CHECK(rc.size() == 5);

    CHECK(rc.remove(3));
    CHECK(rc.size() == 4);
    expected = { 1, 1, 2, 4 };
    CHECK(rc.to_multi_set() == expected);

    CHECK(rc.remove(1));
    expected = { 1, 2, 4 };
    CHECK(rc.to_multi_set() == expected);
}

TEST_CASE("Random test insert/remove") {
    uniform_int_distribution<int> elem_gen(0, 9);
    discrete_distribution<int> action_gen({ 2, 1 });
    random_device rd;

    for (int round = 0; round < 1000; round++) {
        const int N = 100;
        RandomizedCollection rc;
        unordered_multiset<int> expected;

        for (int i = 0; i < N; i++) {
            int el = elem_gen(rd);
            int action = action_gen(rd);

            if (action == 0) {
                bool has = !rc.insert(el);
                CHECK((expected.count(el) > 0) == has);
                expected.insert(el);
            } else {
                bool has = rc.remove(el);
                CHECK((expected.count(el) > 0) == has);

                if (has) {
                    expected.erase(expected.find(el));
                }
            }
        }

        CAPTURE(round);
        CHECK(rc.to_multi_set() == expected);
    }
}

TEST_CASE("Test getRandom() distribution") {
    auto test_dist = [](
        const vector<int> &freqs,
        int insert_round = 100,
        int test_round = 10000)
    {
        RandomizedCollection rc;

        for (int round = 0; round < insert_round; round++) {
            for (int i = 0; i < freqs.size(); i++) {
                for (int j = 0; j < freqs[i]; j++) {
                    rc.insert(i);
                }
            }
        }

        vector<double> stats(freqs.size());
        for (int i = 0; i < test_round; i++) {
            stats[rc.getRandom()] += 1.0;
        }
        for (double &d : stats) {
            d /= test_round;
        }
        return stats;
    };

    auto distance = [](const vector<double> &v1, const vector<double> &v2) {
        assert(v1.size() == v2.size());
        vector<double> sub;
        for (int i = 0; i < v1.size(); i++) {
            sub.push_back(v1[i] - v2[i]);
        }
        transform(sub.begin(), sub.end(), sub.begin(), [](double i) { return i * i; });
        return sqrt(accumulate(sub.begin(), sub.end(), 0.0L));
    };

    auto normalize = [](const vector<int> &v1) {
        vector<double> ans;
        double sum = accumulate(v1.begin(), v1.end(), 0.0L);
        for (int i = 0; i < v1.size(); i++) {
            ans.push_back(v1[i] / sum);
        }
        return ans;
    };

    auto test = [&](
        const vector<int> &freqs,
        int insert_round = 100,
        int test_round = 10000)
    {
        vector<double> stats = test_dist(freqs);
        CAPTURE(stats);
        CHECK(distance(stats, normalize(freqs)) < 0.05);
    };

    test({ 1 }, 1, 100000);
    test({ 1, 1 });
    test({ 1, 2 });
    test({ 2, 1 }, 1);
    test({ 2, 1, 2, 1, 2, 1, 0, 5 });
}

TEST_CASE("381. Insert Delete GetRandom O(1) - Duplicates allowed") {
    // dummy
}
#endif
