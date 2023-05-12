// Реализуйте структуру данных типа “множество строк” на основе динамической
// хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
// строчных латинских букв. Хеш-функция строки должна быть реализована с помощью
// вычисления значения многочлена методом Горнера. Начальный размер таблицы
// должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов
// в случае, когда коэффициент заполнения таблицы достигает 3/4. Структура
// данных должна поддерживать операции добавления строки в множество, удаления
// строки из множества и проверки принадлежности данной строки множеству. 1_1.

// Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
// g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.

// Формат входных данных
// Каждая строка входных данных задает одну операцию над множеством. Запись
// операции состоит из типа операции и следующей за ним через пробел строки, над
// которой проводится операция. Тип операции  – один из трех символов:
//     +  означает добавление данной строки в множество;
//     -  означает удаление  строки из множества;
//     ?  означает проверку принадлежности данной строки множеству.
// При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в
// этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он
// присутствует в этом множестве.

// Формат выходных данных Программа должна
//  вывести для каждой операции одну из двух строк OK или FAIL, в зависимости от
//  того, встречается ли данное слово в нашем множестве.

#include <assert.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

constexpr int CONST_HASH = 11;
constexpr float LOAD_FACTOR = 3.0F / 4;
constexpr int START_CAPACITY = 8;
constexpr size_t CODE_ERROR = -1;
constexpr float EPS = 0.00000001F;

using CodeReturn = size_t;

namespace {
size_t Hash(const std::string &str, size_t size) {
    size_t hash = 0;
    for (auto it = str.begin(); it < str.end(); it++) {
        hash = (hash * CONST_HASH + *it) % size;
    }

    return hash % size;
};

struct HashTableNode {
    HashTableNode() : flag_delete_(false){};
    explicit HashTableNode(const std::string &key)
        : key_(key), flag_delete_(false){};

    bool flag_delete_;
    std::string key_;

    bool isNotExist() const { return key_ == ""; };
};

class HashTable {
  public:
    HashTable()
        : buffer_(START_CAPACITY), cap_(START_CAPACITY), real_size_(0){};
    HashTable(size_t cap, size_t real_size)
        : buffer_(cap), cap_(cap), real_size_(real_size){};

    bool isExist(const std::string &key) const;
    CodeReturn insert(const std::string &key);
    CodeReturn deleteNode(const std::string &key);

  private:
    std::vector<HashTableNode> buffer_;

    size_t cap_;
    size_t real_size_;

    // size_t cap() const;
    bool isLoad() const;
    void reHash(size_t new_size);
};

size_t findInsertPos(std::vector<HashTableNode> &buffer,
                     const std::string &key) {
    size_t cap = buffer.size();
    size_t insert_pos = Hash(key, cap);
    bool flagFirstDelete = true;
    size_t result = insert_pos;
    for (size_t i = 1; i <= cap; i++) {
        HashTableNode &node = buffer[insert_pos];
        if (node.isNotExist()) {
            return insert_pos;
        }

        if (node.flag_delete_ && flagFirstDelete) {
            flagFirstDelete = false;
            result = insert_pos;
        }

        if (node.key_ == key && !node.flag_delete_) {
            return CODE_ERROR;
        }

        insert_pos = (insert_pos + i) % cap;
    }

    return result;
}

bool HashTable::isExist(const std::string &key) const {
    size_t cur_pos = Hash(key, cap_);
    for (size_t i = 1; i <= cap_; i++) {
        const HashTableNode &node = buffer_[cur_pos];
        if (node.isNotExist()) {
            return false;
        }

        if (node.key_ == key && !node.flag_delete_) {
            return true;
        }

        cur_pos = (cur_pos + i) % cap_;
    }

    return false;
}

CodeReturn HashTable::deleteNode(const std::string &key) {
    size_t delete_pos = Hash(key, cap_);
    for (size_t i = 1; i <= cap_; i++) {
        HashTableNode &node = buffer_[delete_pos];
        if (node.isNotExist()) {
            return CODE_ERROR;
        }

        if (node.key_ == key && !node.flag_delete_) {
            node.flag_delete_ = true;
            real_size_--;
            return 0;
        }

        delete_pos = (delete_pos + i) % cap_;
    }

    return CODE_ERROR;
}

CodeReturn HashTable::insert(const std::string &key) {
    if (key == "") {
        return CODE_ERROR;
    }

    if (isLoad()) {
        reHash(cap_ * 2);
    }

    size_t insert_pos = findInsertPos(buffer_, key);
    if (insert_pos == CODE_ERROR) {
        return CODE_ERROR;
    }
    buffer_[insert_pos].key_ = key;
    buffer_[insert_pos].flag_delete_ = false;

    real_size_++;

    return 0;
};

bool HashTable::isLoad() const {
    return ((static_cast<float>(real_size_) / cap_) - LOAD_FACTOR) > EPS;
};

void HashTable::reHash(size_t new_size) {
    auto new_buffer = std::vector<HashTableNode>(new_size);

    for (auto it = buffer_.begin(); it < buffer_.end(); it++) {
        if (!it->isNotExist() && !it->flag_delete_) {
            size_t insert_pos = findInsertPos(new_buffer, it->key_);
            new_buffer[insert_pos] = *it;
        }
    }

    buffer_ = move(new_buffer);
    HashTable::cap_ = new_size;
}

CodeReturn handleOperation(const std::string &operation, const std::string &arg,
                           HashTable &table) {
    if (operation == "+") {
        return table.insert(arg);
    } else if (operation == "-") {
        return table.deleteNode(arg);
    } else if (operation == "?") {
        if (!table.isExist(arg)) {
            return CODE_ERROR;
        }
        return 0;
    }

    return CODE_ERROR;
}

void run(std::istream &input, std::ostream &output) {
    auto table = HashTable();
    std::string operation;
    while (input >> operation) {
        std::string arg;
        input >> arg;

        CodeReturn code = handleOperation(operation, arg, table);
        if (code == CODE_ERROR) {
            output << "FAIL" << std::endl;
            continue;
        }

        output << "OK" << std::endl;
    }
}

void testLogic() {
    {
        std::string str("asdf");
        for (int i = 0; i < 100000; i++) {
            size_t res;
            res = ::Hash(str + std::to_string(i), 10);
            assert(res < 10);
        }
        std::cout << "test Hash() OK" << std::endl;
    }
    {
        auto table = HashTable();
        table.insert("aaa");
        assert(table.isExist("aaa"));

        table.deleteNode("aaa");
        assert(!table.isExist("aaa"));

        table.insert("aaa");
        assert(table.isExist("aaa"));

        std::cout << "test HashTable one insert and 2 delete OK" << std::endl;
    }
    {
        auto table = HashTable();
        std::string str("asdf");

        for (int i = 0; i < 100; i++) {
            size_t res;
            auto code = table.insert(str + std::to_string(i));
            assert(table.isExist(str + std::to_string(i)));
            assert(code != CODE_ERROR);
        }

        for (int i = 0; i < 100; i++) {
            size_t res;
            auto code = table.deleteNode(str + std::to_string(i));
            assert(!table.isExist(str + std::to_string(i)));
            assert(code != CODE_ERROR);
        }

        std::cout << "test HashTable a lot of insert OK" << std::endl;
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
