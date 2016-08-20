
CXXFLAGS += -Wall -g -std=gnu++11

.PHONY : all clean
.PRECIOUS: build/%	# This will tell make that intermediate files are also precious

all: perm_seq word_ladder max_points lru_cache div_int

clean:
	rm -rf ./build/*

%: build/%
	

build/%: src/%.cpp
	$(CXX) $(CXXFLAGS) -o build/$* $<
