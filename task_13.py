import heapq


class MaxHeap:
    def __init__(self, data=None):
        if data is None:
            self.heap = []
        else:
            self.heap = [-x for x in data]
            heapq.heapify(self.heap)
    
    def push(self, item):
        heapq.heappush(self.heap, -item)
    
    def pop(self):
        if self.is_empty():
            raise IndexError("Pop from empty heap")
        return -heapq.heappop(self.heap)
    
    def peek(self):
        if self.is_empty():
            return None
        return -self.heap[0]
    
    def is_empty(self):
        return len(self.heap) == 0
    
    def size(self):
        return len(self.heap)
    
    def __str__(self):
        return str([-x for x in self.heap])

# Пример использования Max-Heap
max_heap = MaxHeap()
max_heap.push(5)
max_heap.push(3)
max_heap.push(8)
max_heap.push(1)
max_heap.push(10)


print("\nMaxHeap класс:")
print("Куча:", max_heap)
print("Максимальный элемент:", max_heap.peek())
print("Размер:", max_heap.size())
print("\n")


class BinomialNode:
    def __init__(self, key):
        self.key = key
        self.degree = 0
        self.parent = None
        self.child = None
        self.sibling = None

class BinomialHeap:
    def __init__(self):
        self.head = None
    
    def _merge_heaps(self, h1, h2):
        if not h1:
            return h2
        if not h2:
            return h1
        
        if h1.degree <= h2.degree:
            result = h1
            h1 = h1.sibling
        else:
            result = h2
            h2 = h2.sibling
        
        current = result
        
        while h1 and h2:
            if h1.degree <= h2.degree:
                current.sibling = h1
                h1 = h1.sibling
            else:
                current.sibling = h2
                h2 = h2.sibling
            current = current.sibling
        
        current.sibling = h1 if h1 else h2
        return result
    
    def _link_trees(self, y, z):
        y.parent = z
        y.sibling = z.child
        z.child = y
        z.degree += 1
    
    def _union(self, h1, h2):
        h = self._merge_heaps(h1, h2)
        if not h:
            return None
        
        prev = None
        x = h
        next = x.sibling
        
        while next:
            if (x.degree != next.degree or 
                (next.sibling and next.sibling.degree == x.degree)):
                prev = x
                x = next
            elif x.key <= next.key:
                x.sibling = next.sibling
                self._link_trees(next, x)
            else:
                if not prev:
                    h = next
                else:
                    prev.sibling = next
                self._link_trees(x, next)
                x = next
            next = x.sibling
        
        return h
    
    def insert(self, key):
        new_heap = BinomialNode(key)
        self.head = self._union(self.head, new_heap)
    
    def get_min(self):
        if not self.head:
            return None
        
        min_node = self.head
        current = self.head.sibling
        
        while current:
            if current.key < min_node.key:
                min_node = current
            current = current.sibling
        
        return min_node.key
    
    def extract_min(self):
        if not self.head:
            return None
        
        # Находим узел с минимальным ключом
        min_node = self.head
        prev_min = None
        prev = None
        current = self.head
        
        while current:
            if current.key < min_node.key:
                min_node = current
                prev_min = prev
            prev = current
            current = current.sibling
        
        # Удаляем минимальный узел из списка корней
        if prev_min:
            prev_min.sibling = min_node.sibling
        else:
            self.head = min_node.sibling
        
        # Создаем кучу из детей минимального узла
        child_heap = None
        child = min_node.child
        
        while child:
            next_child = child.sibling
            child.sibling = child_heap
            child.parent = None
            child_heap = child
            child = next_child
        
        # Объединяем основную кучу с кучей детей
        self.head = self._union(self.head, child_heap)
        
        return min_node.key
    
    def _print_tree(self, node, level=0):
        if node:
            print("  " * level + f"Key: {node.key}, Degree: {node.degree}")
            self._print_tree(node.child, level + 1)
            self._print_tree(node.sibling, level)
    
    def print_heap(self):
        print("Биномиальная куча:")
        current = self.head
        while current:
            self._print_tree(current)
            current = current.sibling
        print()

# Пример использования биномиальной кучи
bheap = BinomialHeap()
bheap.insert(10)
bheap.insert(20)
bheap.insert(5)
bheap.insert(15)
bheap.insert(30)

bheap.print_heap()

print("Извлечение минимальных элементов:")
while bheap.head:
    min_val = bheap.extract_min()
    print(f"Извлечен: {min_val}")