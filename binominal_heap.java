import java.util.*;

public class BinomialHeap<T> {
    private Node<T> head;
    private Comparator<T> comparator;
    
    private static class Node<T> {
        T key;
        int degree;
        Node<T> parent;
        Node<T> child;
        Node<T> sibling;
        
        Node(T key) {
            this.key = key;
            this.degree = 0;
        }
    }
    
    public BinomialHeap() {
        this(null);
    }
    
    public BinomialHeap(Comparator<T> comparator) {
        this.head = null;
        this.comparator = comparator;
    }
    
    // Сравнение элементов
    @SuppressWarnings("unchecked")
    private boolean compare(T a, T b) {
        if (comparator != null) {
            return comparator.compare(a, b) < 0;
        } else {
            return ((Comparable<T>) a).compareTo(b) < 0;
        }
    }
    
    // Объединение двух куч
    private Node<T> merge(Node<T> h1, Node<T> h2) {
        if (h1 == null) return h2;
        if (h2 == null) return h1;
        
        Node<T> head;
        Node<T> tail;
        
        // Выбираем голову с меньшей степенью
        if (h1.degree <= h2.degree) {
            head = h1;
            h1 = h1.sibling;
        } else {
            head = h2;
            h2 = h2.sibling;
        }
        
        tail = head;
        
        // Сливаем списки
        while (h1 != null && h2 != null) {
            if (h1.degree <= h2.degree) {
                tail.sibling = h1;
                h1 = h1.sibling;
            } else {
                tail.sibling = h2;
                h2 = h2.sibling;
            }
            tail = tail.sibling;
        }
        
        // Добавляем оставшиеся узлы
        tail.sibling = (h1 != null) ? h1 : h2;
        
        return head;
    }
    
    // Связывание двух деревьев одинаковой степени
    private void link(Node<T> child, Node<T> parent) {
        child.parent = parent;
        child.sibling = parent.child;
        parent.child = child;
        parent.degree++;
    }
    
    // Объединение с уплотнением
    private Node<T> union(Node<T> h1, Node<T> h2) {
        Node<T> head = merge(h1, h2);
        if (head == null) return null;
        
        Node<T> prev = null;
        Node<T> curr = head;
        Node<T> next = curr.sibling;
        
        while (next != null) {
            if (curr.degree != next.degree || 
                (next.sibling != null && next.sibling.degree == curr.degree)) {
                prev = curr;
                curr = next;
            } else {
                if (compare(curr.key, next.key)) {
                    curr.sibling = next.sibling;
                    link(next, curr);
                } else {
                    if (prev == null) {
                        head = next;
                    } else {
                        prev.sibling = next;
                    }
                    link(curr, next);
                    curr = next;
                }
            }
            next = curr.sibling;
        }
        
        return head;
    }
    
    // Основные операции
    public void insert(T key) {
        Node<T> node = new Node<>(key);
        head = union(head, node);
    }
    
    public T findMinimum() {
        if (head == null) {
            throw new NoSuchElementException("Heap is empty");
        }
        
        Node<T> min = head;
        Node<T> current = head.sibling;
        
        while (current != null) {
            if (compare(current.key, min.key)) {
                min = current;
            }
            current = current.sibling;
        }
        
        return min.key;
    }
    
    public T extractMinimum() {
        if (head == null) {
            throw new NoSuchElementException("Heap is empty");
        }
        
        // Находим узел с минимальным ключом
        Node<T> min = head;
        Node<T> prevMin = null;
        Node<T> prev = null;
        Node<T> current = head;
        
        while (current != null) {
            if (compare(current.key, min.key)) {
                min = current;
                prevMin = prev;
            }
            prev = current;
            current = current.sibling;
        }
        
        // Удаляем min из списка корней
        if (prevMin != null) {
            prevMin.sibling = min.sibling;
        } else {
            head = min.sibling;
        }
        
        // Переворачиваем список детей min
        Node<T> childHead = null;
        Node<T> child = min.child;
        while (child != null) {
            Node<T> next = child.sibling;
            child.sibling = childHead;
            child.parent = null;
            childHead = child;
            child = next;
        }
        
        // Объединяем основную кучу с кучей детей
        head = union(head, childHead);
        
        return min.key;
    }
    
    public boolean isEmpty() {
        return head == null;
    }
    
    // Вспомогательные методы для отладки
    public void printHeap() {
        System.out.println("Binomial Heap:");
        printTree(head, 0);
    }
    
    private void printTree(Node<T> node, int level) {
        while (node != null) {
            System.out.println("  ".repeat(level) + 
                "Key: " + node.key + ", Degree: " + node.degree);
            if (node.child != null) {
                printTree(node.child, level + 1);
            }
            node = node.sibling;
        }
    }
    
    // Пример использования
    public static void main(String[] args) {
        BinomialHeap<Integer> heap = new BinomialHeap<>();
        
        heap.insert(10);
        heap.insert(5);
        heap.insert(20);
        heap.insert(3);
        heap.insert(8);
        
        heap.printHeap();
        
        System.out.println("Minimum: " + heap.findMinimum());
        System.out.println("Extracted: " + heap.extractMinimum());
        System.out.println("New minimum: " + heap.findMinimum());
        
        heap.printHeap();
    }
}