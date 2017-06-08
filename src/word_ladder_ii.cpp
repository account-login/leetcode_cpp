#include <algorithm>
#include <unordered_set>
#include <limits>
#include <iterator>
#include <iostream>

#ifdef RUN_TEST
#   include "catch.hpp"
#else
#   define NDEBUG   // remove assert() call
#endif
#include <cassert>


// https://leetcode.com/problems/word-ladder-ii/


using namespace std;


bool word_cmp(const string &s1, const string &s2)
{
    int count = 0;
    assert(s1.size() == s2.size());
    for (size_t i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i]) {
            count++;
            if (count > 1) {
                return false;
            }
        }
    }
    return count == 1;
}


vector<vector<int>> make_graph(const vector<string> &words) {
    vector<vector<int>> ans(words.size());
    for (size_t j = 1; j < words.size(); ++j) {
        for (size_t i = 0; i < j; ++i) {
            if (word_cmp(words[i], words[j])) {
                ans[i].push_back(static_cast<int>(j));
                ans[j].push_back(static_cast<int>(i));
            }
        }
    }
    return ans;
}


vector<vector<int>> shortest_path(const vector<vector<int>> &graph, int begin, int end) {
    enum {INIT, PENDING, PROCESSED};
    vector<int> states(graph.size(), INIT);
    vector<vector<int>> back_graph(graph.size());

    vector<int> cur_level;
    cur_level.push_back(begin);
    vector<int> next_level;

    while (!cur_level.empty()) {
        for (int cur : cur_level) {
            if (cur == end) {
                return back_graph;
            }
            states[cur] = PROCESSED;
        }
        for (int cur : cur_level) {
            for (int next : graph[cur]) {
                switch (states[next]) {
                case INIT:
                    next_level.push_back(next);
                    states[next] = PENDING;
                    // fall through
                case PENDING:
                    back_graph[next].push_back(cur);
                }
            }
        }

        cur_level.clear();
        cur_level.swap(next_level);
    }

    // no path from begin to end
    return {};
}


vector<vector<string>>
back_graph_to_path(
    const vector<vector<int>> &back_graph, int begin, int end, const vector<string> &words)
{
    if (begin == end) {
        return {
            {words[end]},
        };
    } else {
        vector<vector<string>> ans;
        for (int prev : back_graph[end]) {
            vector<vector<string>> sub = back_graph_to_path(back_graph, begin, prev, words);
            ans.reserve(ans.size() + sub.size());
            for (vector<string> &subpath : sub) {
                ans.emplace_back(move(subpath));
            }
        }
        for (vector<string> &path : ans) {
            path.emplace_back(words[end]);
        }
        return ans;
    }
}


template<class T>
int vector_unique_add(vector<T> &vec, const T &value) {
    auto it = find(vec.cbegin(), vec.cend(), value);
    if (it == vec.cend()) {
        vec.emplace_back(value);
        return static_cast<int>(vec.size() - 1);
    } else {
        return static_cast<int>(distance(vec.cbegin(), it));
    }
}


class Solution {
public:
    vector<vector<string>>
    findLadders(const string &begin_word, const string &end_word, vector<string> word_list) {
        int begin = vector_unique_add(word_list, begin_word);
        int end = static_cast<int>(
            find(word_list.cbegin(), word_list.cend(), end_word) - word_list.cbegin()
        );
        if (end == static_cast<int>(word_list.size())) {
            return {};
        }

        vector<vector<int>> graph = make_graph(word_list);
        vector<vector<int>> back_graph = shortest_path(graph, begin, end);
        if (back_graph.size() == 0) {
            return {};
        } else {
            return back_graph_to_path(back_graph, begin, end, word_list);
        }
    }
};


