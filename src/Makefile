CXX = clang++
CXXFLAGS = -std=c++17 -march=native -O3 
OUTPUT_DIR = bin


all: $(OUTPUT_DIR) discosum discotool

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

discosum: discohash.cpp discosum.cpp  
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/discosum discosum.cpp discohash.cpp

discotool: discohash.cpp discotool.cpp 
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/discotool discotool.cpp discohash.cpp

clean:
	rm -rf $(OUTPUT_DIR)
