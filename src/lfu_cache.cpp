#include <unordered_map>
#include <list>
#include <tuple>
#include <cstdint>
#include <algorithm>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#   define private public
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/lfu-cache/


using namespace std;


// BEGIN LFUCache_heap
struct HeapItem {
    uint32_t serial;
    int key;
    uint32_t freq;

    HeapItem(int serial, int key)
        : serial(serial), key(key), freq(0)
    {}
};

bool operator < (const HeapItem &lhs, const HeapItem &rhs) {
    return tie(lhs.freq, lhs.serial) < tie(rhs.freq, rhs.serial);
}

bool operator > (const HeapItem &lhs, const HeapItem &rhs) {
    return tie(lhs.freq, lhs.serial) > tie(rhs.freq, rhs.serial);
}

void swap(HeapItem &lhs, HeapItem &rhs) {
    swap(lhs.freq, rhs.freq);
    swap(lhs.serial, rhs.serial);
    swap(lhs.key, rhs.key);
}


struct MapValue {
    size_t heapidx;
    int value;

    MapValue(size_t heapidx, int value)
        : heapidx(heapidx), value(value)
    {}
};


int heap_left(int parent) {
    return parent * 2 + 1;
}

int heap_right(int parent) {
    return parent * 2 + 2;
}

int heap_parent(int child) {
    return (child + 1) / 2 - 1;
}


class LFUCache_heap {
private:
    size_t capacity;
    uint32_t serial;
    unordered_map<int, MapValue> map;
    vector<HeapItem> heap;  // min heap

    size_t heap_push() {
        assert(this->heap.size() > 0);

        size_t index = this->heap.size() - 1;
        HeapItem item = this->heap[index];
        int parent = heap_parent(index);
        while (parent >= 0 && item < this->heap[parent]) {
            this->map.find(this->heap[parent].key)->second.heapidx = index;
            this->heap[index] = this->heap[parent];
            index = parent;
            parent = heap_parent(index);
        }

        this->heap[index] = item;
        assert(is_heap(this->heap.begin(), this->heap.end(), greater<HeapItem>()));
        return index;
    }

    void evict() {
        assert(this->heap.size() > 0);

        this->map.erase(this->heap.front().key);
        if (this->heap.size() > 1) {
            this->heap.front() = this->heap.back();
            this->heap_increased(0, this->heap.size() - 1);
        }
        this->heap.pop_back();
        assert(is_heap(this->heap.begin(), this->heap.end(), greater<HeapItem>()));
    }

    void heap_increased(size_t index, size_t end) {
        assert(0 < end && end <= this->heap.size());
        assert(index < end);

        this->heap.push_back(this->heap[index]);
        while (true) {
            size_t minimum = this->heap.size() - 1;
            for (size_t child : { heap_right(index), heap_left(index) }) {
                if (child < end && this->heap[child] < this->heap[minimum]) {
                    minimum = child;
                }
            }

            if (minimum != this->heap.size() - 1) {
                this->map.find(this->heap[minimum].key)->second.heapidx = index;
                this->heap[index] = this->heap[minimum];
                index = minimum;
            } else {
                this->heap[index] = this->heap.back();
                this->map.find(this->heap[index].key)->second.heapidx = index;
                this->heap.pop_back();
                assert(is_heap(this->heap.begin(), this->heap.begin() + end, greater<HeapItem>()));
                return;
            }
        }
    }

    void accessed(size_t heapidx) {
        this->heap[heapidx].freq++;
        this->heap[heapidx].serial = this->get_serial();
        this->heap_increased(heapidx, this->heap.size());
    }

    uint32_t get_serial() {
        return this->serial++;
    }

public:
    explicit LFUCache_heap(int capacity)
        : capacity(capacity), serial(0)
    {
        assert(capacity >= 0);
    }

    int get(int key) {
        auto it = this->map.find(key);
        if (it == this->map.end()) {
            return -1;
        } else {
            this->accessed(it->second.heapidx);
            return it->second.value;
        }
    }

    int peek(int key) const {
        auto it = this->map.find(key);
        if (it == this->map.end()) {
            return -1;
        } else {
            return it->second.value;
        }
    }

    // set is O(log n) for key not in cache since it has smallest frequency.
    void set(int key, int value) {
        if (this->capacity == 0) {
            return;
        }

        auto it = this->map.find(key);
        if (it == this->map.end()) {
            // evict before insertion
            if (this->heap.size() == this->capacity) {
                this->evict();
            }
            // push the key to heap
            this->heap.emplace_back(this->get_serial(), key);
            // and get the index of key in heap
            size_t heapidx = this->heap_push();
            // then insert key to map with index
            this->map.insert({ key, MapValue(heapidx, value) });
        } else {
            // key already exists
            it->second.value = value;
            this->accessed(it->second.heapidx);
        }
    }

    size_t size() const {
        return this->heap.size();
    }
};
// END LFUCache_heap


// BEGIN LFUCache_list
template<class KeyType, class ValueType, ValueType DefaultValue = (ValueType() - 1)>
class LFUCache_list {
private:
    struct FreqListNode {
        uint32_t freq;
        list<KeyType> keys;

        FreqListNode(uint32_t freq)
            : freq(freq)
        {}
    };
    typedef typename list<KeyType>::iterator KeyIter;

    size_t capacity;
    list<FreqListNode> freqs;
    typedef typename list<FreqListNode>::iterator FreqIter;

    struct WrappedValue {
        ValueType value;
        FreqIter freq_iter;
        KeyIter key_iter;

