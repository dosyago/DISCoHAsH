#include <iostream>
#include <bitset>
#include <cmath>
#include <map>
#include <vector>
#include <iomanip>

const uint64_t DEFAULT_PRIME = 13166748625691186689ULL;
const uint64_t DEFAULT_GENERATOR = 3180491373ULL;

int hamming_distance(uint64_t a, uint64_t b) {
    return std::bitset<64>(a ^ b).count();
}

int main(int argc, char* argv[]) {
    uint64_t P = (argc > 1) ? std::stoull(argv[1]) : DEFAULT_PRIME;
    uint64_t G = (argc > 2) ? std::stoull(argv[2]) : DEFAULT_GENERATOR;

    uint64_t state = 1;
    uint64_t prevState = 0;
    const int iterations = 1000000;
    int sumBitChanges = 0;
    int minBitChanges = 64, maxBitChanges = 0;
    uint64_t minState = 0, maxState = 0, minPrevState = 0, maxPrevState = 0;
    std::vector<int> bitChanges;

    std::map<int, int> histogram;

    for (int i = 1; i <= iterations; i++) {
        state = (state * G) % P;

        int bitsChanged = hamming_distance(state, prevState);
        histogram[bitsChanged]++;
        bitChanges.push_back(bitsChanged);

        if (bitsChanged < minBitChanges) {
            minBitChanges = bitsChanged;
            minState = state;
            minPrevState = prevState;
        }
        if (bitsChanged > maxBitChanges) {
            maxBitChanges = bitsChanged;
            maxState = state;
            maxPrevState = prevState;
        }

        sumBitChanges += bitsChanged;

        std::cout << std::setw(10) << std::setfill(' ') << i << "     ";
        std::cout << std::bitset<64>(state) << "     ";
        std::cout << std::setw(2) << bitsChanged << "     ";
        std::cout << std::fixed << std::setprecision(2) << "(" << (double)bitsChanged / 64 * 100 << "%)" << std::endl;

        prevState = state;
    }

    double average = static_cast<double>(sumBitChanges) / iterations;
    double sumOfSquaredDifferences = 0;
    for (auto &e : bitChanges) {
        sumOfSquaredDifferences += (e - average) * (e - average);
    }
    double stdev = std::sqrt(sumOfSquaredDifferences / bitChanges.size());

    std::cout << "\nHistogram:" << std::endl;
    for (auto &entry : histogram) {
        std::cout << std::setw(2) << entry.first << " bits: ";
        int barLength = entry.second / 5000;
        for (int i = 0; i < barLength; i++) std::cout << "#";
        if (barLength > 0) std::cout << " ";
        std::cout << "(" << entry.second << ")" << std::endl;
    }

    std::cout << "\nMin bits changed: " << minBitChanges << " in state transition " << std::bitset<64>(minPrevState) << " -> " << std::bitset<64>(minState) << std::endl;
    std::cout << "Max bits changed: " << maxBitChanges << " in state transition " << std::bitset<64>(maxPrevState) << " -> " << std::bitset<64>(maxState) << std::endl;

    std::cout << "\nAverage percentage of bits changed: " << average / 64 * 100 << "%";
    std::cout << " (" << average << " bits)" << std::endl;
    std::cout << "Standard deviation of bits changed: " << stdev << std::endl;

    return 0;
}

