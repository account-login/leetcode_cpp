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


template<class ElemType, class FlagsType = vector<bool>>
class HashMultiSetOpenAddress {

private:
    vector<ElemType> values;
    FlagsType flags;
    size_t _count;
    double _max_load_factor;
    hash<ElemType> _hash_fn;

    /**
     * @brief _insert
     * @param ctn
     * @param val
     * @return Return true if \a ctn did not already contain \a val
     */
    bool _insert(vector<ElemType> &ctn, FlagsType &flags, const ElemType &val) {
        bool found = false;

        size_t h = this->_hash_fn(val);
        h %= ctn.size();
        while (flags[h]) {
            if (ctn[h] == val) {
                found = true;
            }
            h++;
            h %= ctn.size();
        }

        ctn[h] = val;
        flags[h] = true;
        return !found;
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
    class iterator : public std::iterator<forward_iterator_tag, ElemType> {

    private:
        const HashMultiSetOpenAddress &rc;
        size_t index;

    public:
        iterator(const HashMultiSetOpenAddress &rc, size_t index)
            : rc(rc), index(index)
        {}

         // Pre-increment
        iterator &operator++ () {
            assert(this->index < this->rc.capacity() && "WTF");

            this->index++;
            while (this->index < this->rc.capacity() && !this->rc.flags[this->index]) {
                this->index++;
            }

            if (this->index == this->rc.capacity()) {
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
            return &this->rc == &rhs.rc && this->index == rhs.index;
        }

        bool operator != (const iterator &rhs) const {
            return !(*this == rhs);
        }

        const ElemType &operator *() const {
            assert(this->index < this->rc.capacity());
            assert(this->rc.flags[this->index]);
            return this->rc.values[this->index];
        }

        const ElemType &operator->() const {
            return this->operator *();
        }

        friend ostream &operator << ( ostream &os, const iterator &it ) {
            os << "<Iterator for " << std::hex << &it.rc << " index=" << it.index << ">";
            return os;
        }
    };

    /** Initialize your data structure here. */
    HashMultiSetOpenAddress()
        : values(), flags(), _count(0), _max_load_factor(0.6)
    {}

    size_t size() const {
        return this->_count;
    }

    size_t capacity() const {
        return this->values.size();
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

        iterator it(*this, 0);
        if (this->flags[0]) {
            return it;
        } else {
            ++it;
            return it;
        }
    }

    iterator end() const {
        return iterator(*this, numeric_limits<size_t>::max());
    }

    /** Inserts a value to the collection.
     *
     * Returns true if the collection did not already contain the specified element.
     */
    bool insert(const ElemType &val) {
        // rehash
        if (this->capacity() == 0 || this->load_factor() > this->max_load_factor()) {
            this->rehash(this->_next_capacity(this->capacity()));
        }
        assert(this->load_factor() < this->max_load_factor());

        this->_count++;
        return this->_insert(this->values, this->flags, val);
    }

    void rehash(size_t new_cap) {
        assert(new_cap > this->size() / this->max_load_factor());
        vector<ElemType> new_ctn(new_cap);
        FlagsType new_flags(new_cap);

        for (int i = 0; i < this->capacity(); i++) {
            if (this->flags[i]) {
                this->_insert(new_ctn, new_flags, this->values[i]);
            }
        }

        swap(this->values, new_ctn);
        swap(this->flags, new_flags);
    }

    /** Removes a value from the collection.
     *
     * Returns true if the collection contained the specified element.
     */
    bool remove(const ElemType &val) {
        if (this->size() == 0) {    // capacity may be zero
            return false;
        }

        bool found = false;
        size_t last_found;

        int count = 0;
        size_t h = this->_hash_fn(val);
        h %= this->capacity();
        while (this->flags[h] && count < this->capacity()) {
            if (this->values[h] == val) {
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
            // remove element
            this->values[last_found] = ElemType();  // replace old element with default one
            this->flags[last_found] = false;

            // rehash next element
            int count = 0;
            for (h = (last_found + 1) % this->capacity();
                 this->flags[h] && count < this->capacity();
                 h = (h + 1) % this->capacity())
            {
                if (this->_hash_fn(this->values[h]) % this->capacity() != h) {
                    this->flags[h] = false;
                    this->_insert(this->values, this->flags, this->values[h]);
                }
                count++;
            }

            this->_count--;
            return true;
        }
    }

    /** Get a random element from the collection. */
    const ElemType &getRandom() const {
        assert(this->size() > 0);   // capacity must not be zero
        random_device rd;
        uniform_int_distribution<int> dis(0, this->capacity() - 1);

        for (int i = dis(rd); ; i = dis(rd)) {
            if (this->flags[i]) {
                return this->values[i];
            }
        }
        assert(false);
    }

#ifdef RUN_TEST
    unordered_multiset<ElemType> to_multiset() const {
        unordered_multiset<ElemType> ans(this->begin(), this->end());
        return ans;
    }

    vector<pair<ElemType, bool>> dump() const {
        vector<pair<ElemType, bool>> ans;
        for (int i = 0; i < this->capacity(); i++) {
            ans.push_back({ this->values[i], this->flags[i] });
        }

        return ans;
    }
#endif
};

typedef HashMultiSetOpenAddress<int> RandomizedCollection;

/**
 * Your RandomizedCollection object will be instantiated and called as such:
 * RandomizedCollection obj = new RandomizedCollection();
 * bool param_1 = obj.insert(val);
 * bool param_2 = obj.remove(val);
 * int param_3 = obj.getRandom();
 */


#ifdef RUN_TEST
TEST_CASE("Test on types other than int") {
    HashMultiSetOpenAddress<string> mset;
    mset.insert("asdf");
    mset.insert("343");
    mset.insert("asdf");
    mset.insert("fda");
    mset.remove("asdf");

    unordered_multiset<string> expected = {
        "asdf", "343", "fda"
    };
    CHECK(mset.to_multiset() == expected);
}

TEST_CASE("Test insert/remove/iterator") {
    RandomizedCollection rc;
    CHECK(rc.capacity() == 0);
    CHECK(rc.size() == 0);
    CHECK(rc.begin() == rc.end());
    unordered_multiset<int> expected = {};
    CHECK(rc.to_multiset() == expected);

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
    CHECK(rc.to_multiset() == expected);

    CHECK_FALSE(rc.insert(1));
    CHECK(rc.capacity() >= 3);
    CHECK(rc.size() == 3);
    expected = { 1, 1, 2 };
    CHECK(rc.to_multiset() == expected);

    CHECK(rc.insert(3));
    CHECK(rc.insert(4));
    CHECK(rc.size() == 5);
    expected = { 1, 1, 2, 3, 4 };
    CHECK(rc.to_multiset() == expected);

    // test removal
    CHECK_FALSE(rc.remove(5));
    CHECK(rc.size() == 5);

    CHECK(rc.remove(3));
    CHECK(rc.size() == 4);
    expected = { 1, 1, 2, 4 };
    CHECK(rc.to_multiset() == expected);

    CHECK(rc.remove(1));
    expected = { 1, 2, 4 };
    CHECK(rc.to_multiset() == expected);
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
        CHECK(rc.to_multiset() == expected);
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
