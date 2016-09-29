#include <string>
#include <utility>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>

// https://leetcode.com/problems/remove-k-digits/


using namespace std;


class Solution {
public:
    string removeKdigits(string num, int k) {
        assert(num.size() >= k);
        if (num.size() == k) {
            return "0";
        }

        auto next = [&num](int i) {
            i++;
            while (num[i] == 'x') {
                i++;
            }
            return i;
        };

        auto left_near = [&num](int i) {
            while (i > 0 && num[i] == 'x') {
                i--;
            }
            // i == 0, not found in left, go to right
            while (num[i] == 'x') {
                i++;
            }
            return i;
        };

        int start = next(-1);
        for (int i = 0; i < k; i++) {
            for(int left = start, right = next(left);
                ;
                left = right, right = next(right))
            {
                if (num[right] == '\0') {
                    // ascending digits, remove k - i digits from end and return.
                    for (int j = 0; j < k - i - 1; j++) {
                        left--;
                        left = left_near(left);
                    }
                    num[left] = '\0';
                    goto RETURN;
                } else if (num[right] < num[left]) {
                    num[left] = 'x';
                    start = left_near(left);
                    break;
                }
            }
        }

    RETURN:
        int src = next(-1);
        int dest = 0;
        bool leading = true;
        do {
            if (!(leading && num[src] == '0')) {
                leading = false;
                num[dest] = num[src];
                dest ++;
            }
            src = next(src);
        } while (num[src] != '\0');

        num.resize(dest);
        if (num.empty()) {
            return "0";
        } else {
            return num;
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("402. Remove K Digits") {
    Solution s;

    CHECK(s.removeKdigits("", 0) == "0");

    CHECK(s.removeKdigits("1432219", 3) == "1219");
    CHECK(s.removeKdigits("10200", 1) == "200");
    CHECK(s.removeKdigits("10", 2) == "0");
    CHECK(s.removeKdigits("100", 1) == "0");
    CHECK(s.removeKdigits("00100", 1) == "0");
    CHECK(s.removeKdigits("010100", 2) == "0");
    CHECK(s.removeKdigits("010100", 3) == "0");

    CHECK(s.removeKdigits("00122345678", 4) == "12234");
    CHECK(s.removeKdigits("001223945678", 5) == "12234");
    CHECK(s.removeKdigits("001226945678", 2) == "12245678");

    CHECK(s.removeKdigits(string(100000, '1'), 50000) == string(50000, '1'));

    int N = 10000;
    string num = "";
    for (int i = 0; i < N; i++) {
        num += "9876543210";
    }
    CHECK(s.removeKdigits(num, 9 * N) == "0");

    num = "";
    for (int i = 0; i < N; i++) {
        num += "0123456789";
    }
    CHECK(s.removeKdigits(num, 9 * N) == "0");
    CHECK(s.removeKdigits(num, 9 * (N / 2)) == num.substr(10 * (N / 2) + 1));

    num = "";
    for (int i = 0; i < N; i++) {
        num += "0123456789";
        num += "9876543210";
    }
    CHECK(s.removeKdigits(num, 9 * N) == num.substr(10 * N + 1));

    num = string(N, '5');
    for (int i = 0; i < N; i += 2) {
        num[i] = '2';
    }
    CHECK(s.removeKdigits(num, N / 2) == string(N / 2, '2'));
}
#endif
