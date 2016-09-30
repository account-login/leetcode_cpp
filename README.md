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

| Problem                                                                                                                        | Answer                                           |  Time | Beats |
|--------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------|-------|-------|
| [60. Permutation Sequence](<https://leetcode.com/problems/permutation-sequence/>)                                              | [perm_seq](<src/perm_seq.cpp>)                   |       |       |
| [126. Word Ladder II](<https://leetcode.com/problems/word-ladder-ii/>)                                                         | [word_ladder_ii](<src/word_ladder_ii.cpp>)       |       |       |
| [149. Max Points on a Line](<https://leetcode.com/problems/max-points-on-a-line/>)                                             | [max_points](<src/max_points.cpp>)               |       |       |
| [146. LRU Cache](<https://leetcode.com/problems/lru-cache/>)                                                                   | [lru_cache](<src/lru_cache.cpp>)                 |       |       |
| [29. Divide Two Integers](<https://leetcode.com/problems/divide-two-integers/>)                                                | [div_int](<src/div_int.cpp>)                     |       |       |
| [166. Fraction to Recurring Decimal](<https://leetcode.com/problems/fraction-to-recurring-decimal/>)                           | [frac2dec](<src/frac2dec.cpp>)                   |       |       |
| [44. Wildcard Matching](<https://leetcode.com/problems/wildcard-matching/>)                                                    | [wildcard](<src/wildcard.cpp>)                   |  18ms |   62% |
| [8. String to Integer (atoi)](<https://leetcode.com/problems/string-to-integer-atoi/>)                                         | [atoi](<src/atoi.cpp>)                           |       |       |
| [65. Valid Number](<https://leetcode.com/problems/valid-number/>)                                                              | [valid_num](<src/valid_num.cpp>)                 |       |       |
| [4. Median of Two Sorted Arrays](<https://leetcode.com/problems/median-of-two-sorted-arrays/>)                                 | [2array_median](<src/2array_median.cpp>)         |       |       |
| [212. Word Search II](<https://leetcode.com/problems/word-search-ii/>)                                                         | [word_search_ii](<src/word_search_ii.cpp>)       |       |       |
| [91. Decode Ways](<https://leetcode.com/problems/decode-ways/>)                                                                | [decode_ways](<src/decode_ways.cpp>)             |       |       |
| [140. Word Break II](<https://leetcode.com/problems/word-break-ii/>)                                                           | [word_break_ii](<src/word_break_ii.cpp>)         |       |       |
| [391. Perfect Rectangle](<https://leetcode.com/problems/perfect-rectangle/>)                                                   | [perf_rect](<src/perf_rect.cpp>)                 |       |       |
| [68. Text Justification](<https://leetcode.com/problems/text-justification/>)                                                  | [text_just](<src/text_just.cpp>)                 |       |       |
| [214. Shortest Palindrome](<https://leetcode.com/problems/shortest-palindrome/>)                                               | [short_palin](<src/short_palin.cpp>)             |       |       |
| [30. Substring with Concatenation of All Words](<https://leetcode.com/problems/substring-with-concatenation-of-all-words/>)    | [substr_cat](<src/substr_cat.cpp>)               | 149ms |   75% |
| [174. Dungeon Game](<https://leetcode.com/problems/dungeon-game/>)                                                             | [dungeon](<src/dungeon.cpp>)                     |       |       |
| [15. 3Sum](<https://leetcode.com/problems/3sum/>)                                                                              | [3sum](<src/3sum.cpp>)                           |  49ms |   70% |
| [130. Surrounded Regions](<https://leetcode.com/problems/surrounded-regions/>)                                                 | [surr_region](<src/surr_region.cpp>)             |  49ms |    8% |
| [76. Minimum Window Substring](<https://leetcode.com/problems/minimum-window-substring/>)                                      | [min_window](<src/min_window.cpp>)               |   9ms |   86% |
| [335. Self Crossing](<https://leetcode.com/problems/self-crossing/>)                                                           | [self_cross](<src/self_cross.cpp>)               |   0ms |       |
| [165. Compare Version Numbers](<https://leetcode.com/problems/compare-version-numbers/>)                                       | [version_cmp](<src/version_cmp.cpp>)             |   0ms |       |
| [307. Range Sum Query - Mutable](<https://leetcode.com/problems/range-sum-query-mutable/>)                                     | [range_sum_mut](<src/range_sum_mut.cpp>)         |  52ms |   63% |
| [179. Largest Number](<https://leetcode.com/problems/largest-number/>)                                                         | [largest_num](<src/largest_num.cpp>)             |   3ms |   94% |
| [211. Add and Search Word - Data structure design](<https://leetcode.com/problems/add-and-search-word-data-structure-design/>) | [add_n_search](<src/add_n_search.cpp>)           |  69ms |   96% |
| [98. Validate Binary Search Tree](<https://leetcode.com/problems/validate-binary-search-tree/>)                                | [valid_bst](<src/valid_bst.cpp>)                 |  19ms |   12% |
| [189. Rotate Array](<https://leetcode.com/problems/rotate-array/>)                                                             | [rotate_array](<src/rotate_array.cpp>)           |  26ms |   31% |
| [32. Longest Valid Parentheses](<https://leetcode.com/problems/longest-valid-parentheses/>)                                    | [longest_par](<src/longest_par.cpp>)             |  12ms |   22% |
| [304. Range Sum Query 2D - Immutable](<https://leetcode.com/problems/range-sum-query-2d-immutable/>)                           | [range_sum_2d_immu](<src/range_sum_2d_immu.cpp>) |  23ms |   47% |
| [10. Regular Expression Matching](<https://leetcode.com/problems/regular-expression-matching/>)                                | [regex](<src/regex.cpp>)                         |  36ms |   43% |
| [390. Elimination Game](<https://leetcode.com/problems/elimination-game/>)                                                     | [elim_game](<src/elim_game.cpp>)                 |  29ms |   62% |
| [44. Wildcard Matching](<https://leetcode.com/problems/wildcard-matching/>)                                                    | [wildcard_dp](<src/wildcard_dp.cpp>)             | 119ms |   48% |
| [402. Remove K Digits](<https://leetcode.com/problems/remove-k-digits/>)                                                       | [remove_k](<src/remove_k.cpp>)                   |   6ms |   56% |
| [18. 4Sum](<https://leetcode.com/problems/4sum/>)                                                                              | [4sum](<src/4sum.cpp>)                           |  16ms |   86% |
| [385. Mini Parser](<https://leetcode.com/problems/mini-parser/>)                                                               | [parser](<src/parser.cpp>)                       |  22ms |   99% |
| [220. Contains Duplicate III](<https://leetcode.com/problems/contains-duplicate-iii/>)                                         | [contain_dup_iii](<src/contain_dup_iii.cpp>)     |  19ms |   92% |
| [295. Find Median from Data Stream](<https://leetcode.com/problems/find-median-from-data-stream/>)                             | [median_stream](<src/median_stream.cpp>)         | 109ms |   97% |
| [135. Candy](<https://leetcode.com/problems/candy/>)                                                                           | [candy](<src/candy.cpp>)                         |  46ms |   21% |

