
CXXFLAGS += -Wall -g -std=gnu++11 -DRUN_TEST=1

.PHONY : all clean
.PRECIOUS: build/%	# This will tell make that intermediate files are also precious

all: perm_seq word_ladder max_points lru_cache div_int frac2dec wildcard atoi valid_num 2array_median

clean:
	rm -rf ./build/*

%: build/%
	[[ -n "$(TEST)" ]] && ./build/$* -d yes -s || :

build/%: src/%.cpp
	$(CXX) $(CXXFLAGS) -o build/$* $<
