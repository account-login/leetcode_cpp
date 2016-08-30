
CXXFLAGS += -Wall -g -std=gnu++11 -DRUN_TEST=1

.PHONY : all clean
.PRECIOUS: build/%	# This will tell make that intermediate files are also precious

LEETCODES = perm_seq word_ladder max_points lru_cache div_int frac2dec wildcard \
            atoi valid_num 2array_median word_search_ii decode_ways word_break_ii \
            perf_rect

all: $(LEETCODES)

clean:
	rm -rf ./build/*

%: build/%
	[[ -n "$(TEST)" ]] && ./build/$* -d yes -s || :

build/%: src/%.cpp
	$(CXX) $(CXXFLAGS) -o build/$* $<
