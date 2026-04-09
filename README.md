# WAVL Tree Implementation

## Overview 
This is my implementation of the weak AVL tree data structure that was proposed by [Hauepler, Sen and Tarjan](https://sidsen.azurewebsites.net/papers/rb-trees-talg.pdf) in 2015.

## Usage 
The library is contained in a single file `WavlTree.hpp`, which includes the whole functionality of the structure. To use it, copy the file into your `include` or similar and then use it in the following way:
```
#include "include/WavlTree.hpp"

int main() {
    WavlTree::WavlTree<int> tree;
    return 0;
}
```
*The running examples are contained int the `tests/main.cpp` file.*

My implementation supports storing custom data types in the tree, however each value is associated with a key that you have to provide upon insertion. This key is then used for accessing and removing the corresponding values.   

### Features
| Function | Definition | Time Complexity |
| --- | --- | --- |
| `WavlTree<class T> tree;` | declaration of tree object | constant |
| `insert(T value, int key)` | insert an element to tree | O(logN) |
| `remove(int key)` | remove a value corresponding to the key from the tree | O(logN) |
| `find(int key)` | return `std::optional` containing the value correspoding to the key | O(logN) |
| `finMax()` | return `std::optional` containing the value with the maximum key | O(logN) |
| `finMin()` | return  `std::optional` containing thevalue with the minimum key | O(logN) |
| `clear()` | clear the tree | N |
| `size()` | return size of the tree | constant |
| `postOrderMaxHeight` | return the maximum height of the tree found by postorder traversal (*mainly used for debugging*) | O(N)

## Speed Comparison

This tree was proposed as an alternative with better performance in some scenarios then other self balancing trees, such as AVL or RedBlack trees.

I've decided to test my implementation against [this](https://github.com/KadirEmreOto/AVL-Tree/tree/master) implementation of and AVL tree and C++ STL implementation of the Red Black tree, `std::multiset`. The tests for the speed comparison are contained in the file `tests/speed_comparison`.
___
The tests were performed with 1 000 000 inserted elements and 500 000 removal and lookup operations.
### Results
| Operation  |Container | Measured execution time (ms) |
| --- | --- | --- |
| `insertion` | `WavlTree`<br>`AVLTree`<br>`std::multiset` | 497<br>2531<br><span style="color: green;">306</span> |
| `removal` | `WavlTree`<br>`AVLTree`<br>`std::multiset` | <span style="color: green;">68</span><br>81<br>148 |
| `lookup` | `WavlTree`<br>`AVLTree`<br>`std::multiset` | 493<br>1415<br><span style="color: green;">374</span> |

As you can see, the WavlTree is almost as good as the STL `multiset` in performing the `insert` and `remove` operations. However in the `lookup` operations, it is significantly - more than two times - better than the `std::multiset` which makes it great for lookup intensive tasks.

## Library Design
The whole library is essentialy just a one file `WavlTree.hpp`. The reason behind this is the ease of use. You import one file and you have the data structure ready to use. I wanted the library to be contained in a header file and have the ability to work with custom data types, so thats why I'm using `template` throughout the library source code. The code for the library was written entirely by me, and I studied the WAVL tree data structure from the following sources:
___
[https://sidsen.azurewebsites.net/papers/rb-trees-talg.pdf](https://sidsen.azurewebsites.net/papers/rb-trees-talg.pdf) [https://dspace.cuni.cz/bitstream/handle/20.500.11956/127952/130308737.pdf?sequence=1](https://dspace.cuni.cz/bitstream/handle/20.500.11956/127952/130308737.pdf?sequence=1)  
[https://is.muni.cz/th/elt9h/thesis.pdf](https://is.muni.cz/th/elt9h/thesis.pdf).  
The AVL tree [implementation](https://github.com/KadirEmreOto/AVL-Tree/tree/master) that I used in my performance comparison.
___
