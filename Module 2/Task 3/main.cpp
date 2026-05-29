#include <iostream>
#include <vector>
#include <queue>
#include <cstdint>

struct Comparator {
    bool operator()(uint64_t left, uint64_t right) const {
        return left < right;
    }
};

template <typename T, typename Compare>
class BTree {
private:
    struct Node {
        bool leaf;
        std::vector<T> keys;
        std::vector<int> children;

        Node(bool is_leaf) {
            leaf = is_leaf;
        }
    };

    std::vector<Node> nodes;
    int root;
    int t;
    Compare compare;

    void split_child(int parent_index, int child_position) {
        int child_index = nodes[parent_index].children[child_position];

        Node new_node(nodes[child_index].leaf);

        T middle_key = nodes[child_index].keys[t - 1];

        // В новый узел переносится правая часть ключей полного ребенка
        for (int i = 0; i < t - 1; i++) {
            new_node.keys.push_back(nodes[child_index].keys[i + t]);
        }

        // Если ребенок не лист, переносим и правую часть его потомков
        if (!nodes[child_index].leaf) {
            for (int i = 0; i < t; i++) {
                new_node.children.push_back(nodes[child_index].children[i + t]);
            }
        }

        nodes[child_index].keys.resize(t - 1);

        if (!nodes[child_index].leaf) {
            nodes[child_index].children.resize(t);
        }

        int new_node_index = static_cast<int>(nodes.size());
        nodes.push_back(new_node);

        nodes[parent_index].keys.insert(
            nodes[parent_index].keys.begin() + child_position,
            middle_key
        );

        nodes[parent_index].children.insert(
            nodes[parent_index].children.begin() + child_position + 1,
            new_node_index
        );
    }

    void insert_non_full(int node_index, const T& value) {
        int i = static_cast<int>(nodes[node_index].keys.size()) - 1;

        if (nodes[node_index].leaf) {
            nodes[node_index].keys.push_back(value);

            // Вставляем ключ в нужную позицию внутри листа
            while (i >= 0 && compare(value, nodes[node_index].keys[i])) {
                nodes[node_index].keys[i + 1] = nodes[node_index].keys[i];
                i--;
            }

            nodes[node_index].keys[i + 1] = value;
        }
        else {
            // Выбираем потомка, в которого нужно спускаться
            while (i >= 0 && compare(value, nodes[node_index].keys[i])) {
                i--;
            }

            i++;

            if (static_cast<int>(nodes[nodes[node_index].children[i]].keys.size()) == 2 * t - 1) {
                split_child(node_index, i);

                // После разделения решаем, в левую или правую часть идти дальше
                if (!compare(value, nodes[node_index].keys[i])) {
                    i++;
                }
            }

            insert_non_full(nodes[node_index].children[i], value);
        }
    }

public:
    BTree(int min_degree, Compare comp) {
        t = min_degree;
        compare = comp;

        nodes.push_back(Node(true));
        root = 0;
    }

    void insert(const T& value) {
        if (static_cast<int>(nodes[root].keys.size()) == 2 * t - 1) {
            Node new_root(false);
            new_root.children.push_back(root);

            nodes.push_back(new_root);

            root = static_cast<int>(nodes.size()) - 1;

            split_child(root, 0);
            insert_non_full(root, value);
        }
        else {
            insert_non_full(root, value);
        }
    }

    void print_by_levels(std::ostream& out) const {
        std::queue<int> queue;
        queue.push(root);

        while (!queue.empty()) {
            int level_size = static_cast<int>(queue.size());
            bool first = true;

            for (int i = 0; i < level_size; i++) {
                int current = queue.front();
                queue.pop();

                for (size_t j = 0; j < nodes[current].keys.size(); j++) {
                    if (!first) {
                        out << ' ';
                    }

                    first = false;
                    out << nodes[current].keys[j];
                }

                for (int child : nodes[current].children) {
                    queue.push(child);
                }
            }

            out << '\n';
        }
    }
};

int main() {
    int t;
    std::cin >> t;

    Comparator comparator;
    BTree<uint64_t, Comparator> tree(t, comparator);

    uint64_t value;

    while (std::cin >> value) {
        tree.insert(value);
    }

    tree.print_by_levels(std::cout);

    return 0;
}