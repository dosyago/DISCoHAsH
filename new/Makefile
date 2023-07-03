CXXP = /usr/local/opt/llvm/bin/clang++
CXX = clang++
CXXFLAGS = -std=c++17 -march=native -O3
OUTPUT_DIR = bin

all: $(OUTPUT_DIR) $(OUTPUT_DIR)/pg_els $(OUTPUT_DIR)/avsearch3 $(OUTPUT_DIR)/avsearch2 $(OUTPUT_DIR)/avasearch $(OUTPUT_DIR)/ava3 $(OUTPUT_DIR)/ava2 $(OUTPUT_DIR)/ava $(OUTPUT_DIR)/inv $(OUTPUT_DIR)/gen $(OUTPUT_DIR)/els $(OUTPUT_DIR)/ngen

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(OUTPUT_DIR)/pg_els: pg_els.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OUTPUT_DIR)/avsearch3: avsearch3.cpp
	$(CXXP) $(CXXFLAGS) -fopenmp -o $@ $<

$(OUTPUT_DIR)/avsearch2: avsearch2.cpp
	$(CXXP) $(CXXFLAGS) -fopenmp -o $@ $<

$(OUTPUT_DIR)/avasearch: avasearch.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OUTPUT_DIR)/ava3: ava3.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OUTPUT_DIR)/ava2: ava2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OUTPUT_DIR)/ava: ava.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OUTPUT_DIR)/inv: inv.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OUTPUT_DIR)/gen: gen.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OUTPUT_DIR)/els: els.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OUTPUT_DIR)/ngen: ngen.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -rf $(OUTPUT_DIR)