#ifdef RUN_TEST
TEST_CASE("126. Word Ladder II") {
    CHECK(word_cmp("asdf", "asdb"));
    CHECK(!word_cmp("asdf", "asdf"));
    CHECK(!word_cmp("asdf", "asas"));

    Solution s;
    vector<vector<string>> expects;

    auto solve = [&](
        const string &word_begin, const string &word_end, const vector<string> &words)
    {
        auto ans = s.findLadders(word_begin, word_end, words);
        sort(ans.begin(), ans.end());
        return ans;
    };

    expects = {};
    CHECK(solve("ab", "ba", {}) == expects);
    CHECK(solve("ab", "ba", {"zz"}) == expects);
    CHECK(solve("a", "b", {"zz"}) == expects);

    expects = {
        {"hit", "hot", "dot", "dog", "cog"},
        {"hit", "hot", "lot", "log", "cog"}
    };
    CHECK(solve("hit", "cog", {"hot", "dot", "dog", "lot", "log", "cog"}) == expects);

    expects = {
        {"hit", "hot"}
    };
    CHECK(solve("hit", "hot", {"hit", "hot"}) == expects);
    CHECK(solve("hit", "hot", {"hot"}) == expects);
    CHECK(solve("hit", "hot", {"xxx", "hot"}) == expects);

    expects = {
        {"xxxa", "xxxy", "xxzy", "xbzy", "abzy"},
    };
    CHECK(solve("xxxa", "abzy", {"xbzy", "xxzy", "xxxy", "abzy"}) == expects);

    expects = {
        {"xxxx", "xxxa", "xxca", "yxca", "yxaa", "zxaa", "zzaa", "zzya", "zzyy"},
        {"xxxx", "xxxa", "xxca", "yxca", "yxcb", "zxcb", "zzcb", "zzyb", "zzyy"},
        {"xxxx", "xxxb", "xxab", "yxab", "yxaa", "zxaa", "zzaa", "zzya", "zzyy"},
        {"xxxx", "xxxb", "xxab", "yxab", "yxcb", "zxcb", "zzcb", "zzyb", "zzyy"},
    };
    CHECK(solve("xxxx", "zzyy", {
        "xxxa", "xxxb",
        "xxca", "xxab", "yxca", "yxab",
        "yxcb", "yxaa", "zxcb", "zxaa",
        "zzcb", "zzaa",
        "zzyb", "zzya",
        "zzyy"
    }) == expects);

    expects = {
        {"cet", "cat", "can", "ian", "inn", "ins", "its", "ito", "ibo", "ibm", "ism"},
        {"cet", "cot", "con", "ion", "inn", "ins", "its", "ito", "ibo", "ibm", "ism"},
        {"cet", "get", "gee", "gte", "ate", "ats", "its", "ito", "ibo", "ibm", "ism"},
    };
    CHECK(solve("cet", "ism", {
        "kid","tag","pup","ail","tun","woo","erg","luz","brr","gay","sip","kay","per","val","mes","ohs","now","boa","cet","pal","bar","die","war","hay","eco","pub","lob","rue","fry","lit","rex","jan","cot","bid","ali","pay","col","gum","ger","row","won","dan","rum","fad","tut","sag","yip","sui","ark","has","zip","fez","own","ump","dis","ads","max","jaw","out","btu","ana","gap","cry","led","abe","box","ore","pig","fie","toy","fat","cal","lie","noh","sew","ono","tam","flu","mgm","ply","awe","pry","tit","tie","yet","too","tax","jim","san","pan","map","ski","ova","wed","non","wac","nut","why","bye","lye","oct","old","fin","feb","chi","sap","owl","log","tod","dot","bow","fob","for","joe","ivy","fan","age","fax","hip","jib","mel","hus","sob","ifs","tab","ara","dab","jag","jar","arm","lot","tom","sax","tex","yum","pei","wen","wry","ire","irk","far","mew","wit","doe","gas","rte","ian","pot","ask","wag","hag","amy","nag","ron","soy","gin","don","tug","fay","vic","boo","nam","ave","buy","sop","but","orb","fen","paw","his","sub","bob","yea","oft","inn","rod","yam","pew","web","hod","hun","gyp","wei","wis","rob","gad","pie","mon","dog","bib","rub","ere","dig","era","cat","fox","bee","mod","day","apr","vie","nev","jam","pam","new","aye","ani","and","ibm","yap","can","pyx","tar","kin","fog","hum","pip","cup","dye","lyx","jog","nun","par","wan","fey","bus","oak","bad","ats","set","qom","vat","eat","pus","rev","axe","ion","six","ila","lao","mom","mas","pro","few","opt","poe","art","ash","oar","cap","lop","may","shy","rid","bat","sum","rim","fee","bmw","sky","maj","hue","thy","ava","rap","den","fla","auk","cox","ibo","hey","saw","vim","sec","ltd","you","its","tat","dew","eva","tog","ram","let","see","zit","maw","nix","ate","gig","rep","owe","ind","hog","eve","sam","zoo","any","dow","cod","bed","vet","ham","sis","hex","via","fir","nod","mao","aug","mum","hoe","bah","hal","keg","hew","zed","tow","gog","ass","dem","who","bet","gos","son","ear","spy","kit","boy","due","sen","oaf","mix","hep","fur","ada","bin","nil","mia","ewe","hit","fix","sad","rib","eye","hop","haw","wax","mid","tad","ken","wad","rye","pap","bog","gut","ito","woe","our","ado","sin","mad","ray","hon","roy","dip","hen","iva","lug","asp","hui","yak","bay","poi","yep","bun","try","lad","elm","nat","wyo","gym","dug","toe","dee","wig","sly","rip","geo","cog","pas","zen","odd","nan","lay","pod","fit","hem","joy","bum","rio","yon","dec","leg","put","sue","dim","pet","yaw","nub","bit","bur","sid","sun","oil","red","doc","moe","caw","eel","dix","cub","end","gem","off","yew","hug","pop","tub","sgt","lid","pun","ton","sol","din","yup","jab","pea","bug","gag","mil","jig","hub","low","did","tin","get","gte","sox","lei","mig","fig","lon","use","ban","flo","nov","jut","bag","mir","sty","lap","two","ins","con","ant","net","tux","ode","stu","mug","cad","nap","gun","fop","tot","sow","sal","sic","ted","wot","del","imp","cob","way","ann","tan","mci","job","wet","ism","err","him","all","pad","hah","hie","aim","ike","jed","ego","mac","baa","min","com","ill","was","cab","ago","ina","big","ilk","gal","tap","duh","ola","ran","lab","top","gob","hot","ora","tia","kip","han","met","hut","she","sac","fed","goo","tee","ell","not","act","gil","rut","ala","ape","rig","cid","god","duo","lin","aid","gel","awl","lag","elf","liz","ref","aha","fib","oho","tho","her","nor","ace","adz","fun","ned","coo","win","tao","coy","van","man","pit","guy","foe","hid","mai","sup","jay","hob","mow","jot","are","pol","arc","lax","aft","alb","len","air","pug","pox","vow","got","meg","zoe","amp","ale","bud","gee","pin","dun","pat","ten","mob"}
    ) == expects);
}
#endif
