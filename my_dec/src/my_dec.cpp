#include <iostream>
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