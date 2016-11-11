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

-   Coverage

    ```
    TEST=1 CXXFLAGS=--coverage make <prog>
    gcov <prog>
    ```

-   Run all leetcodes:

    `TEST=1 make all`

## Indexes

| Problem                                                                                                                                  | Answer                                           |  Time | Beats |
|------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------|-------|-------|
| [60. Permutation Sequence](<https://leetcode.com/problems/permutation-sequence/>)                                                        | [perm_seq](<src/perm_seq.cpp>)                   |       |       |
| [126. Word Ladder II](<https://leetcode.com/problems/word-ladder-ii/>)                                                                   | [word_ladder_ii](<src/word_ladder_ii.cpp>)       |       |       |
| [149. Max Points on a Line](<https://leetcode.com/problems/max-points-on-a-line/>)                                                       | [max_points](<src/max_points.cpp>)               |       |       |
| [146. LRU Cache](<https://leetcode.com/problems/lru-cache/>)                                                                             | [lru_cache](<src/lru_cache.cpp>)                 |       |       |
| [29. Divide Two Integers](<https://leetcode.com/problems/divide-two-integers/>)                                                          | [div_int](<src/div_int.cpp>)                     |       |       |
| [166. Fraction to Recurring Decimal](<https://leetcode.com/problems/fraction-to-recurring-decimal/>)                                     | [frac2dec](<src/frac2dec.cpp>)                   |       |       |
| [44. Wildcard Matching](<https://leetcode.com/problems/wildcard-matching/>)                                                              | [wildcard](<src/wildcard.cpp>)                   |  18ms |   62% |
| [8. String to Integer (atoi)](<https://leetcode.com/problems/string-to-integer-atoi/>)                                                   | [atoi](<src/atoi.cpp>)                           |       |       |
| [65. Valid Number](<https://leetcode.com/problems/valid-number/>)                                                                        | [valid_num](<src/valid_num.cpp>)                 |       |       |
| [4. Median of Two Sorted Arrays](<https://leetcode.com/problems/median-of-two-sorted-arrays/>)                                           | [2array_median](<src/2array_median.cpp>)         |       |       |
| [212. Word Search II](<https://leetcode.com/problems/word-search-ii/>)                                                                   | [word_search_ii](<src/word_search_ii.cpp>)       |       |       |
| [91. Decode Ways](<https://leetcode.com/problems/decode-ways/>)                                                                          | [decode_ways](<src/decode_ways.cpp>)             |       |       |
| [140. Word Break II](<https://leetcode.com/problems/word-break-ii/>)                                                                     | [word_break_ii](<src/word_break_ii.cpp>)         |       |       |
| [391. Perfect Rectangle](<https://leetcode.com/problems/perfect-rectangle/>)                                                             | [perf_rect](<src/perf_rect.cpp>)                 |       |       |
| [68. Text Justification](<https://leetcode.com/problems/text-justification/>)                                                            | [text_just](<src/text_just.cpp>)                 |       |       |
| [214. Shortest Palindrome](<https://leetcode.com/problems/shortest-palindrome/>)                                                         | [short_palin](<src/short_palin.cpp>)             |       |       |
| [30. Substring with Concatenation of All Words](<https://leetcode.com/problems/substring-with-concatenation-of-all-words/>)              | [substr_cat](<src/substr_cat.cpp>)               | 149ms |   75% |
| [174. Dungeon Game](<https://leetcode.com/problems/dungeon-game/>)                                                                       | [dungeon](<src/dungeon.cpp>)                     |       |       |
| [15. 3Sum](<https://leetcode.com/problems/3sum/>)                                                                                        | [3sum](<src/3sum.cpp>)                           |  49ms |   70% |
| [130. Surrounded Regions](<https://leetcode.com/problems/surrounded-regions/>)                                                           | [surr_region](<src/surr_region.cpp>)             |  49ms |    8% |
| [76. Minimum Window Substring](<https://leetcode.com/problems/minimum-window-substring/>)                                                | [min_window](<src/min_window.cpp>)               |   9ms |   86% |
| [335. Self Crossing](<https://leetcode.com/problems/self-crossing/>)                                                                     | [self_cross](<src/self_cross.cpp>)               |   0ms |       |
| [165. Compare Version Numbers](<https://leetcode.com/problems/compare-version-numbers/>)                                                 | [version_cmp](<src/version_cmp.cpp>)             |   0ms |       |
| [307. Range Sum Query - Mutable](<https://leetcode.com/problems/range-sum-query-mutable/>)                                               | [range_sum_mut](<src/range_sum_mut.cpp>)         |  52ms |   63% |
| [179. Largest Number](<https://leetcode.com/problems/largest-number/>)                                                                   | [largest_num](<src/largest_num.cpp>)             |   3ms |   94% |
| [211. Add and Search Word - Data structure design](<https://leetcode.com/problems/add-and-search-word-data-structure-design/>)           | [add_n_search](<src/add_n_search.cpp>)           |  69ms |   96% |
| [98. Validate Binary Search Tree](<https://leetcode.com/problems/validate-binary-search-tree/>)                                          | [valid_bst](<src/valid_bst.cpp>)                 |  13ms |   40% |
| [189. Rotate Array](<https://leetcode.com/problems/rotate-array/>)                                                                       | [rotate_array](<src/rotate_array.cpp>)           |  26ms |   31% |
| [32. Longest Valid Parentheses](<https://leetcode.com/problems/longest-valid-parentheses/>)                                              | [longest_par](<src/longest_par.cpp>)             |  12ms |   22% |
| [304. Range Sum Query 2D - Immutable](<https://leetcode.com/problems/range-sum-query-2d-immutable/>)                                     | [range_sum_2d_immu](<src/range_sum_2d_immu.cpp>) |  23ms |   47% |
| [10. Regular Expression Matching](<https://leetcode.com/problems/regular-expression-matching/>)                                          | [regex](<src/regex.cpp>)                         |  36ms |   43% |
| [390. Elimination Game](<https://leetcode.com/problems/elimination-game/>)                                                               | [elim_game](<src/elim_game.cpp>)                 |  29ms |   62% |
| [44. Wildcard Matching](<https://leetcode.com/problems/wildcard-matching/>)                                                              | [wildcard_dp](<src/wildcard_dp.cpp>)             | 119ms |   48% |
| [402. Remove K Digits](<https://leetcode.com/problems/remove-k-digits/>)                                                                 | [remove_k](<src/remove_k.cpp>)                   |   6ms |   56% |
| [18. 4Sum](<https://leetcode.com/problems/4sum/>)                                                                                        | [4sum](<src/4sum.cpp>)                           |  16ms |   86% |
| [385. Mini Parser](<https://leetcode.com/problems/mini-parser/>)                                                                         | [parser](<src/parser.cpp>)                       |  22ms |   99% |
| [220. Contains Duplicate III](<https://leetcode.com/problems/contains-duplicate-iii/>)                                                   | [contain_dup_iii](<src/contain_dup_iii.cpp>)     |  19ms |   92% |
| [295. Find Median from Data Stream](<https://leetcode.com/problems/find-median-from-data-stream/>)                                       | [median_stream](<src/median_stream.cpp>)         | 109ms |   97% |
| [135. Candy](<https://leetcode.com/problems/candy/>)                                                                                     | [candy](<src/candy.cpp>)                         |  42ms |   31% |
| [188. Best Time to Buy and Sell Stock IV](<https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv/>)                           | [stock_iv](<src/stock_iv.cpp>)                   |   9ms |   42% |
| [97. Interleaving String](<https://leetcode.com/problems/interleaving-string/>)                                                          | [interleave](<src/interleave.cpp>)               |   3ms |   60% |
| [124. Binary Tree Maximum Path Sum](<https://leetcode.com/problems/binary-tree-maximum-path-sum/>)                                       | [tree_path_sum](<src/tree_path_sum.cpp>)         |  29ms |   54% |
| [41. First Missing Positive](<https://leetcode.com/problems/first-missing-positive/>)                                                    | [first_missing](<src/first_missing.cpp>)         |   3ms |   19% |
| [23. Merge k Sorted Lists](<https://leetcode.com/problems/merge-k-sorted-lists/>)                                                        | [merge_k](<src/merge_k.cpp>)                     |  42ms |   57% |
| [85. Maximal Rectangle](<https://leetcode.com/problems/maximal-rectangle/>)                                                              | [max_rect](<src/max_rect.cpp>)                   |  49ms |    8% |
| [287. Find the Duplicate Number](<https://leetcode.com/problems/find-the-duplicate-number/>)                                             | [dup_num](<src/dup_num.cpp>)                     |  22ms |   28% |
| [218. The Skyline Problem](<https://leetcode.com/problems/the-skyline-problem/>)                                                         | [skyline](<src/skyline.cpp>)                     |  63ms |   62% |
| [71. Simplify Path](<https://leetcode.com/problems/simplify-path/>)                                                                      | [simp_path](<src/simp_path.cpp>)                 |   6ms |   50% |
| [138. Copy List with Random Pointer](<https://leetcode.com/problems/copy-list-with-random-pointer/>)                                     | [copy_list](<src/copy_list.cpp>)                 |  99ms |   97% |
| [233. Number of Digit One](<https://leetcode.com/problems/number-of-digit-one/>)                                                         | [count_one](<src/count_one.cpp>)                 |   0ms |       |
| [133. Clone Graph](<https://leetcode.com/problems/clone-graph/>)                                                                         | [clone_graph](<src/clone_graph.cpp>)             |  69ms |   88% |
| [381. Insert Delete GetRandom O(1) - Duplicates allowed](<https://leetcode.com/problems/insert-delete-getrandom-o1-duplicates-allowed/>) | [hash_dup](<src/hash_dup.cpp>)                   |  99ms |   62% |
| [210. Course Schedule II](<https://leetcode.com/problems/course-schedule-ii/>)                                                           | [course_ii](<src/course_ii.cpp>)                 |  23ms |   82% |
| [45. Jump Game II](<https://leetcode.com/problems/jump-game-ii/>)                                                                        | [jump_ii](<src/jump_ii.cpp>)                     |  12ms |   88% |
| [151. Reverse Words in a String](<https://leetcode.com/problems/reverse-words-in-a-string/>)                                             | [rev_word](<src/rev_word.cpp>)                   |   6ms |   67% |
| [457. Circular Array Loop](<https://leetcode.com/problems/circular-array-loop/>)                                                         | [circ_arr_loop](<src/circ_arr_loop.cpp>)         |   0ms |       |

