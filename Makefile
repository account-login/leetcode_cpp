
CXXFLAGS += -Wall -Wextra -pedantic -g -std=c++11 -DRUN_TEST=1

.PHONY : all clean
.PRECIOUS: build/%	# This will tell make that intermediate files are also precious

LEETCODES = perm_seq word_ladder_ii max_points lru_cache div_int frac2dec \
            wildcard atoi valid_num 2array_median word_search_ii decode_ways \
            word_break_ii perf_rect text_just short_palin substr_cat dungeon \
            3sum surr_region min_window self_cross version_cmp range_sum_mut \
            largest_num add_n_search valid_bst rotate_array longest_par \
            range_sum_2d_immu regex elim_game wildcard_dp remove_k 4sum parser \
            contain_dup_iii median_stream candy stock_iv interleave \
            tree_path_sum first_missing merge_k max_rect dup_num skyline \
            simp_path copy_list count_one clone_graph hash_dup course_ii \
            jump_ii rev_word circ_arr_loop lfu_cache count_rep rev_pairs \
            count_range_sum sub_arr_sum max_product tag_validator

all: $(LEETCODES)

clean:
	rm -rf ./build/*

%: build/%
	[[ -n "$(TEST)" ]] && ./build/$* -d yes -s || :

build/%: src/%.cpp build/catch_main.o src/stringify.h
	$(CXX) $(CXXFLAGS) -o build/$* src/$*.cpp build/catch_main.o

build/catch_main.o: src/catch.hpp
	$(CXX) $(CXXFLAGS) -DCATCH_CONFIG_MAIN -xc++ -c -o build/catch_main.o $^

src/%.cpp: src/catch.hpp
