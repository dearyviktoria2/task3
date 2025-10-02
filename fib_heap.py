class SimpleFibonacciHeap:
    class Node:
        def __init__(self, key):
            self.key = key
            self.degree = 0
            self.parent = None
            self.child = None
            self.left = self
            self.right = self
    
    def __init__(self):
        self.min_node = None
        self.count = 0
    
    def insert(self, key):
        new_node = self.Node(key)
        
        if self.min_node is None:
            self.min_node = new_node
        else:
            # Добавляем в корневой список
            new_node.left = self.min_node
            new_node.right = self.min_node.right
            self.min_node.right.left = new_node
            self.min_node.right = new_node
            
            if key < self.min_node.key:
                self.min_node = new_node
        
        self.count += 1
        return new_node
    
    def extract_min(self):
        if self.min_node is None:
            return None
        
        min_node = self.min_node
        min_key = min_node.key
        
        # Добавляем детей в корневой список
        if min_node.child:
            child = min_node.child
            while True:
                next_child = child.right
                child.parent = None
                
                # Добавляем child в корневой список
                child.left = self.min_node
                child.right = self.min_node.right
                self.min_node.right.left = child
                self.min_node.right = child
                
                child = next_child
                if child == min_node.child:
                    break
        
        # Удаляем min_node
        min_node.left.right = min_node.right
        min_node.right.left = min_node.left
        
        if min_node == min_node.right:
            self.min_node = None
        else:
            self.min_node = min_node.right
            self._consolidate()
        
        self.count -= 1
        return min_key
    
    def _consolidate(self):
        if not self.min_node:
            return
        
        degree_map = {}
        nodes = []
        
        current = self.min_node
        while True:
            nodes.append(current)
            current = current.right
            if current == self.min_node:
                break
        
        for node in nodes:
            degree = node.degree
            while degree in degree_map:
                other = degree_map[degree]
                if node.key > other.key:
                    node, other = other, node
                
                self._link(other, node)
                del degree_map[degree]
                degree += 1
            degree_map[degree] = node
        
        # Перестраиваем кучу
        self.min_node = None
        for node in degree_map.values():
            if self.min_node is None:
                self.min_node = node
                node.left = node
                node.right = node
            else:
                node.left = self.min_node
                node.right = self.min_node.right
                self.min_node.right.left = node
                self.min_node.right = node
                
                if node.key < self.min_node.key:
                    self.min_node = node
    
    def _link(self, child, parent):
        child.left.right = child.right
        child.right.left = child.left
        
        child.parent = parent
        if not parent.child:
            parent.child = child
            child.left = child
            child.right = child
        else:
            child.left = parent.child
            child.right = parent.child.right
            parent.child.right.left = child
            parent.child.right = child
        
        parent.degree += 1

# Тестирование упрощенной версии
def test_simple_fib_heap():
    heap = SimpleFibonacciHeap()
    
    heap.insert(5)
    heap.insert(3)
    heap.insert(8)
    heap.insert(1)
    heap.insert(10)
    
    print("Simple Fibonacci Heap:")
    print("Extracted:", heap.extract_min())  # 1
    print("Extracted:", heap.extract_min())  # 3
    print("Extracted:", heap.extract_min())  # 5
    print("Extracted:", heap.extract_min())  # 8
    print("Extracted:", heap.extract_min())  # 10

if __name__ == "__main__":
    test_simple_fib_heap()