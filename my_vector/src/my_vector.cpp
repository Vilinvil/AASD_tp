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
    mySlice() : buf(nullptr), len(0), cap(0) {}
    mySlice(T *buf, size_t len, size_t cap) : buf(buf), len(len), cap(cap) {}

    ~mySlice() { delete[] buf; }

    mySlice(const mySlice &other)
        : buf(new T[other.cap]), len(other.len), cap(other.cap) {
        copyBuf(other.buf, this->buf, other.len);
    }

    mySlice &operator=(const mySlice &other) {
        if (this != &other) {
            T *tmp = new T[other.cap];
            copyBuf(other.buf, tmp, other.len);
            delete[] this->buf;

            this->buf = tmp;
            this->len = other.len;
            this->cap = other.cap;
        }
        return *this;
    }

    void grow(size_t new_cap) {
        if (new_cap > cap) {
            T *new_buf = new T[new_cap];
            copyBuf(this->buf, new_buf, len);
            delete[] this->buf;

            this->buf = new_buf;
            this->cap = new_cap;
        }
    }

    // Добавление элемента в слайс
    void push(const T &val) {
        if (this->len == this->cap) {
            grow(max(this->cap * 2, (size_t) 1));
        }

        this->buf[this->len] = val;
        this->len++;
    }

    // Удаление элемента из слайса
    void pop() {
        if (this->len > 0) {
            this->len--;
        }
    }

    // Получить элемент по индексу
    T &operator[](size_t idx) { return buf[idx]; }

    // Возвращает кол-во элементов в слайсе
    size_t lenSl() const { return len; }

    // Возвращает емкость слайса
    size_t capSl() const { return cap; }

  private:
    T *buf;
    size_t len;
    size_t cap;
};

// Вывести слайс
template <typename T> void printSl(mySlice<T> &sl) {
    for (size_t i = 0; i < sl.lenSl(); i++) {
        std::cout << sl.buf[i] << " ";
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
    s.printSl();

    mySlice<int> s_new(s);
    std::cout << "s_new: ";
    s_new.printSl();

    mySlice<int> s_copy = s;
    std::cout << "s_copy: ";
    s_copy.printSl();

    return 0;
}