CXXP = /usr/local/opt/llvm/bin/clang++ 
CXX = clang++
CXXFLAGS = -std=c++17 -march=native -O3 
OUTPUT_DIR = bin


all: $(OUTPUT_DIR) gen els ngen inv ava ava2 ava3 avasearch avsearch2 avsearch3

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

avsearch3: avsearch3.cpp
	$(CXXP) $(CXXFLAGS) -fopenmp -o $(OUTPUT_DIR)/avsearch3 avsearch3.cpp

avsearch2: avsearch2.cpp
	$(CXXP) $(CXXFLAGS) -fopenmp -o $(OUTPUT_DIR)/avsearch2 avsearch2.cpp

avasearch: avasearch.cpp
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/avasearch avasearch.cpp

ava3: ava3.cpp
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/ava3 ava3.cpp

ava2: ava2.cpp
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/ava2 ava2.cpp

ava: ava.cpp
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/ava ava.cpp

inv: inv.cpp
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/inv inv.cpp

gen: gen.cpp 
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/gen gen.cpp

els: els.cpp 
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/els els.cpp

ngen: ngen.cpp 
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/ngen ngen.cpp

clean:
	rm -rf $(OUTPUT_DIR)
