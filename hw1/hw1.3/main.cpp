// Во всех задачах из следующего списка следует написать структуру данных,
// обрабатывающую команды push_back* и pop*. Формат входных данных. В первой
// строке количество команд n. n ≤ 1000000. Каждая команда задаётся как 2 целых
// числа: a b. a = 1 - push_back front a = 2 - pop front a = 3 - push_back back
// a = 4 - pop back Команды добавления элемента 1 и 3 заданы с неотрицательным
// параметром b. Для очереди используются команды 2 и 3. Для дека используются
// все четыре команды. Если дана команда pop*, то число b - ожидаемое значение.
// Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
// Формат выходных данных. Требуется напечатать YES - если все ожидаемые
// значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то
// напечатать NO.

// 3_2. Реализовать дек с динамическим зацикленным буфером.
// Требования: Дек должен быть реализован в виде класса.

#include <assert.h>
#include <fstream>
#include <iostream>

#define COMANND_PUSH_FRONT 1
#define COMANND_POP_FRONT  2
#define COMANND_PUSH_BACK  3
#define COMANND_POP_BACK   4

// Здесь реализована структура дек, на массиве с динамическим зацикленным
// буфером.

template <typename T> T max(T a, T b) { return a > b ? a : b; };

// Мой динамически расширяемый дек.
template <typename T> class myDec {
  public:
    myDec()
        : head_buf_(nullptr), real_head_(nullptr), real_tail_(nullptr), len_(0),
          cap_(0) {}

    myDec(T *head_buf, size_t len, size_t cap)
        : head_buf_(head_buf), real_head_(head_buf), real_tail_(head_buf),
          len_(len), cap_(cap) {}

    ~myDec() { delete[] head_buf_; }

    myDec(const myDec &other)
        : head_buf_(new T[other.cap_]), len_(other.len_), cap_(other.cap_) {
        other.copyBuf(head_buf_);
    }

    myDec &operator=(const myDec &other) {
        if (this != &other) {
            T *tmp = new T[other.cap_];
            other.copyBuf(tmp);
            delete[] head_buf_;

            len_ = other.len_;
            cap_ = other.cap_;

            head_buf_ = tmp;
            real_head_ = head_buf_;
            real_tail_ = head_buf_ + len_ - 1;
        }
        return *this;
    }

    void grow(size_t new_cap) {
        if (new_cap > cap_) {
            T *new_buf = new T[new_cap];
            this->copyBuf(new_buf);
            delete[] head_buf_;

            head_buf_ = new_buf;
            real_head_ = new_buf;

            if (len_ == 0) {
                real_tail_ = new_buf;
            } else {
                real_tail_ = new_buf + len_ - 1;
            }

            cap_ = new_cap;
        }
    }

    // Добавление элемента в хвост дека
    void push_back(const T &val) {
        if (len_ == cap_) {
            grow(max(cap_ * 2, (size_t) 1));
        }

        if (len_ == 0) {
            *real_head_ = val;
            len_++;
        } else if (real_tail_ == head_buf_ + cap_ - 1 &&
                   head_buf_ != real_head_) {
            real_tail_ = head_buf_;
            *real_tail_ = val;
            len_++;
        } else {
            real_tail_++;
            *real_tail_ = val;
            len_++;
        }
    }

    // Добавление элемента в голову дека
    void push_front(const T &val) {
        if (len_ == cap_) {
            grow(max(cap_ * 2, (size_t) 1));
        }

        if (len_ == 0) {
            *real_head_ = val;
            len_++;
        } else if (real_head_ == head_buf_ &&
                   head_buf_ + cap_ - 1 != real_tail_) {
            real_head_ = head_buf_ + cap_ - 1;
            *real_head_ = val;
            len_++;
        } else {
            real_head_--;
            *real_head_ = val;
            len_++;
        }
    }

    // Удаление элемента из хвоста дека
    T pop_back() {
        if (len_ == 0) {
            return -1;
        }

        T res = *real_tail_;
        if (len_ == 1) {
            len_--;
        } else if (real_tail_ == head_buf_) {
            real_tail_ = head_buf_ + cap_ - 1;
            len_--;
        } else {
            real_tail_--;
            len_--;
        }

        return res;
    }

    // Удаление элемента из головы дека
    T pop_front() {
        if (len_ == 0) {
            return -1;
        }

        T res = *real_head_;
        if (len_ == 1) {
            len_--;
        } else if (real_head_ == head_buf_ + (cap_ - 1)) {
            real_head_ = head_buf_;
            len_--;
        } else {
            real_head_++;
            len_--;
        }

        return res;
    }

    // Получить элемент по индексу (не слишком безопасно, можно вытащить
    // удаленный элемент или вообще за пределами)
    T &operator[](size_t idx) { return head_buf_[idx]; }

    // Возвращает кол-во элементов в слайсе
    size_t lenSl() const { return len_; }

    // Возвращает емкость слайса
    size_t capSl() const { return cap_; }

    // Вывести дек
    void printDec() {
        int dif = head_buf_ + cap_ - 1 - real_head_;

        for (int i = 0; i < len_; i++) {
            for (; i <= dif; i++) {
                std::cout << real_head_[i] << " ";
            }
            std::cout << head_buf_[i] << " ";
        }
        std::cout << std::endl;
    }

  private:
    // указатель на начало массива
    T *head_buf_;

    T *real_head_;
    T *real_tail_;

    size_t len_;
    size_t cap_;

    // Копирует len элементов из head_buf в new_buf
    void copyBuf(T *new_buf) {
        int dif = (head_buf_ + cap_) - real_head_;
        for (int i = 0; i < len_; i++) {
            for (; i < dif; i++) {
                new_buf[i] = real_head_[i];
            }
            new_buf[i] = head_buf_[i - dif];
        }
    };
};

