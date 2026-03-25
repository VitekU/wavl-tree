#include <utility>
#include <iostream>
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

            // user functions
            Node* _insertValue(Node* node, Node* newNode);
            Node* _deleteValue(Node* node, int key);
            Node* _findMax(Node* root);
            void _printTree(Node* node);

            // utility functions
            Node* rotateLeft(Node* x);
            Node* rotateRight(Node* y);
            Node* _findSuccesor(Node* root);
            

            std::pair<int, int> calculateNodeType(Node* node);

            Node* root;
        public:
            WavlTree();
            void insertValue(T value, int key);
            void printTree();
            void deleteValue(int key);
            std::pair<int, int> findMax();
            
    };

    template<typename T>
    WavlTree<T>::WavlTree() {
        root = nullptr;
    }

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

    template<typename T>
    std::pair<int, int> WavlTree<T>::findMax() {
        Node* max = _findMax(root);
        return {max->value, max->key};
    }
    
    template<typename T>
    void WavlTree<T>::insertValue(T value, int key) {
        Node* newNode = new Node(key, value, 0);
        root = _insertValue(root, newNode);
    }

    template<typename T>
    void WavlTree<T>::deleteValue(int key) {
        root = _deleteValue(root, key);
    }

    template<typename T>
    void WavlTree<T>::printTree() {
        _printTree(root);
        std::cout << "Printed whole tree.\n";
    }

    template<typename T>
    void WavlTree<T>::_printTree(Node* node) {
        if (node == nullptr) {
            return;
        }
        
        _printTree(node->leftChild);
        _printTree(node->rightChild);
        std::cout << "key: " << node->key << "\n";
        std::cout << "rank: " << node->rank << "\n";
        if (node->leftChild)
            std::cout << "leftChild: " << node->leftChild->key << "\n";
        if (node->rightChild)
            std::cout << "rightChild: " << node->rightChild->key << "\n";
        std::cout << "-----------\n";
        return;
    }

    template<typename T>
    typename WavlTree<T>::Node* WavlTree<T>::_findMax(Node* node) {
        Node* max = node;
        while (max->rightChild != nullptr) {
            max = max->rightChild;
        }
        return max;
    }


    template<typename T>
    typename WavlTree<T>::Node* WavlTree<T>::_findSuccesor(Node* node) {
        return _findMax(node->leftChild);
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

        std::pair<int,int> nodeType = calculateNodeType(node);

        if (nodeType == std::pair<int, int>{1,0} || nodeType == std::pair<int, int>{0,1}) {
            node->rank++;
        }
        else if (nodeType == std::pair<int, int>{0,2}) {
            std::pair<int, int> zeroChildNodeType = calculateNodeType(node->leftChild);

            if (zeroChildNodeType == std::pair<int, int>{1,1} || zeroChildNodeType == std::pair<int, int>{1,2}) {
                node->rank--;
                node = rotateRight(node);
            }
            else if (zeroChildNodeType == std::pair<int, int>{2,1}) {
                node->leftChild = rotateLeft(node->leftChild);
                node = rotateRight(node);
                node->rank++;
                node->leftChild->rank--;
                node->rightChild->rank--;
            }
        }                
        else if (nodeType == std::pair<int, int>{2,0}) {
            std::pair<int, int> zeroChildNodeType = calculateNodeType(node->rightChild);

            if (zeroChildNodeType == std::pair<int, int>{1,1} || zeroChildNodeType == std::pair<int, int>{2,1} ) {
                node->rank--;
                node = rotateLeft( node);
            }
            else if (zeroChildNodeType == std::pair<int, int>{1,2}) {
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
        if (key > node->key) {
            _deleteValue(node->rightChild, key);
        }
        else if (key < node->key) {
            _deleteValue(node->leftChild, key);
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
        }
    }
}