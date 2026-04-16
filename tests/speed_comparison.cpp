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
    for (int i = 0; i < n; ++i) {
        std::shuffle(data.begin() + i*segmentSize, data.begin() + (i + 1)*segmentSize, gen);
    }
    int remainder = data.size() % n;

    if (remainder > 0) {
        std::shuffle(data.end() - remainder, data.end(), gen);
    }
    return data;
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

    const int insertCount = 100000;
    const int removeCount = 50000;

    std::vector<int> randomData;
    for (int i = 0; i < insertCount; ++i) {
        if (i % 2 == 0) {
            randomData.push_back(i);
        }
        else {
            randomData.push_back(-i);   
        }
    } 

    std::vector<int> removeElements = randomData;
    std::vector<int> lookupElements = randomData;
    //std::shuffle(randomData.begin(), removeElements.end(), gen);
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

    auto data = sortedData(27);
    data = shuffleEachSegment(data, 5, gen);
    for (auto e : data) {
        std::cout << e << " ";
    }
    std::cout << "\n";


    return 0;
}