# leetcode_cpp

Solutions for leetcode using c++11.

## Compile & Run

-   Compile only:

    `make <prog>`

-   Run test:

    `./build/<prog> -s -d yes`

-   Compile & Test:

    `TEST=1 make <prog>`

-   Add compiler flags:

    `CXXFLAGS=-O2 make <prog>`

-   Profiling

    ```
    TEST=1 CXXFLAGS=-pg make <prog>
    gprof build/<prog> gmon.out >prof.txt
    ```

-   Run all leetcodes:

    `TEST=1 make all`

## Indexes

| Problem                                                                                                                     | Answer                                     | Time | Beats |
|-----------------------------------------------------------------------------------------------------------------------------|--------------------------------------------|------|-------|
| [60. Permutation Sequence](<https://leetcode.com/problems/permutation-sequence/>)                                           | [perm_seq](<src/perm_seq.cpp>)             |      |       |
| [126. Word Ladder II](<https://leetcode.com/problems/word-ladder-ii/>)                                                      | [word_ladder_ii](<src/word_ladder_ii.cpp>) |      |       |
| [Max Points on a Line](<https://leetcode.com/problems/max-points-on-a-line/>)                                               | [max_points](<src/max_points.cpp>)         |      |       |
| [LRU Cache](<https://leetcode.com/problems/lru-cache/>)                                                                     | [lru_cache](<src/lru_cache.cpp>)           |      |       |
| [Divide Two Integers](<https://leetcode.com/problems/divide-two-integers/>)                                                 | [div_int](<src/div_int.cpp>)               |      |       |
| [Fraction to Recurring Decimal](<https://leetcode.com/problems/fraction-to-recurring-decimal/>)                             | [frac2dec](<src/frac2dec.cpp>)             |      |       |
| [Wildcard Matching](<https://leetcode.com/problems/wildcard-matching/>)                                                     | [wildcard](<src/wildcard.cpp>)             |      |       |
| [8. String to Integer (atoi)](<https://leetcode.com/problems/string-to-integer-atoi/>)                                      | [atoi](<src/atoi.cpp>)                     |      |       |
| [65. Valid Number](<https://leetcode.com/problems/valid-number/>)                                                           | [valid_num](<src/valid_num.cpp>)           |      |       |
| [4. Median of Two Sorted Arrays](<https://leetcode.com/problems/median-of-two-sorted-arrays/>)                              | [2array_median](<src/2array_median.cpp>)   |      |       |
| [212. Word Search II](<https://leetcode.com/problems/word-search-ii/>)                                                      | [word_search_ii](<src/word_search_ii.cpp>) |      |       |
| [91. Decode Ways](<https://leetcode.com/problems/decode-ways/>)                                                             | [decode_ways](<src/decode_ways.cpp>)       |      |       |
| [140. Word Break II](<https://leetcode.com/problems/word-break-ii/>)                                                        | [word_break_ii](<src/word_break_ii.cpp>)   |      |       |
| [391. Perfect Rectangle](<https://leetcode.com/problems/perfect-rectangle/>)                                                | [perf_rect](<src/perf_rect.cpp>)           |      |       |
| [68. Text Justification](<https://leetcode.com/problems/text-justification/>)                                               | [text_just](<src/text_just.cpp>)           |      |       |
| [214. Shortest Palindrome](<https://leetcode.com/problems/shortest-palindrome/>)                                            | [short_palin](<src/short_palin.cpp>)       |      |       |
| [30. Substring with Concatenation of All Words](<https://leetcode.com/problems/substring-with-concatenation-of-all-words/>) | [substr_cat](<src/substr_cat.cpp>)         |      |       |
| [174. Dungeon Game](<https://leetcode.com/problems/dungeon-game/>)                                                          | [dungeon](<src/dungeon.cpp>)               |      |       |
| [15. 3Sum](<https://leetcode.com/problems/3sum/>)                                                                           | [3sum](<src/3sum.cpp>)                     |      |       |

