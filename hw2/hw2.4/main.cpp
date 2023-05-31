// 4_2. Порядковые статистики. Дано число N и N строк. Каждая строка содержит
// команду добавления или удаления натуральных чисел, а также запрос на
// получение k-ой порядковой статистики. Команда добавления числа A задается
// положительным числом A, команда удаления числа A задается отрицательным
// числом “-A”. Запрос на получение k-ой порядковой статистики задается числом
// k. Требования: скорость выполнения запроса - O(log n).

#include <assert.h>
#include <iostream>
#include <sstream>
#include <stack>

namespace {
template <typename T> struct DefaultComparator {
    int operator()(const T &l, const T &r) {
        if (l < r) {
            return -1;
        }
        if (l > r) {
            return 1;
        }
        return 0;
    }
};

template <typename Key, typename Comparator = DefaultComparator<Key>>
class AVLTree {
    struct TreeNode {
      public:
        TreeNode(const Key &key)
            : key_(key), left_(nullptr), right_(nullptr), count_nodes_(1),
              height_(1){};
        Key key_;

        size_t count_nodes_;
        uint8_t height_;
        TreeNode *left_;
        TreeNode *right_;
    };

  public:
    AVLTree(Comparator comp = Comparator()) : root_(nullptr), comp_(comp){};

    const Key *find(const Key &key) { return findAux(key, root_); };
    void insert(const Key &key) { root_ = insertAux(key, root_); };
    void erase(const Key &key) { root_ = eraseAux(key, root_); };

    const Key *kthStatistic(size_t k) { return kthStatisticAux(k, root_); }

    ~AVLTree() {
        post_order([](TreeNode *node) { delete node; });
    };

  private:
    TreeNode *root_;
    Comparator comp_;

    size_t count(TreeNode *node) {
        if (!node) {
            return 0;
        }

        return node->count_nodes_;
    }

    const Key *kthStatisticAux(size_t k, const TreeNode *node) {
        if (!node || k >= node->count_nodes_) {
            return nullptr;
        }

        size_t left_count = count(node->left_);
        if (left_count > k) {
            return kthStatisticAux(k, node->left_);
        }
        if (left_count == k) {
            return &node->key_;
        }

        return kthStatisticAux(k - left_count - 1, node->right_);
    };

