#include <iostream>
#include <algorithm>

struct Comparator {
    bool operator()(int left, int right) const {
        return left > right;
    }
};

template <typename T, typename Compare>
class AVLTree {
private:
    struct Node {
        T key;
        int height;
        int size;
        Node* left;
        Node* right;

        Node(const T& value) {
            key = value;
            height = 1;
            size = 1;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;
    Compare compare;

    int get_height(Node* node) const {
        return node == nullptr ? 0 : node->height;
    }

    int get_size(Node* node) const {
        return node == nullptr ? 0 : node->size;
    }

    void update(Node* node) {
        if (node == nullptr) {
            return;
        }

        node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
        node->size = get_size(node->left) + get_size(node->right) + 1;
    }

    int get_balance(Node* node) const {
        return get_height(node->right) - get_height(node->left);
    }

    Node* rotate_left(Node* node) {
        Node* right_child = node->right;

        node->right = right_child->left;
        right_child->left = node;

        update(node);
        update(right_child);

        return right_child;
    }

    Node* rotate_right(Node* node) {
        Node* left_child = node->left;

        node->left = left_child->right;
        left_child->right = node;

        update(node);
        update(left_child);

        return left_child;
    }

    Node* balance(Node* node) {
        update(node);

        int balance_value = get_balance(node);

        if (balance_value == 2) {
            if (get_balance(node->right) < 0) {
                node->right = rotate_right(node->right);
            }

            return rotate_left(node);
        }

        if (balance_value == -2) {
            if (get_balance(node->left) > 0) {
                node->left = rotate_left(node->left);
            }

            return rotate_right(node);
        }

        return node;
    }

    Node* insert(Node* node, const T& value, int& position) {
        if (node == nullptr) {
            return new Node(value);
        }

        // compare(value, node->key) означает, что новый солдат должен стоять раньше
        if (compare(value, node->key)) {
            node->left = insert(node->left, value, position);
        }
        else {
            position += get_size(node->left) + 1;
            node->right = insert(node->right, value, position);
        }

        return balance(node);
    }

    Node* get_first(Node* node) const {
        while (node->left != nullptr) {
            node = node->left;
        }

        return node;
    }

    Node* remove_by_position(Node* node, int position) {
        if (node == nullptr) {
            return nullptr;
        }

        int left_size = get_size(node->left);

        if (position < left_size) {
            node->left = remove_by_position(node->left, position);
        }
        else if (position > left_size) {
            node->right = remove_by_position(node->right, position - left_size - 1);
        }
        else {
            Node* left_child = node->left;
            Node* right_child = node->right;

            if (right_child == nullptr) {
                delete node;
                return left_child;
            }

            Node* next_node = get_first(right_child);
            node->key = next_node->key;
            node->right = remove_by_position(node->right, 0);
        }

        return balance(node);
    }

    void clear(Node* node) {
        if (node == nullptr) {
            return;
        }

        clear(node->left);
        clear(node->right);

        delete node;
    }

public:
    AVLTree(Compare comp) {
        root = nullptr;
        compare = comp;
    }

    ~AVLTree() {
        clear(root);
    }

    int insert(const T& value) {
        int position = 0;
        root = insert(root, value, position);

        return position;
    }

    void remove_by_position(int position) {
        root = remove_by_position(root, position);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    Comparator comparator;
    AVLTree<int, Comparator> soldiers(comparator);

    for (int i = 0; i < n; i++) {
        int command, value;
        std::cin >> command >> value;

        if (command == 1) {
            std::cout << soldiers.insert(value) << '\n';
        }
        else if (command == 2) {
            soldiers.remove_by_position(value);
        }
    }

    return 0;
}