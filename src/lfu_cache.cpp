#include <unordered_map>
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

namespace std {
    ostream &operator << (ostream& os, const HeapItem &item) {
        os << "(" << item.key << ", s=" << item.serial << ", f=" << item.freq << ")";
        return os;
    }
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


class LFUCache {
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
    explicit LFUCache(int capacity)
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

/**
 * Your LFUCache object will be instantiated and called as such:
 * LFUCache obj = new LFUCache(capacity);
 * int param_1 = obj.get(key);
 * obj.set(key,value);
 */


#ifdef RUN_TEST
TEST_CASE("460. LFU Cache") {
    auto cache = LFUCache(2);

    cache.set(1, 1);
    cache.set(2, 2);
    CHECK(cache.get(1) == 1);

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
