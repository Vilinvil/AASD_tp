#include <iostream>

// Здесь реализована структура динамический массив, похожий на слайс в гошке.

template <typename T> T max(T a, T b) { return a > b ? a : b; };

// Копирует count элементов из buf1 в buf2
template <typename T> void copyBuf(T *buf1, T *buf2, size_t count) {
    for (int i = 0; i < count; i++) {
        buf2[i] = buf1[i];
    }
};

// Vой динамический массив
template <typename T> class mySlice {
  public:
    mySlice() : buf_(nullptr), len_(0), cap_(0) {}
    mySlice(T *buf, size_t len, size_t cap) : buf_(buf), len_(len), cap_(cap) {}

    ~mySlice() { delete[] buf_; }

    mySlice(const mySlice &other)
        : buf_(new T[other.cap_]), len_(other.len_), cap_(other.cap_) {
        copyBuf(other.buf_, this->buf_, other.len_);
    }

    mySlice &operator=(const mySlice &other) {
        if (this != &other) {
            T *tmp = new T[other.cap_];
            copyBuf(other.buf_, tmp, other.len_);
            delete[] this->buf_;

            this->buf_ = tmp;
            this->len_ = other.len_;
            this->cap_ = other.cap_;
        }
        return *this;
    }

    void grow(size_t new_cap) {
        if (new_cap > cap_) {
            T *new_buf = new T[new_cap];
            copyBuf(this->buf_, new_buf, len_);
            delete[] this->buf_;

            this->buf_ = new_buf;
            this->cap_ = new_cap;
        }
    }

    // Добавление элемента в слайс
    void push(const T &val) {
        if (this->len_ == this->cap_) {
            grow(max(this->cap_ * 2, (size_t) 1));
        }

        this->buf_[this->len_] = val;
        this->len_++;
    }

    // Удаление элемента из слайса
    void pop() {
        if (this->len_ > 0) {
            this->len_--;
        }
    }

    // Получить элемент по индексу. Не безопасная операция
    T &operator[](size_t idx) {
        if (idx >= len_ && idx < 0) {
            throw std::runtime_error("index out of range")
        }
        return buf_[idx];
    }

    // Возвращает кол-во элементов в слайсе
    size_t lenSl() const { return len_; }

    // Возвращает емкость слайса
    size_t capSl() const { return cap_; }

  private:
    T *buf_;
    size_t len_;
    size_t cap_;
};

// Вывести слайс
template <typename T> void printSl(mySlice<T> &sl) {
    for (size_t i = 0; i < sl.lenSl(); i++) {
        std::cout << sl.buf_[i] << " ";
    }

    std::cout << std::endl;
}

int main() {
    mySlice<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);

    std::cout << "s: ";
    s.pop();
    printSl(s);

    mySlice<int> s_new(s);
    std::cout << "s_new: ";
    printSl(s_new);

    mySlice<int> s_copy = s;
    std::cout << "s_copy: ";
    printSl(s_copy);

    return 0;
}