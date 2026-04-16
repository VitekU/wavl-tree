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

struct timeTest {
    int wavl;
    int avl;
    int multiset; 

    timeTest(int w, int a, int m) : wavl(w), avl(a), multiset(m) {}
};

WavlTree::WavlTree<int> wavlTree;
AVLTree<int> avlTree;
std::multiset<int> multiset;

timeTest testInsert(std::vector<int> data) {
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

    return timeTest(getMs(endWavl, begWavl), getMs(endAvl, begAvl), getMs(endSet, begSet));
}

timeTest testLookup(std::vector<int> data) {
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

    return timeTest(getMs(endWavl, begWavl), getMs(endAvl, begAvl), getMs(endSet, begSet));
}

timeTest testRemove(std::vector<int> data) {
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

    return timeTest(getMs(endWavl, begWavl), getMs(endAvl, begAvl), getMs(endSet, begSet));
}

timeTest testRemoveInsert(std::vector<int> removeData, std::vector<int> insertData) {
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

    return timeTest(getMs(endWavl, begWavl), getMs(endAvl, begAvl), getMs(endSet, begSet));
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



void printResults(std::string operation, timeTest result) {
    std::cout << "WAVL " << operation << " time is: " << result.wavl << "\n"; 
    std::cout << "AVL " << operation << " time is: " << result.avl << "\n"; 
    std::cout << "Multiset " << operation << " time is: " << result.multiset << "\n"; 
    std::cout << "-----------------------------\n";
}


int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    const int insertCount = 1000000;
    const int removeCount = 500000;
    const int segmentSize = 10000;

    auto randomData = sortedData(insertCount);
    randomData = shuffleSegments(randomData, segmentSize, gen);

    std::vector<int> removeElements = randomData;
    std::vector<int> lookupElements = randomData;
    std::shuffle(removeElements.begin(), removeElements.end(), gen);
    std::shuffle(lookupElements.begin(), lookupElements.end(), gen);

    removeElements.resize(removeCount);
    lookupElements.resize(removeCount);
    // beginning of the benchmark here


    auto insertResult = testInsert(randomData);
    auto lookupResult = testLookup(lookupElements);
    auto removeResult = testRemove(removeElements);
    auto removeInsertReult = testRemoveInsert(removeElements, lookupElements);

    printResults("insert", insertResult);
    printResults("lookup", lookupResult);
    printResults("remove", removeResult);
    printResults("remove and insert", removeInsertReult);



    return 0;
}