        WrappedValue(const ValueType &value, const FreqIter &freq_iter, const KeyIter &key_iter)
            : value(value), freq_iter(freq_iter), key_iter(key_iter)
        {}
    };

    unordered_map<KeyType, WrappedValue> map;
    typedef typename unordered_map<KeyType, WrappedValue>::iterator MapIter;

    void accessed(WrappedValue &wv) {
        const FreqIter &fi = wv.freq_iter;
        const KeyIter &ki = wv.key_iter;

        // get next frequency list
        FreqIter next_fi = this->next_freq(fi);
        assert(next_fi->freq == fi->freq + 1);
        // move key to the front of next frequency list
        next_fi->keys.emplace_front(*ki);
        fi->keys.erase(ki);
        // remove empty freqency list
        if (fi->keys.size() == 0) {
            this->freqs.erase(fi);
        }
        // update iterator
        wv.freq_iter = next_fi;
        wv.key_iter = next_fi->keys.begin();
    }

    FreqIter next_freq(FreqIter freq_iter) {
        uint32_t freq = freq_iter->freq + 1;
        freq_iter++;
        if (freq_iter != this->freqs.end() && freq_iter->freq == freq) {
            return freq_iter;
        } else {
            return this->freqs.emplace(freq_iter, freq);
        }
    }

    void evict() {
        assert(this->size() > 0);

        FreqIter lowest = this->freqs.begin();
        assert(lowest->keys.size() > 0);
        // remove the least recent key at back
        this->map.erase(lowest->keys.back());
        lowest->keys.pop_back();
        // remove empty freqency list
        if (lowest->keys.size() == 0) {
            this->freqs.erase(lowest);
        }
    }

    FreqIter zero_freq() {
        if (!(this->freqs.begin() != this->freqs.end() && this->freqs.front().freq == 0)) {
            this->freqs.emplace_front(0);
        }
        assert(this->freqs.front().freq == 0);
        return this->freqs.begin();
    }

public:
    explicit LFUCache_list(int capacity)
        : capacity(capacity)
    {
        assert(capacity >= 0);
    }

    ValueType get(const KeyType &key) {
        MapIter it = this->map.find(key);
        if (it == this->map.end()) {
            return DefaultValue;
        } else {
            this->accessed(it->second);
            return it->second.value;
        }
    }

    ValueType peek(const KeyType &key) {
        MapIter it = this->map.find(key);
        if (it == this->map.end()) {
            return DefaultValue;
        } else {
            return it->second.value;
        }
    }

    void set(const KeyType &key, const ValueType &value) {
        if (this->capacity == 0) {
            return;
        }

        auto it = this->map.find(key);
        if (it == this->map.end()) {
            // evict before insertion
            if (this->size() == this->capacity) {
                this->evict();
            }
            // insert key to frequency list
            auto fi = this->zero_freq();
            fi->keys.emplace_front(key);
            // insert key-value to map
            this->map.emplace(key, WrappedValue(value, fi, fi->keys.begin()));
        } else {
            it->second.value = value;
            this->accessed(it->second);
        }
    }

    size_t size() const {
        return this->map.size();
    }
};
// END LFUCache_list

#ifdef ALGO_heap
typedef LFUCache_heap LFUCache;
#else
#define ALGO_list
typedef LFUCache_list<int, int> LFUCache;
#endif

/**
 * Your LFUCache object will be instantiated and called as such:
 * LFUCache obj = new LFUCache(capacity);
 * int param_1 = obj.get(key);
 * obj.set(key,value);
 */


#ifdef RUN_TEST
namespace std {
    ostream &operator << (ostream& os, const HeapItem &item) {
        os << "(" << item.key << ", s=" << item.serial << ", f=" << item.freq << ")";
        return os;
    }

    typedef LFUCache_list<int, int>::FreqListNode FreqListNode;
    ostream &operator << (ostream& os, const FreqListNode &item) {
        os << "(" << item.freq << ", " << item.keys << ")";
        return os;
    }
}

TEST_CASE("460. LFU Cache") {
    auto cache = LFUCache(2);

    cache.set(1, 1);
    cache.set(2, 2);
    CHECK(cache.get(1) == 1);
    CHECK(true);

    cache.set(3, 3);    // evicts key 2
    CHECK(cache.get(2) == -1);

    CHECK(cache.get(3) == 3);

    cache.set(4, 4);    // evicts key 1
    CHECK(cache.get(1) == -1);

    CHECK(cache.get(3) == 3);
    CHECK(cache.get(4) == 4);

    cache.set(5, 5);
    CHECK(cache.get(4) == -1);

    cache.set(5, 6);
    cache.set(5, 7);
    CHECK(cache.size() == 2);
    CHECK(cache.peek(5) == 7);
    CHECK(cache.peek(3) == 3);

    cache.set(7, 7);
    CHECK(cache.get(3) == -1);

    cache = LFUCache(3);
    cache.set(2, 2);
    cache.set(1, 1);
    cache.get(2);
    cache.get(1);
    cache.get(2);
    cache.set(3, 3);
    cache.set(4, 4);
    CHECK(cache.get(3) == -1);
    CHECK(cache.get(2) == 2);
    CHECK(cache.get(1) == 1);
    CHECK(cache.get(4) == 4);

    // one sized cache
    cache = LFUCache(1);
    for (int i = 0; i < 3; i++) {
        CHECK(cache.get(i) == -1);
        cache.set(i, i);
        CHECK(cache.get(i - 1) == -1);
        CHECK(cache.peek(i) == i);
    }

    // allow zero sized cache
    cache = LFUCache(0);
    cache.set(1, 1);
    CHECK(cache.get(1) == -1);
}
#endif
