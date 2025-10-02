#include <iostream>
#include <vector>
#include <algorithm>

class BinomialHeap {
private:
    struct Node {
        int key;
        int degree;
        Node* parent;
        Node* child;
        Node* sibling;

        Node(int k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
    };

    Node* head;

    // Связывает два биномиальных дерева одинакового порядка
    void linkTrees(Node* y, Node* z) {
        y->parent = z;
        y->sibling = z->child;
        z->child = y;
        z->degree++;
    }

    // Объединяет два списка корней
    Node* mergeHeaps(Node* h1, Node* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        Node* newHead = nullptr;
        Node** current = &newHead;

        while (h1 && h2) {
            if (h1->degree <= h2->degree) {
                *current = h1;
                h1 = h1->sibling;
            } else {
                *current = h2;
                h2 = h2->sibling;
            }
            current = &((*current)->sibling);
        }

        if (h1) {
            *current = h1;
        } else {
            *current = h2;
        }

        return newHead;
    }

    // Объединяет две биномиальные кучи
    Node* unionHeaps(Node* h1, Node* h2) {
        Node* newHead = mergeHeaps(h1, h2);
        if (!newHead) return nullptr;

        Node* prev = nullptr;
        Node* curr = newHead;
        Node* next = curr->sibling;

        while (next) {
            if (curr->degree != next->degree || 
                (next->sibling && next->sibling->degree == curr->degree)) {
                prev = curr;
                curr = next;
            } else {
                if (curr->key <= next->key) {
                    curr->sibling = next->sibling;
                    linkTrees(next, curr);
                } else {
                    if (!prev) {
                        newHead = next;
                    } else {
                        prev->sibling = next;
                    }
                    linkTrees(curr, next);
                    curr = next;
                }
            }
            next = curr->sibling;
        }

        return newHead;
    }

    // Переворачивает список детей
    Node* reverseList(Node* node) {
        Node* prev = nullptr;
        Node* curr = node;
        while (curr) {
            Node* next = curr->sibling;
            curr->sibling = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }

    // Рекурсивно освобождает память
    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->child);
        deleteTree(node->sibling);
        delete node;
    }

public:
    BinomialHeap() : head(nullptr) {}

    ~BinomialHeap() {
        deleteTree(head);
    }

    void insert(int key) {
        Node* newNode = new Node(key);
        head = unionHeaps(head, newNode);
    }

    int getMinimum() {
        if (!head) {
            throw std::runtime_error("Heap is empty");
        }

        Node* minNode = head;
        Node* curr = head->sibling;
        while (curr) {
            if (curr->key < minNode->key) {
                minNode = curr;
            }
            curr = curr->sibling;
        }
        return minNode->key;
    }

    int extractMin() {
        if (!head) {
            throw std::runtime_error("Heap is empty");
        }

        // Находим узел с минимальным ключом
        Node* minNode = head;
        Node* prevMin = nullptr;
        Node* prev = nullptr;
        Node* curr = head;

        while (curr) {
            if (curr->key < minNode->key) {
                minNode = curr;
                prevMin = prev;
            }
            prev = curr;
            curr = curr->sibling;
        }

        // Удаляем minNode из списка корней
        if (prevMin) {
            prevMin->sibling = minNode->sibling;
        } else {
            head = minNode->sibling;
        }

        // Создаем кучу из детей minNode
        Node* childHeap = reverseList(minNode->child);

        // Объединяем основную кучу с кучей детей
        head = unionHeaps(head, childHeap);

        int minKey = minNode->key;
        delete minNode;
        return minKey;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    void printTree(Node* node, int level) {
        while (node) {
            std::cout << std::string(level * 2, ' ') 
                      << "Key: " << node->key << ", Degree: " << node->degree << std::endl;
            if (node->child) {
                printTree(node->child, level + 1);
            }
            node = node->sibling;
        }
    }

    void printHeap() {
        if (!head) {
            std::cout << "Binomial Heap is empty" << std::endl;
            return;
        }
        std::cout << "Binomial Heap:" << std::endl;
        printTree(head, 0);
    }
};

// Пример использования
int main() {
    BinomialHeap heap;
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);
    heap.insert(15);
    heap.insert(30);

    std::cout << "\nBinomial Heap Operations:" << std::endl;
    heap.printHeap();
    std::cout << "Min element: " << heap.getMinimum() << std::endl;
    
    std::cout << "Extracting min: " << heap.extractMin() << std::endl;
    heap.printHeap();
    
    std::cout << "Extracting min: " << heap.extractMin() << std::endl;
    heap.printHeap();
}