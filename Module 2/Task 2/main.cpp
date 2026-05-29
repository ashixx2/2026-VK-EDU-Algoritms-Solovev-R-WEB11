#include <iostream>
#include <vector>
#include <functional>

struct Comparator {
    bool operator()(long long left, long long right) const {
        return left < right;
    }
};

template <typename T, typename Compare>
class BinarySearchTree {
private:
    struct Node {
        T key;
        int left;
        int right;

        Node(const T& value) {
            key = value;
            left = -1;
            right = -1;
        }
    };

    std::vector<Node> nodes;
    int root;
    Compare compare;

public:
    BinarySearchTree(Compare comparator) {
        root = -1;
        compare = comparator;
    }

    void reserve(size_t size) {
        nodes.reserve(size);
    }

    void insert(const T& value) {
        if (root == -1) {
            nodes.push_back(Node(value));
            root = 0;
            return;
        }

        int current = root;

        while (true) {
            // Если новое значение меньше текущего, идём в левое поддерево.
            // Иначе идём в правое, как требуется в условии.
            if (compare(value, nodes[current].key)) {
                if (nodes[current].left == -1) {
                    nodes[current].left = static_cast<int>(nodes.size());
                    nodes.push_back(Node(value));
                    return;
                }

                current = nodes[current].left;
            }
            else {
                if (nodes[current].right == -1) {
                    nodes[current].right = static_cast<int>(nodes.size());
                    nodes.push_back(Node(value));
                    return;
                }

                current = nodes[current].right;
            }
        }
    }

    void print_pre_order(std::ostream& out) const {
        if (root == -1) {
            return;
        }

        std::vector<int> stack;
        stack.push_back(root);

        bool first = true;

        while (!stack.empty()) {
            int current = stack.back();
            stack.pop_back();

            if (!first) {
                out << ' ';
            }

            first = false;
            out << nodes[current].key;

            // Стек работает по принципу LIFO.
            // Поэтому сначала кладём правого сына, а потом левого,
            // чтобы левый был обработан раньше.
            if (nodes[current].right != -1) {
                stack.push_back(nodes[current].right);
            }

            if (nodes[current].left != -1) {
                stack.push_back(nodes[current].left);
            }
        }

        out << '\n';
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    Comparator comparator;
    BinarySearchTree<long long, Comparator> tree(comparator);

    tree.reserve(n);

    for (int i = 0; i < n; i++) {
        long long value;
        std::cin >> value;

        tree.insert(value);
    }

    tree.print_pre_order(std::cout);

    return 0;
}