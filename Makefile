
CXXFLAGS += -Wall -g -std=gnu++11 -DRUN_TEST=1

.PHONY : all clean
.PRECIOUS: build/%	# This will tell make that intermediate files are also precious

LEETCODES = perm_seq word_ladder_ii max_points lru_cache div_int frac2dec \
            wildcard atoi valid_num 2array_median word_search_ii decode_ways \
            word_break_ii perf_rect text_just short_palin substr_cat dungeon \
            3sum surr_region min_window self_cross version_cmp range_sum_mut \
            largest_num add_n_search valid_bst rotate_array longest_par \
            range_sum_2d_immu

all: $(LEETCODES)

clean:
	rm -rf ./build/*

%: build/%
	[[ -n "$(TEST)" ]] && ./build/$* -d yes -s || :

build/%: src/%.cpp
	$(CXX) $(CXXFLAGS) -o build/$* $<

src/%.cpp: src/catch.hpp
