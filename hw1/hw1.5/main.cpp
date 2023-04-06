// Требование для всех вариантов Задачи 5
// Во всех задачах данного раздела необходимо реализовать и использовать
// сортировку слиянием в виде шаблонной функции. Решение должно поддерживать
// передачу функции сравнения снаружи. Общее время работы алгоритма O(n log n).

// 5_4. Закраска прямой 2.
// На числовой прямой окрасили N отрезков. Известны координаты левого и правого
// концов каждого отрезка (Li и Ri). Найти сумму длин частей числовой прямой,
// окрашенных ровно в один слой.

#include <iostream>

// Здесь вначале реализована структура динамический массив, похожий на слайс в
// гошке. (взята с прошлых задач)

template <typename T> T max(T a, T b) { return a > b ? a : b; };
template <typename T> T min(T a, T b) { return a < b ? a : b; };

// Копирует count элементов из buf1 в buf2
template <typename T> void copyBuf(T *buf1, T *buf2, size_t count) {
    for (int i = 0; i < count; i++) {
        buf2[i] = buf1[i];
    }
};

// Мой динамический массив
template <typename T> class mySlice {
  public:
    mySlice() : buf_(nullptr), len_(0), cap_(0) {}

    mySlice(size_t cap) : buf_(new T[cap]), len_(0), cap_(cap) {}

    mySlice(T *buf, size_t len, size_t cap) : buf_(buf), len_(len), cap_(cap) {}

    mySlice(const mySlice &other)
        : buf_(new T[other.cap_]), len_(other.len_), cap_(other.cap_) {
        copyBuf(other.buf_, this->buf_, other.len_);
    }

    ~mySlice() { delete[] buf_; }

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
    T &operator[](size_t idx) { return buf_[idx]; }

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
        std::cout << sl[i] << " ";
    }

    std::cout << std::endl;
}

// Здесь начинается реализация сортировки слияния без рекурсии
template <typename T, typename Comparator>
void merge(mySlice<T> &sl, int left_idx, int mid_idx, int right_idx,
           Comparator comp) {
    int it1 = 0;
    int it2 = 0;
    mySlice<T> res_sl(static_cast<size_t>(right_idx - left_idx));

    while (left_idx + it1 < mid_idx && mid_idx + it2 < right_idx) {
        if (comp(sl[left_idx + it1], sl[mid_idx + it2])) {
            res_sl[it1 + it2] = sl[left_idx + it1];
            it1++;
        } else {
            res_sl[it1 + it2] = sl[mid_idx + it2];
            it2++;
        }
    }

    for (; left_idx + it1 < mid_idx; it1++) {
        res_sl[it1 + it2] = sl[left_idx + it1];
    }
    for (; mid_idx + it2 < right_idx; it2++) {
        res_sl[it1 + it2] = sl[mid_idx + it2];
    }

    for (int i = 0; i < it1 + it2; i++) {
        sl[left_idx + i] = res_sl[i];
    }
}

template <typename T, typename Comparator>
void mergeSort(mySlice<T> &sl, Comparator comp) {
    for (int chank_size = 1; chank_size < sl.lenSl(); chank_size *= 2) {
        for (int left_idx = 0; left_idx < (sl.lenSl() - chank_size);
             left_idx += 2 * chank_size) {
            merge(sl, left_idx, left_idx + chank_size,
                  min(left_idx + 2 * chank_size, static_cast<int>(sl.lenSl())),
                  comp);
        }
    }
}

// Слой закраски прямой
struct Layer {
    // Координата начала слоя
    int start_;
    // Толщина слоя
    int thickness_;

    Layer() : start_(0), thickness_(0){};
    Layer(int start, int thickness) : start_(start), thickness_(thickness){};

    bool operator<(const Layer &other) const { return start_ < other.start_; }
};

// Вычисляет суммарную длинну частей закрашенных ровно в один слой
int calculateSumLen(mySlice<Layer> &slLayers) {
    mergeSort(slLayers, [](const Layer &l, const Layer &r) { return l < r; });

    int prevStart = 0;
    int sumThickness = 0;
    int sumLen = 0;
    for (int i = 0; i < slLayers.lenSl(); i++) {
        Layer curLayer = slLayers[i];
        if (sumThickness == 1) {
            sumLen += curLayer.start_ - prevStart;
        }
        sumThickness += curLayer.thickness_;
        prevStart = curLayer.start_;
    }

    return sumLen;
}

int run(std::istream &input, std::ostream &output) {
    int count;
    input >> count;
    mySlice<Layer> slLayers;
    for (int i = 0; i < count; i++) {
        int start, end;
        input >> start >> end;

        slLayers.push(Layer(start, 1));
        slLayers.push(Layer(end, -1));
    }

    output << calculateSumLen(slLayers);

    return 0;
}

// void testSortMerge();

int main() {
    // testSortMerge();
    return run(std::cin, std::cout);
}

// Здесь начинаются тесты
// void testSortMerge() {
//     {
//         mySlice<int> sl;
//         sl.push(-1);
//         sl.push(1);
//         sl.push(34);
//         sl.push(-10);
//         sl.push(-100);

//         printSl(sl);
//         mergeSort(sl, [](const int &l, const int &r) { return l > r; });
//         printSl(sl);
//     }
// }