    // Взял из своего номера 2.2 для удаления
    void post_order(void (*handler)(TreeNode *node)) {
        std::stack<TreeNode *> stack1;
        std::stack<TreeNode *> stack2;
        TreeNode *cur_node = root_;
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

    const Key *findAux(const Key &key, TreeNode *node) {
        if (!node) {
            return nullptr;
        }

        int res_comp = comp_(key, node->key_);

        // key < node->key
        if (res_comp == -1) {
            return findAux(key, node->left_);
        }
        // key > node->key
        if (res_comp == 1) {
            return findAux(key, node->right_);
        }

        // key == node->key
        return &node->key_;
    }

    TreeNode *insertAux(const Key &key, TreeNode *node) {
        if (!node) {
            return new TreeNode(key);
        }

        int res_comp = comp_(key, node->key_);

        // key < node->key
        if (res_comp == -1) {
            node->left_ = insertAux(key, node->left_);
        }
        // key > node->key
        if (res_comp == 1) {
            node->right_ = insertAux(key, node->right_);
        }

        // key == node->key
        return balance(node);
    }

    TreeNode *eraseAux(const Key &key, TreeNode *node) {
        if (!node) {
            return nullptr;
        }

        int res_comp = comp_(key, node->key_);

        // key < node->key
        if (res_comp == -1) {
            node->left_ = eraseAux(key, node->left_);
        } else if (res_comp == 1) {
            // key > node->key
            node->right_ = eraseAux(key, node->right_);
        } else {
            // key == node->key
            TreeNode *left = node->left_;
            TreeNode *right = node->right_;

            delete node;

            if (!right) {
                return left;
            }

            TreeNode *min_node = findAndRemoveMinNode(right);

            min_node->left_ = left;

            return balance(min_node);
        }

        return balance(node);
    }

    TreeNode *findAndRemoveMinNode(TreeNode *node) {
        if (!node->left_) {
            return node;
        }

        std::stack<TreeNode *> stack_balance;
        TreeNode *prev_node = node;
        TreeNode *cur_node = node->left_;
        stack_balance.push(prev_node);
        while (cur_node->left_) {
            prev_node = cur_node;
            cur_node = cur_node->left_;
            stack_balance.push(prev_node);
        }

        prev_node->left_ = cur_node->right_;
        cur_node->right_ = node;

        while (!stack_balance.empty()) {
            TreeNode *tmp = stack_balance.top();
            stack_balance.pop();
            balance(tmp);
        }

        return cur_node;
    }

    uint8_t height(TreeNode *node) {
        if (!node) {
            return 0;
        }

        return node->height_;
    }

    void fixHeightAndCount(TreeNode *node) {
        node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
        node->count_nodes_ = count(node->left_) + count(node->right_) + 1;
    }

    int8_t bFactor(TreeNode *node) {
        return height(node->right_) - height(node->left_);
    }

    TreeNode *balance(TreeNode *node) {
        fixHeightAndCount(node);

        int8_t bF = bFactor(node);
        if (bF == 2) {
            if (bFactor(node->right_) < 0) {
                node->right_ = rotateRight(node->right_);
            }
            return rotateLeft(node);
        }

        if (bF == -2) {
            if (bFactor(node->left_) > 0) {
                node->left_ = rotateLeft(node->left_);
            }
            return rotateRight(node);
        }

        return node;
    }

    TreeNode *rotateLeft(TreeNode *node) {
        TreeNode *tmp = node->right_;
        node->right_ = tmp->left_;
        tmp->left_ = node;
        fixHeightAndCount(node);
        fixHeightAndCount(tmp);
        return tmp;
    };

    TreeNode *rotateRight(TreeNode *node) {
        TreeNode *tmp = node->left_;
        node->left_ = tmp->right_;
        tmp->right_ = node;
        fixHeightAndCount(node);
        fixHeightAndCount(tmp);
        return tmp;
    };
};

// run() can throw std::runtime_error
void run(std::istream &input, std::ostream &output) {
    size_t n;
    input >> n;
    AVLTree<int> tree;
    for (size_t i = 0; i < n; i++) {
        long long operation;
        input >> operation;
        if (operation == 0) {
            throw std::runtime_error(
                "in run: zero number prohibited. operation=" +
                std::to_string(operation));
        }
        if (operation > 0) {
            tree.insert(operation);
        }
        if (operation < 0) {
            tree.erase(operation * (-1));
        }

        size_t kth;
        input >> kth;

        const int *res = tree.kthStatistic(kth);

        if (res) {
            output << *res << std::endl;
        } else {
            throw std::runtime_error(
                "in run: find kthStatic return nullptr. With kth=" +
                std::to_string(kth) +
                " operation=" + std::to_string(operation));
        }
    }
}

void testLogic() {
    {
        std::stringstream in_str;
        std::stringstream out_str;

        in_str << R"(50
2 0
10 0
19 0
17 3
9 4
20 5
4 3
-20 4
21 4
24 5
22 6
3 2
14 2
-21 4
23 6
18 6
-17 8
6 7
15 3
25 5
-22 7
-19 8
17 8
-6 8
6 5
-4 11
-2 7
-9 6
19 3
-10 2
7 1
8 1
20 5
-17 8
4 11
9 11
-19 11
1 2
-9 10
9 13
-9 0
2 7
5 12
-3 5
12 9
-25 0
-2 2
3 13
-8 10
9 13)";
        std::string res_expec = R"(2
2
2
19
19
20
10
17
17
19
21
4
4
10
17
17
22
18
6
10
15
18
17
18
10
25
18
18
14
14
6
6
15
20
24
23
24
4
23
25
1
14
23
7
15
1
5
24
20
24
)";
        try {
            run(in_str, out_str);
            std::cout << out_str.str() << std::endl << std::endl;
            std::cout << res_expec << std::endl << std::endl;
            assert(res_expec == out_str.str());
        } catch (std::runtime_error &e) {
            throw std::runtime_error(e.what());
        }

