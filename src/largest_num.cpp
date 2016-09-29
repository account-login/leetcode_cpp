#include <vector>
#include <string>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/largest-number/


using namespace std;


class Solution {
public:
    string largestNumber(const vector<int> &nums) {
        vector<string> words;
        for (int n : nums) {
            words.push_back(to_string(n));
        }

        /// this is equivalent to (str1 + str2) > (str2 + str1);
        auto comp = [](const string &str1, const string &str2) -> bool {
            const char *s1 = str1.data();
            const char *s2 = str2.data();

            while (true) {
                if (*s1 > *s2) {
                    return true;
                } else if (*s1 < *s2) {
                    return false;
                } else {
                    s1++;
                    s2++;

                    if (*s1 == '\0' && *s2 == '\0') {
                        // equal
                        return false;
                    }

                    if (*s1 == '\0') {
                        s1 = str2.data();
                    }
                    if (*s2 == '\0') {
                        s2 = str1.data();
                    }
                }
            }
        };
        sort(words.begin(), words.end(), comp);

        // all zero
        if (words[0][0] == '0') {
            return "0";
        } else {
            string ans;
            for (const string &word : words) {
                ans += word;
            }
            return ans;
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("179. Largest Number") {
    Solution s;

    CHECK(s.largestNumber({ 1 }) == "1");
    CHECK(s.largestNumber({ 3, 30, 34, 5, 9 }) == "9534330");
    CHECK(s.largestNumber({ 5, 57, 58, 54, 55, 53, 532, 5321, 555 }) == "585755555554535325321");

    vector<int> nums(100, 0);
    CHECK(s.largestNumber(nums) == "0");

    CHECK(s.largestNumber({ 2, 2281, 216, 213 }) == "22812216213");

    nums = { 4704,6306,9385,7536,3462,4798,5422,5529,8070,6241,9094,7846,663,6221,216,6758,8353,3650,3836,8183,3516,5909,6744,1548,5712,2281,3664,7100,6698,7321,4980,8937,3163,5784,3298,9890,1090,7605,1380,1147,1495,3699,9448,5208,9456,3846,3567,6856,2000,3575,7205,2697,5972,7471,1763,1143,1417,6038,2313,6554,9026,8107,9827,7982,9685,3905,8939,1048,282,7423,6327,2970,4453,5460,3399,9533,914,3932,192,3084,6806,273,4283,2060,5682,2,2362,4812,7032,810,2465,6511,213,2362,3021,2745,3636,6265,1518,8398 };
    string ans = "98909827968595339456944893859149094902689398937839883538183810810780707982784676057536747174237321720571007032685668066758674466986636554651163276306626562416221603859725909578457125682552954605422520849804812479847044453428339323905384638363699366436503636357535673516346233993298316330843021297028227452732697246523622362231322812216213206020001921763154815181495141713801147114310901048";
    CHECK(s.largestNumber(nums) == ans);
}
#endif
