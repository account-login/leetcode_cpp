#include <string>
#include <vector>

#ifdef RUN_TEST
#   include "catch.hpp"
#   include "stringify.h"
#   define private public
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/tag-validator/


using namespace std;


class TagValidator {
public:
    TagValidator(const string &input) : input(input) {}
    bool validate() {
        try {
            tag();
        } catch (Bad &) {
            return false;
        }
        return this->cur == this->input.size();
    }

private:
    struct Bad {};

    void expect(const char *str) {
        while (*str) {
            if (this->input[cur++] != *str++) {
                throw Bad();
            }
        }
    }

    string tag_name() {
        string name;
        for (int i = 0; i < 9; ++i) {
            char ch = this->input[this->cur++];
            if ('A' <= ch && ch <= 'Z') {
                name.push_back(ch);
            } else {
                --this->cur;
                break;
            }
        }
        if (name.empty()) {
            throw Bad();
        } else {
            return name;
        }
    }

    string open() {
        expect("<");
        string name = tag_name();
        expect(">");
        return name;
    }

    string close() {
        expect("</");
        string name = tag_name();
        expect(">");
        return name;
    }

    void content() {
        size_t size = this->input.size();
        while (this->cur < size) {
            char ch = this->input[this->cur++];
            if (ch == '<') {
                char la = this->input[this->cur--];
                if (la == '!') {
                    cdata();
                } else if (la == '/') {
                    return;
                } else {
                    tag();
                }
            }
        }
    }

    void tag() {
        string open_tag = open();
        content();
        if (open_tag != close()) {
            throw Bad();
        }
    }

    void cdata() {
        expect("<![CDATA[");
        size_t end = this->input.find("]]>", this->cur);
        if (end == string::npos) {
            throw Bad();
        } else {
            this->cur = end + 3;
        }
    }

    const string &input;
    size_t cur = 0;
};


class Solution {
public:
    bool isValid(const string &code) {
        return TagValidator(code).validate();
    }
};


#ifdef RUN_TEST
TEST_CASE("591. Tag Validator") {
    Solution s;

    CHECK(s.isValid("<A></A>"));
    CHECK(s.isValid("<A>xxx</A>"));
    CHECK(s.isValid("<DIV>This is the first line <![CDATA[<div>]]></DIV>"));
    CHECK(s.isValid("<DIV>>>  ![cdata[]] <![CDATA[<div>]>]]>]]>>]</DIV>"));

    CHECK(s.isValid("<AAAABBBBA></AAAABBBBA>"));
    CHECK(!s.isValid("<AAAABBBBAA></AAAABBBBAA>"));
    CHECK(!s.isValid("<A></A>>"));
    CHECK(!s.isValid("<A>  <B> </A>   </B>"));
    CHECK(!s.isValid("<DIV>  div tag is not closed  <DIV>"));
    CHECK(!s.isValid("<DIV>  unmatched <  </DIV>"));
    CHECK(!s.isValid("<DIV> closed tags with invalid tag name  <b>123</b> </DIV>"));
    CHECK(!s.isValid("<DIV> unmatched tags with invalid tag name  </1234567890> and <CDATA[[]]>  </DIV>"));
    CHECK(!s.isValid("<DIV>  unmatched start tag <B>  and unmatched end tag </C>  </DIV>"));
}
#endif
