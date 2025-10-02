public class SimpleFibonacciHeap {
    private static class Node {
        int key;
        int degree;
        Node parent;
        Node child;
        Node left;
        Node right;

        Node(int key) {
            this.key = key;
            this.degree = 0;
            this.parent = null;
            this.child = null;
            this.left = this;
            this.right = this;
        }
    }

    private Node minNode;
    private int count;

    public void insert(int key) {
        Node newNode = new Node(key);
        
        if (minNode == null) {
            minNode = newNode;
        } else {
            newNode.left = minNode;
            newNode.right = minNode.right;
            minNode.right.left = newNode;
            minNode.right = newNode;

            if (key < minNode.key) {
                minNode = newNode;
            }
        }
        count++;
    }

    public Integer extractMin() {
        if (minNode == null) return null;

        Node min = minNode;
        int minKey = min.key;

        // Add children to root list
        if (min.child != null) {
            Node child = min.child;
            do {
                Node nextChild = child.right;
                child.parent = null;
                
                child.left = minNode;
                child.right = minNode.right;
                minNode.right.left = child;
                minNode.right = child;
                
                child = nextChild;
            } while (child != min.child);
        }

        // Remove min from root list
        min.left.right = min.right;
        min.right.left = min.left;

        if (min == min.right) {
            minNode = null;
        } else {
            minNode = min.right;
            consolidate();
        }

        count--;
        return minKey;
    }

    private void consolidate() {
        if (minNode == null) return;

        int maxDegree = 50; // Просто большое число
        Node[] degreeTable = new Node[maxDegree];

        // Собираем все корневые узлы
        java.util.List<Node> roots = new java.util.ArrayList<>();
        Node current = minNode;
        do {
            roots.add(current);
            current = current.right;
        } while (current != minNode);

        for (Node node : roots) {
            int degree = node.degree;
            while (degreeTable[degree] != null) {
                Node other = degreeTable[degree];
                if (node.key > other.key) {
                    Node temp = node;
                    node = other;
                    other = temp;
                }
                link(other, node);
                degreeTable[degree] = null;
                degree++;
            }
            degreeTable[degree] = node;
        }

        // Восстанавливаем minNode
        minNode = null;
        for (Node node : degreeTable) {
            if (node != null) {
                if (minNode == null) {
                    minNode = node;
                    node.left = node;
                    node.right = node;
                } else {
                    node.left = minNode;
                    node.right = minNode.right;
                    minNode.right.left = node;
                    minNode.right = node;

                    if (node.key < minNode.key) {
                        minNode = node;
                    }
                }
            }
        }
    }

    private void link(Node child, Node parent) {
        child.left.right = child.right;
        child.right.left = child.left;

        child.parent = parent;
        if (parent.child == null) {
            parent.child = child;
            child.left = child;
            child.right = child;
        } else {
            child.left = parent.child;
            child.right = parent.child.right;
            parent.child.right.left = child;
            parent.child.right = child;
        }
        parent.degree++;
    }

    public boolean isEmpty() {
        return minNode == null;
    }

    public static void main(String[] args) {
        SimpleFibonacciHeap heap = new SimpleFibonacciHeap();
        heap.insert(5);
        heap.insert(3);
        heap.insert(8);
        heap.insert(1);
        heap.insert(10);

        System.out.println("Extracting from Fibonacci Heap:");
        while (!heap.isEmpty()) {
            System.out.print(heap.extractMin() + " ");
        }
    }
}