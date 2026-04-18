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

I've tested my implementation with various types of datasets that represent some the worst cases for datas structures to deal with. 
With each dataset I had the structures perform the following operations in this exact order.
- insert
- lookup
- remove
- remove and right after insert another element

### Datasets
- sorted elements
- sorted elements divided into blocks that were then randomly shuffled - only insertion
- sorted elements divided into blocks that were then randomly shuffled - both insertion and lookup/removalx
- sorted elements divided into blocks and elements in each block were randomly shuffled - only insertion
- sorted elements divided into blocks and elements in each block were randomly shuffled - both insertion and lookup/removal
- alternating elements - sorted elements alternate with `+/-` signs

I've also calculated the respective averages from the results, and the those averages are the results displayed below. There were no significant deviation in the measured time performance which means that the WAVL tree handled the situations as it was supposed to.
___
The tests were performed with 1 000 000 inserted elements, 500 000 removal/lookup operations and with blocks each containing 10 000 elements.
### Results
| Operation  |Container | Measured execution time (ms) |
| --- | --- | --- |
| `insertion` | `WavlTree`<br>`AVLTree`<br>`std::multiset` | 512<br>2600<br>**293** |
| `lookup` | `WavlTree`<br>`AVLTree`<br>`std::multiset` | **36**<br>38<br>74 |
| `removal` | `WavlTree`<br>`AVLTree`<br>`std::multiset` | 238<br>1137<br>**148** |
| `removal and insertion` | `WavlTree`<br>`AVLTree`<br>`std::multiset` | 584<br>2028<br>**306** |

As you can see, the WavlTree is almost as good as the STL `multiset` in performing the `insert` and `remove` operations. However in the `lookup` operations, it is significantly - more than two times - better than the `std::multiset` which makes it great for lookup intensive tasks.

## Library Design
The whole library is essentialy just a one file `WavlTree.hpp`. The reason behind this is the ease of use. You import one file and you have the data structure ready to use. I wanted the library to be contained in a header file and have the ability to work with custom data types, so thats why I'm using `template` throughout the library source code. The code for the library was written entirely by me, and I studied the WAVL tree data structure from the following sources:
___
[https://sidsen.azurewebsites.net/papers/rb-trees-talg.pdf](https://sidsen.azurewebsites.net/papers/rb-trees-talg.pdf) [https://dspace.cuni.cz/bitstream/handle/20.500.11956/127952/130308737.pdf?sequence=1](https://dspace.cuni.cz/bitstream/handle/20.500.11956/127952/130308737.pdf?sequence=1)  
[https://is.muni.cz/th/elt9h/thesis.pdf](https://is.muni.cz/th/elt9h/thesis.pdf).  
The AVL tree [implementation](https://github.com/KadirEmreOto/AVL-Tree/tree/master) that I used in my performance comparison.
___