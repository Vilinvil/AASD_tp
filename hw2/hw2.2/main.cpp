// Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки. Т.е.,
// при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Выведите элементы в порядке pre-order (сверху вниз).

// Рекурсия запрещена.
#include <iostream>
#include <sstream>
#include <stack>
#include <utility>

template <typename T> struct DefaultComparator {
    bool operator()(const T &l, const T &r) const {
        if (l < r) {
            return true;
        }
        return false;
    }
};

template <typename T> class TreeNode {
  public:
    TreeNode(int data) : data_(data), left_(nullptr), right_(nullptr){};
    T data_;
    TreeNode *left_;
    TreeNode *right_;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const TreeNode<T> &node) {
    return os << node.data_;
}

template <typename T, typename comp = DefaultComparator<int>> class Tree {
  public:
    Tree() : root_(nullptr){};
    void add(T data);
    void pre_order(void (*handler)(TreeNode<T> *node));
    void post_order(void (*handler)(TreeNode<T> *node));

    Tree(const Tree &other) { copyTree(other); };
    Tree(Tree &&t) { this = std::exchange(t, nullptr); }

    Tree &operator=(Tree &&other) {
        this->~Tree();
        this = std::exchange(other, nullptr);
    }
    Tree &operator=(const Tree &other) {
        this->~Tree();
        copyTree(other);
        return *this;
    };

    ~Tree() {
        post_order([](TreeNode<T> *node) { delete node; });
    }

  private:
    void copyTree(const Tree &other);
    TreeNode<T> *root_;
};

template <typename comparator, typename T>
void Tree<comparator, T>::copyTree(const Tree<comparator, T> &other) {
    if (other.root_ == nullptr) {
        root_ = nullptr;
        return;
    }

    root_ = new TreeNode(other.root_->data_);

    std::stack<std::pair<TreeNode<T> *, TreeNode<T> *>> stack_parent;
    stack_parent.push(
        std::pair<TreeNode<T> *, TreeNode<T> *>(root_, other.root_));

    while (!stack_parent.empty()) {
        auto [cur_this, cur_other] = stack_parent.top();
        stack_parent.pop();
        if (cur_other->left_) {
            cur_this->left_ = new TreeNode(cur_other->left_->data_);
            stack_parent.push(std::pair<TreeNode<T> *, TreeNode<T> *>(
                cur_this->left_, cur_other->left_));
        }
        if (cur_other->right_) {
            cur_this->right_ = new TreeNode(cur_other->right_->data_);
            stack_parent.push(std::pair<TreeNode<T> *, TreeNode<T> *>(
                cur_this->right_, cur_other->right_));
        }
    }
};

template <typename T, typename comparator>
void Tree<T, comparator>::pre_order(void (*handler)(TreeNode<T> *node)) {
    std::stack<TreeNode<T> *> stack_work;
    TreeNode<T> *cur_node = root_;
    if (cur_node == nullptr) {
        return;
    }
    stack_work.push(cur_node);
    while (!stack_work.empty()) {
        cur_node = stack_work.top();
        stack_work.pop();
        handler(cur_node);
        if (cur_node->left_) {
            stack_work.push(cur_node->left_);
        }
        if (cur_node->right_) {
            stack_work.push(cur_node->right_);
        }
    }
}

template <typename T, typename comparator>
void Tree<T, comparator>::post_order(void (*handler)(TreeNode<T> *node)) {
    std::stack<TreeNode<T> *> stack1;
    std::stack<TreeNode<T> *> stack2;
    TreeNode<T> *cur_node = root_;
    if (cur_node == nullptr) {
        return;
    }
    stack1.push(cur_node);
    while (!stack1.empty()) {
        cur_node = stack1.top();
        stack1.pop();
        stack2.push(cur_node);
        if (cur_node->left_) {
            stack1.push(cur_node->left_);
        }
        if (cur_node->right_) {
            stack1.push(cur_node->right_);
        }
    }

    for (; !stack2.empty();) {
        cur_node = stack2.top();
        handler(cur_node);
        stack2.pop();
    }
}

template <typename T, typename comparator>
void Tree<T, comparator>::add(T data) {
    auto *new_node = new TreeNode<T>(data);
    if (root_ == nullptr) {
        root_ = new_node;
        return;
    }

    TreeNode<T> *cur_node = root_;
    while (cur_node) {
        if (comparator()(data, cur_node->data_) && cur_node->right_) {
            cur_node = cur_node->right_;
        } else if (comparator()(data, cur_node->data_) &&
                   cur_node->right_ == nullptr) {
            cur_node->right_ = new_node;
            return;
        } else if (cur_node->left_) {
            cur_node = cur_node->left_;
        } else {
            cur_node->left_ = new_node;
            return;
        }
    }
}

template <typename T> void write(TreeNode<T> *node) {
    if (node) {
        std::cout << *node << " ";
    }
};

// void testLogic() {
//     {
//         Tree<DefaultComparator<int>> tree;
//         Tree<DefaultComparator<int>> tree2;
//         for (size_t i = 0; i < 10; i++) {
//             tree.add(i);
//         }
//         tree2 = tree;
//         tree.pre_order(write);
//         std::cout << std::endl << std::endl;
//         tree2.pre_order(write);
//     }
//     std::cout << "testLogic OK";
// };

int main(int argc, char *argv[]) {
    // testLogic();
    Tree<int> tree;
    int count;
    std::cin >> count;
    for (int i = 0; i < count; i++) {
        int num;
        std::cin >> num;
        tree.add(num);
    }

    tree.pre_order(write);
}