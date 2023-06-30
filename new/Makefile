CXX = clang++
CXXFLAGS = -std=c++17 -march=native -O3 
OUTPUT_DIR = bin


all: $(OUTPUT_DIR) gen els ngen

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

gen: gen.cpp 
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/gen gen.cpp

els: els.cpp 
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/els els.cpp

ngen: ngen.cpp 
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/ngen ngen.cpp

clean:
	rm -rf $(OUTPUT_DIR)
