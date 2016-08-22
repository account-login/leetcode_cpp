
CXXFLAGS += -Wall -g -std=gnu++11 -DRUN_TEST=1

.PHONY : all clean
.PRECIOUS: build/%	# This will tell make that intermediate files are also precious

all: perm_seq word_ladder max_points lru_cache div_int frac2dec wildcard

clean:
	rm -rf ./build/*

%: build/%
	

build/%: src/%.cpp
	$(CXX) $(CXXFLAGS) -o build/$* $<
