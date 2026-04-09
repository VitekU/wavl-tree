#include "../include/WavlTree.hpp"
#include "AvlTree.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include <set>
#include <vector>

using namespace std::chrono;

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    WavlTree::WavlTree<int> wavlTree;
    AVLTree<int> avlTree;
    std::multiset<int> multiset;

    const int insertCount = 1000000;
    const int removeCount = 500000;

    std::vector<int> randomData;
    for (int i = 0; i < insertCount; ++i) {
        randomData.push_back(i);
    } 

    std::vector<int> removeElements = randomData;
    std::vector<int> lookupElements = randomData;
    std::shuffle(randomData.begin(), removeElements.end(), gen);
    std::shuffle(removeElements.begin(), removeElements.end(), gen);
    std::shuffle(lookupElements.begin(), lookupElements.end(), gen);

    removeElements.resize(removeCount);
    lookupElements.resize(removeCount);
    // beginning of the benchmark here

    // wavl insertion time
    auto begWavlIn = high_resolution_clock::now();
    for (auto e : randomData) {
        wavlTree.insert(e, e);
    }
    auto endWavlIn = high_resolution_clock::now();

    // wavl lookup time
    auto begWavlLook = high_resolution_clock::now();
    for (auto e : randomData) {
        auto v = wavlTree.findValue(e);
    }
    auto endWavlLook = high_resolution_clock::now();

    // wavl removal time
    auto begWavlRm = high_resolution_clock::now();
    for (auto e : removeElements) {
        wavlTree.remove(e);
    }
    auto endWavlRm = high_resolution_clock::now();


    // avl insertion time
    auto begAvlIn = high_resolution_clock::now();
    for (auto e : randomData) {
        avlTree.insert(e);
    }
    auto endAvlIn = high_resolution_clock::now();

    // avl lookup time
    auto begAvlLook = high_resolution_clock::now();
    for (auto e : randomData) {
        avlTree.find(e);
    }
    auto endAvlLook = high_resolution_clock::now();

    // avl removal time
    auto begAvlRm = high_resolution_clock::now();
    for (auto e : removeElements) {
        avlTree.erase(e);
    }
    auto endAvlRm = high_resolution_clock::now();   
    
    // multiset insertion time
    auto begSetIn = high_resolution_clock::now();
    for (auto e : randomData) {
        multiset.insert(e);
    }
    auto endSetIn = high_resolution_clock::now();

    // multiset lookup time
    auto begSetLook = high_resolution_clock::now();
    for (auto e : randomData) {
        multiset.find(e);
    }
    auto endSetLook = high_resolution_clock::now();

    // multiset removal time
    auto begSetRm = high_resolution_clock::now();
    for (auto e : removeElements) {
        auto it = multiset.find(e);
        if (it != multiset.end()) {
            multiset.erase(it); 
        }
    }
    auto endSetRm = high_resolution_clock::now();

    std::cout << "WAVL insertion time is: " << duration_cast<milliseconds>(endWavlIn - begWavlIn).count() << "\n"; 
    std::cout << "AVL insertion time is: " << duration_cast<milliseconds>(endAvlIn - begAvlIn).count() << "\n";
    std::cout << "Multiset insertion time is: " << duration_cast<milliseconds>(endSetIn - begSetIn).count() << "\n";  
    std::cout << "\n";
    std::cout << "WAVL lookup time is: " << duration_cast<milliseconds>(endWavlLook - begWavlLook).count() << "\n"; 
    std::cout << "AVL lookup time is: " << duration_cast<milliseconds>(endAvlLook - begAvlLook).count() << "\n"; 
    std::cout << "Multiset lookup time is: " << duration_cast<milliseconds>(endSetLook - begSetLook).count() << "\n";
    std::cout << "\n";
    std::cout << "WAVL removal time is: " << duration_cast<milliseconds>(endWavlRm - begWavlRm).count() << "\n"; 
    std::cout << "AVL removal time is: " << duration_cast<milliseconds>(endAvlRm - begAvlRm).count() << "\n";
     std::cout << "Multiset removal time is: " << duration_cast<milliseconds>(endSetRm - begSetRm).count() << "\n";

    return 0;
}