        std::cout << "test from test system 2 OK\n";
    }
    {
        std::stringstream in_str;
        std::stringstream out_str;

        in_str << "1 0 0";
        std::string res_expec = "in run: zero number prohibited. operation=0";
        try {
            run(in_str, out_str);
        } catch (std::runtime_error &e) {
            assert(res_expec == e.what());
        }

        std::cout << "test one zero element OK\n";
    }
    {
        std::stringstream in_str;
        std::stringstream out_str;

        in_str << "5 40 0 10 1 4 1 -10 0 50 2";
        std::string res_expext = "40\n40\n10\n4\n50\n";
        run(in_str, out_str);

        std::string res_recive = out_str.str();
        assert(res_expext == out_str.str());
        std::cout << "test from document OK\n";
    }
    {
        std::stringstream in_str;
        std::stringstream out_str;

        in_str << "1 140 0";
        std::string res_expext = "140\n";
        run(in_str, out_str);

        std::string res_recive = out_str.str();
        assert(res_expext == out_str.str());
        std::cout << "test 1 elements OK\n";
    }
    {
        std::stringstream in_str;
        std::stringstream out_str;

        in_str << "5 10 0 60 1 100 2 1 0 3 4";
        std::string res_expext = "10\n60\n100\n1\n100\n";
        run(in_str, out_str);

        std::string res_recive = out_str.str();
        assert(res_expext == out_str.str());
        std::cout << "test 5 elements OK\n";
    }
    {
        std::stringstream in_str;
        std::stringstream out_str;

        in_str << "20 \
        9 0 6 0 2 1 8 1 -6 0 5 2 3 4 7 1\
                      -2 4 -9 3 2 1 -5 3 10 2 6 5 1 0 5 1 -5 3 5 3 -3 3 -5 2 ";
        std::string res_expext = R"(9
6
6
6
2
8
9
3
9
8
3
8
7
10
1
2
6
5
6
6
)";
        run(in_str, out_str);

