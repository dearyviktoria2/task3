#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

class MinHeap {
private:
    std::vector<int> heap;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void siftUp(int i) {
        while (i > 0 && heap[i] < heap[parent(i)]) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void siftDown(int i) {
        int minIndex = i;
        int left = leftChild(i);
        int right = rightChild(i);

        if (left < heap.size() && heap[left] < heap[minIndex])
            minIndex = left;

        if (right < heap.size() && heap[right] < heap[minIndex])
            minIndex = right;

        if (i != minIndex) {
            std::swap(heap[i], heap[minIndex]);
            siftDown(minIndex);
        }
    }

public:
    MinHeap() {}

    void insert(int key) {
        heap.push_back(key);
        siftUp(heap.size() - 1);
    }

    int getMin() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }

    int extractMin() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        int min = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            siftDown(0);
        }
        return min;
    }

    bool isEmpty() {
        return heap.empty();
    }

    size_t size() {
        return heap.size();
    }

    void printHeap() {
        std::cout << "Binary Heap: ";
        for (int val : heap) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

// Пример использования
int main() {
    MinHeap heap;
    heap.insert(5);
    heap.insert(3);
    heap.insert(8);
    heap.insert(1);
    heap.insert(10);

    std::cout << "Binary Heap Operations:" << std::endl;
    heap.printHeap();
    std::cout << "Min element: " << heap.getMin() << std::endl;
    
    std::cout << "Extracting elements: ";
    while (!heap.isEmpty()) {
        std::cout << heap.extractMin() << " ";
    }
    std::cout << std::endl;
}
