#include "../include/WavlTree.hpp"
#include "AvlTree.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include <set>
#include <vector>

using namespace std::chrono;


#define getMs(x, y) (duration_cast<milliseconds>(x - y).count())

struct TimeTest {
    int wavl;
    int avl;
    int multiset; 

    TimeTest(int w, int a, int m) : wavl(w), avl(a), multiset(m) {}

    TimeTest operator+(const TimeTest other) {
        return TimeTest(wavl + other.wavl, avl + other.avl, multiset + other.multiset);
    }

    TimeTest operator/(const int n) {
        return TimeTest(wavl / n, avl / n, multiset / n);
    }
};

WavlTree::WavlTree<int> wavlTree;
AVLTree<int> avlTree;
std::multiset<int> multiset;

TimeTest testInsert(std::vector<int> data) {
    auto begWavl = high_resolution_clock::now();
    for (auto e : data) {
        wavlTree.insert(e, e);
    }
    auto endWavl = high_resolution_clock::now();

    auto begAvl = high_resolution_clock::now();
    for (auto e : data) {
        avlTree.insert(e);
    }
    auto endAvl = high_resolution_clock::now();

    auto begSet = high_resolution_clock::now();
    for (auto e : data) {
        multiset.insert(e);
    }
    auto endSet = high_resolution_clock::now();

    return TimeTest(getMs(endWavl, begWavl), getMs(endAvl, begAvl), getMs(endSet, begSet));
}

TimeTest testLookup(std::vector<int> data) {
    auto begWavl = high_resolution_clock::now();
    for (auto e : data) {
        wavlTree.findValue(e);
    }
    auto endWavl = high_resolution_clock::now();

    auto begAvl = high_resolution_clock::now();
    for (auto e : data) {
        avlTree.find(e);
    }
    auto endAvl = high_resolution_clock::now();

    auto begSet = high_resolution_clock::now();
    for (auto e : data) {
        multiset.find(e);
    }
    auto endSet = high_resolution_clock::now();

    return TimeTest(getMs(endWavl, begWavl), getMs(endAvl, begAvl), getMs(endSet, begSet));
}

TimeTest testRemove(std::vector<int> data) {
    auto begWavl = high_resolution_clock::now();
    for (auto e : data) {
        wavlTree.remove(e);
    }
    auto endWavl = high_resolution_clock::now();

    auto begAvl = high_resolution_clock::now();
    for (auto e : data) {
        avlTree.erase(e);
    }
    auto endAvl = high_resolution_clock::now();

    auto begSet = high_resolution_clock::now();
    for (auto e : data) {
        multiset.erase(e);
    }
    auto endSet = high_resolution_clock::now();

    return TimeTest(getMs(endWavl, begWavl), getMs(endAvl, begAvl), getMs(endSet, begSet));
}

TimeTest testRemoveInsert(std::vector<int> removeData, std::vector<int> insertData) {
    auto begWavl = high_resolution_clock::now();
    for (int i = 0; i < removeData.size(); ++i) {
        wavlTree.remove(removeData[i]);
        wavlTree.insert(insertData[i], insertData[i]);
    }
    auto endWavl = high_resolution_clock::now();

    auto begAvl = high_resolution_clock::now();
    for (int i = 0; i < removeData.size(); ++i) {
        avlTree.erase(removeData[i]);
        avlTree.insert(insertData[i]);
    }
    auto endAvl = high_resolution_clock::now();

    auto begSet = high_resolution_clock::now();
    for (int i = 0; i < removeData.size(); ++i) {
        multiset.erase(removeData[i]);
        multiset.insert(insertData[i]);
    }
    auto endSet = high_resolution_clock::now();

    return TimeTest(getMs(endWavl, begWavl), getMs(endAvl, begAvl), getMs(endSet, begSet));
}

std::vector<int> sortedData(int n) {
    std::vector<int> out;
    for (int i = 0; i < n; ++i) {
        out.push_back(i);
    }
    return out;
} 

std::vector<int> alternatingData(int n) {
    std::vector<int> out;
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            out.push_back(i);
        }
        else {
            out.push_back(-i);
        }
    }
    return out;
} 

std::vector<int> shuffleEachSegment(std::vector<int> data, int n, std::mt19937 gen) {
    int segmentSize = data.size() / n;
    int remainder = data.size() % n;

    for (int i = 0; i < n; ++i) {
        std::shuffle(data.begin() + i*segmentSize, data.begin() + (i + 1)*segmentSize, gen);
    }

    if (remainder > 0) {
        std::shuffle(data.end() - remainder, data.end(), gen);
    }
    return data;
}

std::vector<int> shuffleSegments(std::vector<int> data, int n, std::mt19937 gen) {
    int segmentSize = data.size() / n;
    int remainder = data.size() % n;
    std::vector<std::pair<int, int>> blockIndices;
    
    for (int i = 0; i < n; ++i) {
        blockIndices.push_back({i*segmentSize, segmentSize});
    }

    if (remainder > 0) {
        blockIndices.push_back({data.size() - remainder, remainder});
    }

    std::shuffle(blockIndices.begin(), blockIndices.end(), gen);

    std::vector<int> out;

    for (auto block : blockIndices) {
        for (int i = block.first; i < block.first + block.second; ++i) {
            out.push_back(data[i]);
        }
    }
    return out;
}

