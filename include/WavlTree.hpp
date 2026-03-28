#include <utility>
#include <iostream>

#define NODE_TYPE(a, b) std::pair<int, int>{a, b}

namespace WavlTree {

    enum NodeType {

    };
    template<typename T>
    class WavlTree {
        private:
            class Node {
                public:
                    int key;
                    Node* leftChild;
                    Node* rightChild;
                    T value;
                    int rank;
                    NodeType nodeType;

                    Node(int k, T v, int r) : key(k), value(v), rank(r), leftChild(nullptr), rightChild(nullptr) {}    
            };
            Node* root;

            // private user functions
            Node* _insertValue(Node* node, Node* newNode);
            Node* _deleteValue(Node* node, int key);
            Node* _findMax(Node* root);
            Node* _findMin(Node* root);
            std::pair<T, int> _findValue(Node* node, int key);
            bool _containsKey(Node* node, int key);

            // utility functions
            Node* rotateLeft(Node* x);
            Node* rotateRight(Node* y);
            std::pair<int, int> calculateNodeType(Node* node);

        public:
            WavlTree();
            void insertValue(T value, int key);
            void deleteValue(int key);
            
            std::pair<T, int> findMax();
            std::pair<T, int> findMin();
            std::pair<T, int> findValue(int key);
            bool containsKey(int key);
    };

    template<typename T>
    WavlTree<T>::WavlTree() {
        root = nullptr;
    }

    // utility functions 

    template<typename T>
    std::pair<int, int> WavlTree<T>::calculateNodeType(Node* node) {
        int left;
        int right;
        if (node->leftChild) {
            left = node->rank - node->leftChild->rank;
        }
        else {
            left = node->rank + 1;
        }

        if (node->rightChild) {
            right = node->rank - node->rightChild->rank;
        }
        else {
            right = node->rank + 1;
        }

        return {left, right};
    }

    template<typename T>
    typename WavlTree<T>::Node* WavlTree<T>::rotateLeft(Node* x)
    {
        Node* y = x->rightChild;
        Node* b = y->leftChild;

        x->rightChild = b;
        y->leftChild = x;

        return y;
    }

    template<typename T>
    typename WavlTree<T>::Node* WavlTree<T>::rotateRight(Node* y)
    {
        Node* x = y->leftChild;
        Node* b = x->rightChild;

        y->leftChild = b;
        x->rightChild = y;

        return x;
    }




    // public interface for the private functions

    template<typename T>
    std::pair<T, int> WavlTree<T>::findMax() {
        Node* max = _findMax(root);
        return {max->value, max->key};
    }

    template<typename T>
    std::pair<T, int> WavlTree<T>::findMin() {
        Node* min = _findMin(root);
        return {min->value, min->key};
    }

    template<typename T>
    std::pair<T, int> WavlTree<T>::findValue(int key) {
        return _findValue(root, key);
    }

    template<typename T>
    bool WavlTree<T>::containsKey(int key) {
        return _containsKey(root, key);
    }
    
    template<typename T>
    void WavlTree<T>::insertValue(T value, int key) {
        Node* newNode = new Node(key, value, 0);
        if (containsKey(key)) {
            return;
        }
        root = _insertValue(root, newNode);
    }

    template<typename T>
    void WavlTree<T>::deleteValue(int key) {
        if (containsKey(key)) {
            return;
        }
        root = _deleteValue(root, key);
    }

    // private functions

    template<typename T>
    typename WavlTree<T>::Node* WavlTree<T>::_findMax(Node* node) {
        Node* max = node;
        while (max->rightChild != nullptr) {
            max = max->rightChild;
        }
        return max;
    }

    template<typename T>
    typename WavlTree<T>::Node* WavlTree<T>::_findMin(Node* node) {
        Node* min = node;
        while (min->leftChild != nullptr) {
            min = min->leftChild;
        }
        return min;
    }

    template<typename T>
    std::pair<T, int> WavlTree<T>::_findValue(Node* node, int key) {
        while (node) {
            if (key == node->key) {
                return {node->value, node->key};
            }
            if (key > node->key) {
                node = node->rightChild;
            }
            else if (key < node->key) {
                node = node->leftChild;
            }
        }
        return {NULL, NULL};
    }

    template<typename T>
    bool WavlTree<T>::_containsKey(Node* node, int key) {
        while (node) {
            if (key == node->key) {
                return true;
            }
            if (key > node->key) {
                node = node->rightChild;
            }
            else if (key < node->key) {
                node = node->leftChild;
            }
        }
        return false;
    }


