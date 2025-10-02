#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>

template<typename T>
class FibonacciHeap {
private:
    struct Node {
        T key;
        int degree;
        bool marked;
        Node* parent;
        Node* child;
        Node* left;
        Node* right;

        Node(T k) : key(k), degree(0), marked(false), 
                   parent(nullptr), child(nullptr), 
                   left(this), right(this) {}
    };

    Node* minNode;
    int count;

    void linkNodes(Node* child, Node* parent) {
        // Remove child from root list
        child->left->right = child->right;
        child->right->left = child->left;

        // Make child a child of parent
        child->parent = parent;
        if (parent->child == nullptr) {
            parent->child = child;
            child->left = child;
            child->right = child;
        } else {
            child->left = parent->child;
            child->right = parent->child->right;
            parent->child->right->left = child;
            parent->child->right = child;
        }
        parent->degree++;
        child->marked = false;
    }

    void consolidate() {
        if (minNode == nullptr) return;

        int maxDegree = static_cast<int>(std::log(count) / std::log(1.618)) + 1;
        std::vector<Node*> degreeTable(maxDegree, nullptr);

        std::vector<Node*> roots;
        Node* current = minNode;
        do {
            roots.push_back(current);
            current = current->right;
        } while (current != minNode);

        for (Node* node : roots) {
            int degree = node->degree;
            while (degreeTable[degree] != nullptr) {
                Node* other = degreeTable[degree];
                if (node->key > other->key) {
                    std::swap(node, other);
                }
                linkNodes(other, node);
                degreeTable[degree] = nullptr;
                degree++;
                if (degree >= maxDegree) {
                    degreeTable.resize(degree + 1, nullptr);
                }
            }
            degreeTable[degree] = node;
        }

        // Rebuild root list and find new min
        minNode = nullptr;
        for (Node* node : degreeTable) {
            if (node != nullptr) {
                if (minNode == nullptr) {
                    minNode = node;
                    node->left = node;
                    node->right = node;
                } else {
                    // Add to root list
                    node->left = minNode;
                    node->right = minNode->right;
                    minNode->right->left = node;
                    minNode->right = node;

                    if (node->key < minNode->key) {
                        minNode = node;
                    }
                }
            }
        }
    }

public:
    FibonacciHeap() : minNode(nullptr), count(0) {}

    ~FibonacciHeap() {
        clear();
    }

    void insert(T key) {
        Node* newNode = new Node(key);
        
        if (minNode == nullptr) {
            minNode = newNode;
        } else {
            // Add to root list
            newNode->left = minNode;
            newNode->right = minNode->right;
            minNode->right->left = newNode;
            minNode->right = newNode;

            if (key < minNode->key) {
                minNode = newNode;
            }
        }
        count++;
    }

    T getMin() {
        if (minNode == nullptr) {
            throw std::runtime_error("Heap is empty");
        }
        return minNode->key;
    }

    T extractMin() {
        if (minNode == nullptr) {
            throw std::runtime_error("Heap is empty");
        }

        Node* min = minNode;
        T minKey = min->key;

        // Add children to root list
        if (min->child != nullptr) {
            Node* child = min->child;
            do {
                Node* nextChild = child->right;
                child->parent = nullptr;
                
                // Add to root list
                child->left = minNode;
                child->right = minNode->right;
                minNode->right->left = child;
                minNode->right = child;
                
                child = nextChild;
            } while (child != min->child);
        }

        // Remove min from root list
        min->left->right = min->right;
        min->right->left = min->left;

        if (min == min->right) {
            minNode = nullptr;
        } else {
            minNode = min->right;
            consolidate();
        }

        delete min;
        count--;
        return minKey;
    }

    void merge(FibonacciHeap& other) {
        if (other.minNode == nullptr) return;

        if (minNode == nullptr) {
            minNode = other.minNode;
            count = other.count;
        } else {
            // Connect the two circular lists
            Node* thisRight = minNode->right;
            Node* otherLeft = other.minNode->left;

            minNode->right = other.minNode;
            other.minNode->left = minNode;
            thisRight->left = otherLeft;
            otherLeft->right = thisRight;

            if (other.minNode->key < minNode->key) {
                minNode = other.minNode;
            }
            count += other.count;
        }

        // Reset other heap
        other.minNode = nullptr;
        other.count = 0;
    }

    bool isEmpty() const {
        return minNode == nullptr;
    }

    int size() const {
        return count;
    }

    void clear() {
        while (!isEmpty()) {
            extractMin();
        }
    }
};

// Пример использования
int main() {
    FibonacciHeap<int> heap;
    
    heap.insert(5);
    heap.insert(3);
    heap.insert(8);
    heap.insert(1);
    heap.insert(10);

    std::cout << "Fibonacci Heap operations:" << std::endl;
    std::cout << "Min: " << heap.getMin() << std::endl;
    
    std::cout << "Extracting elements: ";
    while (!heap.isEmpty()) {
        std::cout << heap.extractMin() << " ";
    }
    std::cout << std::endl;

    // Тест объединения
    FibonacciHeap<int> heap1, heap2;
    heap1.insert(3);
    heap1.insert(7);
    heap2.insert(2);
    heap2.insert(5);

    heap1.merge(heap2);
    std::cout << "After merge: ";
    while (!heap1.isEmpty()) {
        std::cout << heap1.extractMin() << " ";
    }
    std::cout << std::endl;

    return 0;
}