#include <iostream>
#include <cassert>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

// https://leetcode.com/problems/permutation-sequence/


using namespace std;

class Solution {
public:
    Solution() {
        this->factorial[0] = 1;
        for (int i = 1; i < 10; i++) {
            this->factorial[i] = i * this->factorial[i - 1];
        }
    }

    string getPermutation(int n, int k) {
        vector<char> ans(n);
        k -= 1;

        for (int i = 0; i < n - 1; i++) {
            ans[i] = k / factorial[n - i - 1];
            k -= ans[i] * factorial[n - i - 1];
        }
        ans[n - 1] = k;

        string ret(n, '\0');
        for (int i = 0; i < n; i++) {
            ret[i] = this->next_char(ret, ans[i] + 1);
        }
        return ret;
    }

private:
    int factorial[10];

    char next_char(const string& exclusion, int n) {
        char ret = '0';
        for (int i = 0; i < n; i++) {
            ret += 1;
            while(exclusion.find(ret) != string::npos) {
                ret++;
            }
        }

        return ret;
    }
};


TEST_CASE( "Permutation Sequence" ) {
    Solution s;
    CHECK( s.getPermutation(3, 1) == "123" );
    CHECK( s.getPermutation(3, 2) == "132" );
    CHECK( s.getPermutation(3, 3) == "213" );
    CHECK( s.getPermutation(3, 4) == "231" );
    CHECK( s.getPermutation(3, 5) == "312" );
    CHECK( s.getPermutation(3, 6) == "321" );
}
