#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/compare-version-numbers/


using namespace std;


class Solution {
public:
    int compareVersion(const string &version1, const string &version2) {
        const char *v1 = version1.data();
        const char *v2 = version2.data();

        while (true) {
            int i1 = atoi(v1);
            int i2 = atoi(v2);

            if (i1 > i2) {
                return 1;
            } else if (i1 < i2) {
                return -1;
            } else {
                v1 = strchr(v1, '.');
                v2 = strchr(v2, '.');

                if (!v1 && !v2) {
                    return 0;
                } else if (!v1) {
                    if (is_all_zero(v2 + 1)) {
                        return 0;
                    } else {
                        return -1;
                    }
                } else if (!v2) {
                    if (is_all_zero(v1 + 1)) {
                        return 0;
                    } else {
                        return 1;
                    }
                }

                // skip dot
                v1++;
                v2++;
            }
        }
    }

    bool is_all_zero(const char *v) {
        while (true) {
            if (atoi(v) != 0) {
                return false;
            } else {
                v = strchr(v, '.');
                if (!v) {
                    return true;
                } else {
                    v++;
                }
            }
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("165. Compare Version Numbers") {
    Solution s;

    CHECK(s.compareVersion("1.2", "1.3") < 0);
    CHECK(s.compareVersion("1.0", "1.00") == 0);
    CHECK(s.compareVersion("1.0", "1.0.0") == 0);
    CHECK(s.compareVersion("1.0.1", "1.0") > 0);
}
#endif