        std::string res_recive = out_str.str();
        assert(res_expext == out_str.str());
        std::cout << "test by test system of techno park OK\n";
    }
    {
        std::stringstream in_str;
        std::stringstream out_str;

        in_str << "10 10 0 9 0 8 0 7 0 6 0 5 0 4 0 3 0 2 0 1 0";
        std::string res_expext = "10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n";
        run(in_str, out_str);

        std::string res_recive = out_str.str();
        assert(res_expext == out_str.str());
        std::cout << "test many balance OK\n";
    }
    {
        std::stringstream in_str;
        std::stringstream out_str;

        in_str << "1 -10 0";
        std::string res_expext =
            "in run: find kthStatic return nullptr. With kth=0 operation=-10";
        try {
            run(in_str, out_str);
        } catch (std::runtime_error &e) {
            assert(e.what() == res_expext);
        }
        std::cout << "test delete on empty OK\n";
    }
    {
        std::stringstream in_str;
        std::stringstream out_str;

        int count = 10000;
        in_str << count << " ";
        for (int i = 1; i < count + 1; i++) {
            in_str << i << " " << 0 << " ";
        }
        std::string res_expext;
        for (int i = 1; i < count + 1; i++) {
            res_expext += std::to_string(1) + "\n";
        }

        run(in_str, out_str);

        std::string res_recive = out_str.str();
        assert(res_expext == res_recive);
        std::cout << "test a lot of elements OK\n";
    }
    {
        std::stringstream in_str;
        std::stringstream out_str;

        int count = 10;
        in_str << 2 * count - 2 << " ";
        for (int i = 1; i < count + 1; i++) {
            in_str << i << " " << 0 << " ";
        }
        std::string res_expext;
        for (int i = 1; i < count + 1; i++) {
            res_expext += std::to_string(1) + "\n";
        }

        for (int i = 1; i < count; i++) {
            in_str << -i << " " << 0 << " ";
        }
        for (int i = 2; i < count; i++) {
            res_expext += std::to_string(i) + "\n";
        }

        run(in_str, out_str);

        std::string res_recive = out_str.str();
        assert(res_expext == res_recive);
        std::cout << "test elements added and deleted OK\n";
    }
    {
        AVLTree<int> tree;
        tree.insert(5);
        tree.insert(3);
        tree.insert(7);
        tree.insert(2);
        tree.insert(4);

        assert(*tree.kthStatistic(0) == 2);
        assert(*tree.kthStatistic(1) == 3);
        assert(*tree.kthStatistic(2) == 4);
        assert(*tree.kthStatistic(3) == 5);
        assert(*tree.kthStatistic(4) == 7);

        // Удаление элемента
        tree.erase(3);

        // Проверка изменения порядковых статистик
        assert(*tree.kthStatistic(0) == 2);
        assert(*tree.kthStatistic(1) == 4);
        assert(*tree.kthStatistic(2) == 5);
        assert(*tree.kthStatistic(3) == 7);

        assert(tree.kthStatistic(6) == nullptr);

        std::cout << "test basic work with tree OK" << std::endl;
    }
    {
        AVLTree<int> tree;
        tree.insert(5);
        tree.insert(3);
        tree.insert(7);
        tree.insert(2);
        tree.insert(4);

        assert(*tree.kthStatistic(0) == 2);
        assert(*tree.kthStatistic(1) == 3);
        assert(*tree.kthStatistic(2) == 4);
        assert(*tree.kthStatistic(3) == 5);
        assert(*tree.kthStatistic(4) == 7);

        // Удаление элемента
        tree.erase(3);
        tree.erase(5);

        // Проверка изменения порядковых статистик
        assert(*tree.kthStatistic(0) == 2);
        assert(*tree.kthStatistic(1) == 4);
        assert(*tree.kthStatistic(2) == 7);

        assert(tree.kthStatistic(3) == nullptr);
        assert(tree.kthStatistic(4) == nullptr);

        std::cout << "test basic work with tree with erase OK" << std::endl;
    }
    {
        AVLTree<int> tree;
        tree.insert(5);
        tree.insert(3);
        tree.insert(7);
        tree.insert(2);
        tree.insert(4);

        assert(*tree.kthStatistic(0) == 2);
        assert(*tree.kthStatistic(1) == 3);
        assert(*tree.kthStatistic(2) == 4);
        assert(*tree.kthStatistic(3) == 5);
        assert(*tree.kthStatistic(4) == 7);

        // Удаление элемента
        tree.erase(3);
        tree.erase(5);
        tree.erase(7);
        tree.erase(4);

        // Проверка изменения порядковых статистик
        assert(*tree.kthStatistic(0) == 2);

        assert(tree.kthStatistic(1) == nullptr);
        assert(tree.kthStatistic(2) == nullptr);

        std::cout << "test basic work with tree with erase above all OK"
                  << std::endl;
    }
    {
        AVLTree<int> tree;
        tree.insert(5);
        tree.insert(3);
        tree.insert(7);
        tree.insert(8);

        assert(*tree.kthStatistic(0) == 3);
        assert(*tree.kthStatistic(1) == 5);
        assert(*tree.kthStatistic(2) == 7);
        assert(*tree.kthStatistic(3) == 8);

        tree.erase(5);

        assert(*tree.kthStatistic(0) == 3);
        assert(*tree.kthStatistic(1) == 7);
        assert(*tree.kthStatistic(2) == 8);

        assert(tree.kthStatistic(3) == nullptr);
        assert(tree.kthStatistic(4) == nullptr);

        std::cout << "test tree erase right without left OK" << std::endl;
    }
    {
        AVLTree<int> tree;
        tree.insert(5);
        tree.insert(3);
        tree.insert(7);
        tree.insert(2);
        tree.insert(4);

        assert(*tree.find(3) == 3);
        assert(*tree.find(2) == 2);
        assert(*tree.find(5) == 5);
        assert(*tree.find(4) == 4);
        assert(*tree.find(7) == 7);
        assert(tree.find(6) == nullptr);

        std::cout << "test find OK" << std::endl;
    }

    std::cout << "testLogic OK\n";
}
};   // namespace

int main() {
    testLogic();

    try {
        run(std::cin, std::cout);
    } catch (std::exception &e) {
        std::cerr << "in main exception: " + static_cast<std::string>(e.what());
        return 1;
    }

    return 0;
}