    template<typename T>
    typename WavlTree<T>::Node* WavlTree<T>::_insertValue(Node* node, Node* newNode) {
        if (node == nullptr) {
            return newNode;
        }

        if (newNode->key > node->key) {
            node->rightChild = _insertValue(node->rightChild, newNode);
        }
        else if (newNode->key < node->key){
            node->leftChild = _insertValue(node->leftChild, newNode);
        }

        // rebalancing after insert

        std::pair<int,int> nodeType = calculateNodeType(node);

        if (nodeType == NODE_TYPE(1, 0) || nodeType == NODE_TYPE(0,1)) {
            node->rank++;
        }
        else if (nodeType == NODE_TYPE(0, 2)) {
            std::pair<int, int> zeroChildNodeType = calculateNodeType(node->leftChild);

            if (zeroChildNodeType == NODE_TYPE(1, 1) || zeroChildNodeType == NODE_TYPE(1, 2)) {
                node->rank--;
                node = rotateRight(node);
            }
            else if (zeroChildNodeType == NODE_TYPE(2, 1)) {
                node->leftChild = rotateLeft(node->leftChild);
                node = rotateRight(node);
                node->rank++;
                node->leftChild->rank--;
                node->rightChild->rank--;
            }
        }                
        else if (nodeType == NODE_TYPE(2, 0)) {
            std::pair<int, int> zeroChildNodeType = calculateNodeType(node->rightChild);

            if (zeroChildNodeType == NODE_TYPE(1, 1) || zeroChildNodeType == NODE_TYPE(2, 1)) {
                node->rank--;
                node = rotateLeft( node);
            }
            else if (zeroChildNodeType == NODE_TYPE(1, 2)) {
                node->rightChild = rotateRight(node->rightChild);
                node = rotateLeft(node);
                node->rank++;
                node->leftChild->rank--;
                node->rightChild->rank--;
            }
        }
        return node;
    };

    template<typename T>
    typename WavlTree<T>::Node* WavlTree<T>::_deleteValue(Node* node, int key) {

        if (node == nullptr) {
            return nullptr;
        }

        if (key > node->key) {
            node->rightChild = _deleteValue(node->rightChild, key);
        }
        else if (key < node->key) {
             node->leftChild = _deleteValue(node->leftChild, key);
        }
        else {
            if (node->leftChild == nullptr && node->rightChild == nullptr) {
                delete node;
                return nullptr;
            }
            else if (node->leftChild == nullptr) {
                Node* right = node->rightChild;
                delete node;
                return right;
            }
            else if (node->rightChild == nullptr) {
                Node* left = node->leftChild;
                delete node;
                return left;
            }
            Node* successor = _findMax(node->leftChild);

            node->key = successor->key;
            node->value = successor->value;

            node->leftChild = _deleteValue(node->leftChild, successor->key);
        }

        // rebalancing after delete

        std::pair<int,int> nodeType = calculateNodeType(node);


        // demote
        if (nodeType == NODE_TYPE(3, 2) || nodeType == NODE_TYPE(2, 3)) {
            node->rank--;
        }
        else if (nodeType == NODE_TYPE(2, 2) && node->leftChild == nullptr && node->rightChild == nullptr) {
            node->rank--;
        }
        else if (nodeType == NODE_TYPE(3, 1)) {
            std::pair<int,int> oneChildNodeType = calculateNodeType(node->rightChild);
            
            if (oneChildNodeType == NODE_TYPE(2, 2)) {
                node->rank--;
                node->rightChild->rank--;
            }
            else if (oneChildNodeType == NODE_TYPE(1, 1) || oneChildNodeType == NODE_TYPE(2, 1)) {
                node = rotateLeft(node);
                node->leftChild->rank--;
                node->rank++;
            }
            else {
                node->rightChild = rotateRight(node->rightChild);
                node = rotateLeft(node);
                node->rank += 2;
                node->leftChild->rank -= 2;
                node->rightChild->rank--;
            }
        }
        else if (nodeType == NODE_TYPE(1, 3)) {
            std::pair<int,int> oneChildNodeType = calculateNodeType(node->leftChild);

            if (oneChildNodeType == NODE_TYPE(2, 2)) {
                node->rank--;
                node->leftChild->rank--;
            }
            else if (oneChildNodeType == NODE_TYPE(1, 1) || oneChildNodeType == NODE_TYPE(1, 2)) {
                node = rotateRight(node);
                node->rightChild->rank--;
                node->rank++;
            }
            else {
                node->leftChild = rotateLeft(node->leftChild);
                node = rotateRight(node);
                node->rank += 2;
                node->rightChild->rank -= 2;
                node->leftChild->rank--;
            }
        }

        return node;
    }
}