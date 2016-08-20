#include <iostream>
#include <cassert>
#include <unordered_map>
#include <list>
#include <utility>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

// https://leetcode.com/problems/lru-cache/


using namespace std;


struct ValueWrapper {
    int value;
    list<int>::const_iterator pos;

    ValueWrapper(int value, list<int>::iterator pos) : value(value), pos(pos) {}
};


class LRUCache{
public:
    size_t capacity;
    list<int> order;
    unordered_map<int, ValueWrapper> content;

    LRUCache(int capacity) {
        this->capacity = capacity;
    }

    int get(int key) {
        auto found = this->content.find(key);
        if (found != this->content.end()) {
            return found->second.value;
        } else {
            return -1;
        }
    }

    void set(int key, int value) {
        bool has_key = this->remove(key);
        if (!has_key) {
            if (this->content.size() == this->capacity) {
                int old_key = this->order.front();
                this->order.pop_front();
                this->content.erase(old_key);
            }
        }
        this->order.push_back(key);
        auto last = --this->order.end();
        auto vw = ValueWrapper(value, last);
        this->content.insert({ key, vw });
    }

    bool remove(int key) {
        auto found = this->content.find(key);
        if (found != this->content.end()) {
            this->order.erase(found->second.pos);
            this->content.erase(key);
            return true;
        } else {
            return false;
        }
    }
};


TEST_CASE( "LRU Cache" ) {
    auto s = LRUCache(3);

    s.set(1, 3);
    CHECK(s.get(1) == 3);
    CHECK(s.get(5) == -1);

    s.set(2, 4);
    s.set(3, 6);
    s.set(4, 8);
    CHECK(s.get(1) == -1);

    s.set(1, 2);
    CHECK(s.get(2) == -1);
    s.set(3, 8);
    CHECK((s.get(3) == 8 && s.get(4) == 8 && s.get(1) == 2));
    CHECK(s.order.size() == 3);
    CHECK(s.content.size() == 3);
}