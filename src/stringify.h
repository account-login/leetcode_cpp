#ifndef STRINGIFY_H
#define STRINGIFY_H

#include <iostream>
#include <chrono>
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

    string fmt_time(double sec) {
        char buf[100];

        if (sec >= 10) {
            sprintf(buf, "%.1f s", sec);
        } else if (sec >= 1) {
            sprintf(buf, "%.2f s", sec);
        } else if (sec >= 0.1) {
            sprintf(buf, "%.0f ms", sec * 1000);
        } else if (sec >= 0.01) {
            sprintf(buf, "%.1f ms", sec * 1000);
        } else if (sec >= 0.001) {
            sprintf(buf, "%.2f ms", sec * 1000);
        } else if (sec >= 0.0001) {
            sprintf(buf, "%.3f ms", sec * 1000);
        } else {
            sprintf(buf, "%.1f us", sec * 1000000);
        }

        return string(buf);
    }

    template<typename T>
    std::ostream &operator << (std::ostream& os, const std::chrono::duration<T> &value) {
        os << fmt_time(value.count());
        return os;
    }
}

#endif // STRINGIFY_H
