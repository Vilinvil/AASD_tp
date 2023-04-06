// 2_2. Дан массив целых чисел А[0..n - 1].
// Известно,что на интервале[0, m] значения массива строго возрастают, а на
// интервале[m, n - 1] строго убывают.
// Найти m за O(log m).Требования: Время работы O(log m).Внимание !В этой задаче
// сначала нужно определить диапазон для бинарного поиска размером порядка m с
// помощью экспоненциального поиска,а потом уже в нем делать бинарный поиск.

// 2 ≤ n ≤ 10000.

#include <assert.h>
#include <iostream>
#include <sstream>
#include <vector>

void testFindSeparator();

template <typename T> T max(T a, T b) { return a > b ? a : b; };

// Копирует count элементов из buf1 в buf2
template <typename T> void copyBuf(T *buf1, T *buf2, size_t count) {
    for (size_t i = 0; i < count; i++) {
        buf2[i] = buf1[i];
    }
};

// Мой динамический массив
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

    void push(const T &val) {
        if (this->len == this->cap) {
            grow(max(this->cap * 2, (size_t) 1));
        }

        this->buf[this->len] = val;
        this->len++;
    }

    void pop() {
        if (this->len > 0) {
            this->len--;
        }
    }

    T &operator[](size_t idx) { return buf[idx]; }

    // Возвращает кол-во элементов в слайсе
    size_t lenSl() const { return len; }

    // Возвращает емкость слайса
    size_t capSl() const { return cap; }

    // Вывести слайс
    void printSl() const {
        for (size_t i = 0; i < this->lenSl(); i++) {
            std::cout << this->buf[i] << " ";
        }

        std::cout << std::endl;
    }

  private:
    T *buf;
    size_t len;
    size_t cap;
};

// Диапазон индексов [left, right]
struct rangeIdx {
    int left;
    int right;
};

template <typename T> bool isSeparator(T left, T mid, T right) {
    return (mid > left) && (mid > right) ? true : false;
}

template <typename T> bool isIncrease(T left, T mid, T right) {
    return (mid > left) && (right > mid) ? true : false;
}

// Ищет диапазон индексов, включающий разделитель
template <typename T> rangeIdx exponentialSearch(mySlice<T> &sl) {
    if (sl[0] > sl[1]) {
        return rangeIdx{left : 0, right : 0};
    } else if (sl[1] > sl[0] && sl[1] > sl[2]) {
        return rangeIdx{left : 1, right : 1};
    }

    int prev_idx = 2;
    int cur_idx = 2;
    size_t len_sl = sl.lenSl();
    for (; isIncrease(sl[cur_idx - 1], sl[cur_idx], sl[cur_idx + 1]) &&
           cur_idx < len_sl;) {
        prev_idx = cur_idx;
        cur_idx *= 2;
    }

    if (cur_idx > len_sl) {
        return rangeIdx{left : prev_idx, right : (int) len_sl - 1};
    }

    return rangeIdx{left : prev_idx, right : cur_idx};
}

template <typename T>
T binarySearchSeparator(int left_idx, int right_idx, mySlice<T> &sl) {
    int mid = (right_idx + left_idx) / 2;

    if (isSeparator(sl[mid - 1], sl[mid], sl[mid + 1])) {
        return mid;
    } else if (isIncrease(sl[mid - 1], sl[mid], sl[mid + 1])) {
        return binarySearchSeparator(mid + 1, right_idx, sl);
    }

    return binarySearchSeparator(left_idx, mid - 1, sl);
}

template <typename T> T findSeparator(mySlice<T> &sl) {
    if (sl.lenSl() == 0) {
        return -1;
    }

    rangeIdx range = exponentialSearch(sl);
    if (range.left == range.right) {
        return range.left;
    }

    return binarySearchSeparator(range.left, range.right, sl);
}

int run(std::istream &input, std::ostream &output) {
    int n;
    input >> n;
    mySlice<int> sl;
    for (int i = 0; i < n; i++) {
        int num;
        input >> num;
        sl.push(num);
    }

    output << findSeparator(sl);

    return 0;
};

int main() {
    // testFindSeparator();

    return run(std::cin, std::cout);
}

// Вспомогательная ф-ция для тестов,
// заполняющая слайс count шт. элементами с разеделитем по индексу sep
// template <typename T>
// void fillArrWithSeparator(mySlice<T> &sl, int count, int sep) {
//     for (int i = 0; i < sep; i++) {
//         sl.push(i);
//     }

//     for (int i = sep; i < count; i++) {
//         sl.push(2 * sep - i);
//     }
// }

// void testFindSeparator() {
//     {
//         for (int sep = 0; sep < 20; sep++) {
//             mySlice<int> sl(nullptr, 0, 0);
//             fillArrWithSeparator(sl, 20, sep);
//             assert(findSeparator(sl) == sep);
//         }
//     }
//     {
//         mySlice<int> sl;
//         sl.push(-10);
//         sl.push(-5);
//         sl.push(-2);
//         sl.push(-100);
//         sl.push(-1000);
//         assert(findSeparator(sl) == 2);
//     }
//     {
//         mySlice<int> sl;
//         sl.push(-10);
//         sl.push(-5);
//         sl.push(-2);
//         sl.push(100);
//         sl.push(1000);
//         assert(findSeparator(sl) == 4);
//     }
//     {
//         mySlice<int> sl;
//         assert(findSeparator(sl) == -1);
//     }

//     std::cout << "testFindSeparator OK\n";
// }