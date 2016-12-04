#ifndef STRINGIFY_H
#define STRINGIFY_H

#include <iostream>
#include <vector>
#include <list>
#include <unordered_set>


namespace std {
    template<class T1, class T2>
    std::ostream &operator << (std::ostream& os, const pair<T1, T2> &value) {
        return os << "( " << value.first << ", " << value.second << " )";
    }

    template<class ElemType>
    std::ostream &operator << (std::ostream& os, const unordered_multiset<ElemType> &value) {
        return print_container(os, value, "hash_multiset { ");
    }

    template<class ElemType>
    std::ostream &operator << (std::ostream& os, const vector<ElemType> &value) {
        return print_container(os, value, "vector { ");
    }

    template<class ElemType>
    std::ostream &operator << (std::ostream& os, const list<ElemType> &value) {
        return print_container(os, value, "list { ");
    }

    template<typename ContType>
    std::ostream &print_container (
        std::ostream& os,
        const ContType &value,
        std::string left = "{ ",
        std::string right = " }")
    {
        os << left;
        join(os, value.begin(), value.end());
        os << right;
        return os;
    }

    template<typename IterType>
    std::ostream &join(
        std::ostream& os,
        const IterType &begin, const IterType &end,
        const std::string &sep = ", ")
    {
        std::string nsep = "";
        for (auto it = begin; it != end; ++it) {
            os << nsep << *it;
            nsep = sep;
        }
        return os;
    }
}

#endif // STRINGIFY_H
