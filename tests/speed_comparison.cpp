#include "../include/WavlTree.hpp"
#include "AvlTree.h"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>

using namespace std::chrono;

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100000);

    WavlTree::WavlTree<int> wavlTree;
    AVLTree<int> avlTree;

    const int insertCount = 10000;
    const int removeCount = 5000;

    std::vector<int> randomData;
    for (int i = 0; i < insertCount; ++i) {
        randomData.push_back(dis(gen));
    } 

    std::vector<int> removeElements = randomData;
    std::shuffle(removeElements.begin(), removeElements.end(), gen);
    removeElements.resize(removeCount);
    // beginning of the benchmark here

    // wavl insertion time
    auto begWavlIn = high_resolution_clock::now();
    for (auto e : randomData) {
        wavlTree.insert(e, e);
    }
    auto endWavlIn = high_resolution_clock::now();

    // wavl removal time
    auto begWavlRm = high_resolution_clock::now();
    for (auto e : removeElements) {
        wavlTree.remove(e);
    }
    auto endWavlRm = high_resolution_clock::now();


    // wavl insertion time
    auto begAvlIn = high_resolution_clock::now();
    for (auto e : randomData) {
        avlTree.insert(e);
    }
    auto endAvlIn = high_resolution_clock::now();

    // wavl removal time
    auto begAvlRm = high_resolution_clock::now();
    for (auto e : removeElements) {
        avlTree.erase(e);
    }
    auto endAvlRm = high_resolution_clock::now();    

    std::cout << "WAVL insertion time is: " << duration_cast<microseconds>(endWavlIn - begWavlIn).count() << "\n"; 
    std::cout << "AVL insertion time is: " << duration_cast<microseconds>(endAvlIn - begAvlIn).count() << "\n"; 
    std::cout << "\n";
    std::cout << "WAVL removal time is: " << duration_cast<microseconds>(endWavlRm - begWavlRm).count() << "\n"; 
    std::cout << "AVL removal time is: " << duration_cast<microseconds>(endAvlRm - begAvlRm).count() << "\n";

    return 0;
}