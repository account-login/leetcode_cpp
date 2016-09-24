#ifndef PARSER_H
#define PARSER_H

#include <exception>
#include <stdexcept>
#include <vector>
#include <string>


using std::string;
using std::vector;
using std::logic_error;


// This is the interface that allows for creating nested lists.
// You should not implement it, or speculate about its implementation

enum class NestedIntegerType { INT, LIST };

string to_string(NestedIntegerType type) {
    if (type == NestedIntegerType::INT) {
        return "integer";
    } else {
        return "list";
    }
}


class TypeError: public logic_error {
public:
    TypeError(NestedIntegerType type)
        : logic_error("type error"), type(type)
    {
        this->msg = string("type \"") + to_string(this->type) + "\" not expected.";
    }

    virtual const char* what() const throw() {
        return this->msg.data();
    }

    NestedIntegerType type;

private:
    string msg;
};


class NestedInteger {
public:
    // Constructor initializes an empty nested list.
    NestedInteger()
        : type(NestedIntegerType::LIST) {}

    // Constructor initializes a single integer.
    NestedInteger(int value)
        : type(NestedIntegerType::INT), integer(value) {}

    // Return true if this NestedInteger holds a single integer, rather than a nested list.
    bool isInteger() const {
        return this->type == NestedIntegerType::INT;
    }

    // Return the single integer that this NestedInteger holds, if it holds a single integer
    // The result is undefined if this NestedInteger holds a nested list
    int getInteger() const {
        if (this->type != NestedIntegerType::INT) {
            throw TypeError(this->type);
        }
        return this->integer;
    }

    // Set this NestedInteger to hold a single integer.
    void setInteger(int value) {
        if (this->type != NestedIntegerType::INT) {
            throw TypeError(this->type);
        }
        this->integer = value;
    }

    // Set this NestedInteger to hold a nested list and adds a nested integer to it.
    void add(const NestedInteger &ni) {
        if (this->type != NestedIntegerType::LIST) {
            throw TypeError(this->type);
        }
        this->list.push_back(ni);
    }

    // Return the nested list that this NestedInteger holds, if it holds a nested list
    // The result is undefined if this NestedInteger holds a single integer
    const vector<NestedInteger> &getList() const {
        if (this->type != NestedIntegerType::LIST) {
            throw TypeError(this->type);
        }
        return this->list;
    }

private:
    NestedIntegerType type;
    int integer;
    vector<NestedInteger> list;
};

#endif // PARSER_H
