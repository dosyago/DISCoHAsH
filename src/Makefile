CXX = clang++
CXXFLAGS = -std=c++17 -march=native -O3 
OUTPUT_DIR = bin


all: $(OUTPUT_DIR) discosum discotool cryptosum cryptotool

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

discosum: discohash.cpp discosum.cpp  
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/discosum discosum.cpp discohash.cpp

discotool: discohash.cpp discotool.cpp 
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/discotool discotool.cpp discohash.cpp

cryptosum: cryptohash.cpp cryptosum.cpp  
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/cryptosum cryptosum.cpp cryptohash.cpp

cryptotool: cryptohash.cpp cryptotool.cpp 
	$(CXX) $(CXXFLAGS) -o $(OUTPUT_DIR)/cryptotool cryptotool.cpp cryptohash.cpp

clean:
	rm -rf $(OUTPUT_DIR)