void printResults(std::string operation, TimeTest result) {
    std::cout << "WAVL " << operation << " time is: " << result.wavl << "\n"; 
    std::cout << "AVL " << operation << " time is: " << result.avl << "\n"; 
    std::cout << "Multiset " << operation << " time is: " << result.multiset << "\n"; 
    std::cout << "-----------------------------\n";
}

void testWithData(std::vector<int> insertData, std::vector<int> lookupData, std::vector<int> removeData, TimeTest& insertAvg, TimeTest& lookupAvg, TimeTest& removeAvg, TimeTest& removeInsertAvg) {
    auto insertResult = testInsert(insertData);
    auto lookupResult = testLookup(lookupData);
    auto removeResult = testRemove(removeData);
    auto removeInsertResult = testRemoveInsert(lookupData, removeData);

    insertAvg = insertAvg + insertResult;
    lookupAvg = lookupAvg + lookupResult;
    removeAvg = removeAvg + removeResult;
    removeInsertAvg = removeInsertAvg + removeInsertResult;

    printResults("insert", insertResult);
    printResults("lookup", lookupResult);
    printResults("remove", removeResult);
    printResults("remove and insert", removeInsertResult);
    std::cout << "\n\n";

    wavlTree.clear();
    avlTree.clear();
    multiset.clear();
}


int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    const int insertCount = 1000000;
    const int lookupCount = 500000;
    const int removeCount = 500000;
    const int segmentSize = 10000;

    const int numberOfTests = 6;

    std::vector<int> randomData;
   
    std::vector<int> removeData;
    std::vector<int> lookupData;

    TimeTest insertAvg = TimeTest(0, 0, 0);
    TimeTest lookupAvg = TimeTest(0, 0, 0);
    TimeTest removeAvg = TimeTest(0, 0, 0);
    TimeTest removeInsertAvg = TimeTest(0, 0, 0);

    // beginning of the benchmark here

    // sorted data
    std::cout << "SORTED - ALL\n";
    std::cout << "-----------------------------\n";

    randomData = sortedData(insertCount);
    removeData = sortedData(removeCount);
    lookupData = sortedData(lookupCount);

    testWithData(randomData, lookupData, removeData, insertAvg, lookupAvg, removeAvg, removeInsertAvg);

    // shuffle segments on insert
    std::cout << "SHUFFLED BLOCKS - ONLY INSERT\n";
    std::cout << "-----------------------------\n";

    randomData = shuffleSegments(randomData, segmentSize, gen);

    testWithData(randomData, lookupData, removeData, insertAvg, lookupAvg, removeAvg, removeInsertAvg);

    // shuffle segments in all datasets
    std::cout << "SHUFFLED BLOCKS - ALL\n";
    std::cout << "-----------------------------\n";
    lookupData = shuffleSegments(lookupData, segmentSize, gen);
    removeData = shuffleSegments(removeData, segmentSize, gen);

    testWithData(randomData, lookupData, removeData, insertAvg, lookupAvg, removeAvg, removeInsertAvg);

    // shuffle each segment separately on insert
    std::cout << "SHUFFLED EACH BLOCK - ONLY INSERT\n";
    std::cout << "-----------------------------\n";
    randomData = shuffleEachSegment(sortedData(insertCount), segmentSize, gen);
    removeData = sortedData(removeCount);
    lookupData = sortedData(lookupCount);

    testWithData(randomData, lookupData, removeData, insertAvg, lookupAvg, removeAvg, removeInsertAvg);

    // shuffle each segment separately in all datasets
    std::cout << "SHUFFLED EACH BLOCK - ALL\n";
    std::cout << "-----------------------------\n";
    removeData = shuffleEachSegment(removeData, segmentSize, gen);
    lookupData = shuffleEachSegment(lookupData, segmentSize, gen);

    testWithData(randomData, lookupData, removeData, insertAvg, lookupAvg, removeAvg, removeInsertAvg);

    // alternating data
    std::cout << "ALTERNATE - ALL\n";
    std::cout << "-----------------------------\n";
    randomData = alternatingData(insertCount);
    removeData = alternatingData(removeCount);
    lookupData = alternatingData(lookupCount);

    testWithData(randomData, lookupData, removeData, insertAvg, lookupAvg, removeAvg, removeInsertAvg);

    // average times
    std::cout << "AVERAGE TIMES\n";
    std::cout << "-----------------------------\n";
    insertAvg = insertAvg / numberOfTests;
    lookupAvg = lookupAvg / numberOfTests;
    removeAvg = removeAvg / numberOfTests;
    removeInsertAvg = removeInsertAvg / numberOfTests;

    printResults("insert", insertAvg);
    printResults("lookup", lookupAvg);
    printResults("remove", removeAvg);
    printResults("remove and insert", removeInsertAvg);
    return 0;
}