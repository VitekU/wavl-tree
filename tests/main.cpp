#include "../include/WavlTree.hpp"
#include <iostream>


// struct for demonstration purposes

struct Person {
    int age;
    std::string name;

    Person(int a, std::string n) : age(a), name(n) {}
};

int main() {
    WavlTree::WavlTree<Person> tree;

    tree.insert(Person(10, "Alice"), 10);
    tree.insert(Person(17, "Bob"), 17);
    tree.insert(Person(6, "Cecilie"), 6);
    tree.insert(Person(54, "David"), 54);
    tree.insert(Person(28, "Eda"), 26);
    tree.insert(Person(35, "Filip"), 35);


    // checking for keys
    int key = 17;
    std::cout << (tree.containsKey(key) ? "Tree does contain a value with the key " : "Tree does not contain a value with the key ") << key << ".\n";

    // removing a value by its key
    tree.remove(key);
    
    std::cout << (tree.containsKey(key) ? "Tree does contain a value with the key " : "Tree does not contain a value with the key ") << key << ".\n";

    // finding a value by its key
    key = 35;
    auto p = tree.findValue(key);
    if (p.has_value()) {
        std::cout << p.value().age << " " << p.value().name << "\n";
    }

    // finding max
    auto max = tree.findMax();
    if (max.has_value()) {
        std::cout << max.value().age << " " << max.value().name << "\n";
    }
    
    // find min
    auto min = tree.findMin();
    if (max.has_value()) {
        std::cout << min.value().age << " " << min.value().name << "\n";
    }
    
    // removing a value by its key
    key = 54;
    tree.remove(key);

    // inserting a new value with its key
    tree.insert(Person(100, "Gabriel"), 100);
    
    // find max again
    max = tree.findMax();
    if (max.has_value()) {
        std::cout << max.value().age << " " << max.value().name << "\n";
    }

    return 0;
}