int run(std::istream &input, std::ostream &output) {
    int count = 0;
    input >> count;
    myDec<int> dec;
    for (int i = 0; i < count; i++) {
        int comand, arg;
        input >> comand >> arg;

        switch (comand) {
        case COMANND_PUSH_FRONT:
            dec.push_front(arg);
            break;
        case COMANND_POP_FRONT: {
            int res = dec.pop_front();
            if (res != arg) {
                output << "NO";
                return 0;
            }
            break;
        }
        case COMANND_PUSH_BACK:
            dec.push_back(arg);
            break;
        case COMANND_POP_BACK: {
            int res = dec.pop_back();
            if (res != arg) {
                output << "NO";
                return 0;
            }
            break;
        }
        default:
            throw "in run: Unknown comand";
        }
    }

    output << "YES";

    return 0;
}

// void testDec();

int main() {
    // testDec();
    return run(std::cin, std::cout);
}

// Начало тестов:
// void testDec() {
//     {
//         myDec<int> dec;
//         for (int i = 0; i < 20; i++) {
//             dec.push_back(i);
//             int res = dec.pop_back();
//             assert(res == i);
//         }
//     }
//     {
//         myDec<int> dec;
//         for (int i = 0; i < 5; i++) {
//             dec.push_back(i);
//             int res = dec.pop_back();
//             assert(res == i);
//         }

//         for (int i = 0; i < 10; i++) {
//             int res = dec.pop_back();
//             assert(res == -1);
//         }
//     }
//     {
//         myDec<int> dec;
//         for (int i = 0; i < 10; i++) {
//             dec.push_back(i);
//             dec.push_front(2 * i);
//         }

//         for (int i = 9; i >= 0; i--) {
//             int res_back = dec.pop_back();
//             int res_front = dec.pop_front();
//             assert(2 * res_back == res_front);
//         }
//     }
//     {
//         myDec<int> dec;
//         for (int i = 0; i < 5; i++) {
//             dec.push_back(i);
//         }

//         for (int i = 0; i > 5; i++) {
//             int res_front = dec.pop_front();
//             assert(i == res_front);
//         }
//     }

//     std::cout << "testDec OK\n";
// }