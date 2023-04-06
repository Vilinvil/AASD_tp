// 7_3. Binary MSD для long long.

// Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не
// больше 1000000. Отсортировать массив методом MSD по битам (бинарный
// QuickSort).

#include <fstream>
#include <iostream>

// Для тестов подключаю
#include <algorithm>
#include <assert.h>
#include <climits>
#include <sstream>

// Здесь реализована структура динамический массив, похожий на слайс в гошке.
// (взята с прошлых примеров)

template <typename T> T max(T a, T b) { return a > b ? a : b; };

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

// Здесь начинается непосредственно решение задачи 7

// Вычисляет позицию старшего разряда(нумерация с нуля)
int calculMSD(unsigned long long num) {
    int counter = 0;
    for (; num >> 1 != 0; counter++) {
        num = num >> 1;
    }

    return counter;
}

// Partion методом прохода итераторов от конца к началу. Для разряда с индексом
// max_digit
template <typename T, typename Comparator>
void PartionDigit(mySlice<T> &sl, int left, int right, int max_digit,
                  Comparator comp) {
    if (left >= right || max_digit < 0) {
        return;
    }

    T pivot = static_cast<unsigned long long>(1) << max_digit;

    int it1 = right;
    int it2 = right - 1;
    while (it1 >= left && !comp(sl[it1], pivot, max_digit)) {
        it1--;
        it2--;
    }

    while (it2 >= left) {
        for (; it2 >= left && comp(sl[it2], pivot, max_digit); it2--) {
        }

        if (it2 >= left) {
            std::swap(sl[it2], sl[it1]);
            it1--;
            it2--;
        }
    }

    PartionDigit(sl, left, it1, max_digit - 1, comp);
    PartionDigit(sl, it1 + 1, right, max_digit - 1, comp);

    return;
}

void sortBinaryQuick(mySlice<unsigned long long> &sl) {
    if (sl.lenSl() == 0) {
        throw std::runtime_error("in sortBinaryQuick can`t sort empty slice");
    }

    unsigned long long max_el = sl[0];
    for (size_t i = 0; i < sl.lenSl(); i++) {
        if (sl[i] > max_el) {
            max_el = sl[i];
        }
    }

    // Ниже привожу к инту, т.к. по условию чисел по количеству меньше 10^6 =>
    // вмещаются в int
    int max_digit = calculMSD(max_el);
    PartionDigit(sl, 0, static_cast<int>(sl.lenSl() - 1), max_digit,
                 [](const unsigned long long &l, const unsigned long long &r,
                    unsigned int digit) {
                     return ((l >> digit) % 2) < ((r >> digit) % 2);
                 });
}

int run(std::istream &input, std::ostream &output) {
    int count;
    input >> count;
    mySlice<unsigned long long> sl;
    for (int i = 0; i < count; i++) {
        unsigned long long cur;
        input >> cur;
        sl.push(cur);
    }

    sortBinaryQuick(sl);

    for (int i = 0; i < count - 1; i++) {
        output << sl[i] << " ";
    }
    output << sl[count - 1];

    return 0;
}

void testSortBinaryQuick();

int main() {
    testSortBinaryQuick();
    return run(std::cin, std::cout);
}

// Тут начинаются тесты.
void testSortBinaryQuick() {
    srand(time(NULL));
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;
        for (int i = 0; i < 1000000; i++) {
            unsigned long long tmp = rand();
            actualSl.push(tmp);
            expectedVec.push_back(tmp);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < 1000000; i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test many big random numbers OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;
        std::stringstream ss;
        std::string s =
            "10 7459114088365490217 8263591664936569569 7922137723913136466 "
            "138246996836965105 1530118705988897715 43082431356239495 "
            "5268160487553368356 6115348740006432006 5604737764312627635 "
            "2784986586066674755";
        ss << s;
        int count;
        ss >> count;
        for (int i = 0; i < count; i++) {
            unsigned long long tmp;
            ss >> tmp;
            actualSl.push(tmp);
            expectedVec.push_back(tmp);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < count; i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test 3 from yandex OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;
        for (int i = 0; i < 1000000; i++) {
            unsigned long long tmp = ULLONG_MAX - i * 7 - 1;
            actualSl.push(tmp);
            expectedVec.push_back(tmp);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < 1000000; i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test many big numbers near ULLONG_MAX OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        actualSl.push(18446744073709551615ULL);
        actualSl.push(18446744073709551614ULL);
        actualSl.push(0);
        std::vector<unsigned long long> expectedVec;
        expectedVec.push_back(18446744073709551615ULL);
        expectedVec.push_back(18446744073709551614ULL);
        expectedVec.push_back(0);

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < actualSl.lenSl(); i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test small number at the end OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        actualSl.push(0);
        actualSl.push(18446744073709551614ULL);
        actualSl.push(18446744073709551615ULL);
        std::vector<unsigned long long> expectedVec;
        expectedVec.push_back(0);
        expectedVec.push_back(18446744073709551614ULL);
        expectedVec.push_back(18446744073709551615ULL);

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < actualSl.lenSl(); i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test small number at the begin OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;

        for (int i = 0; i < 1000000; i++) {
            actualSl.push(1234);
            expectedVec.push_back(1234);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < actualSl.lenSl(); i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test many same numbers OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;

        for (int i = 0; i < 1000000; i++) {
            if (i % 2 == 0) {
                actualSl.push(i - 1);
                expectedVec.push_back(i - 1);
            }
            actualSl.push(i);
            expectedVec.push_back(i);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < actualSl.lenSl(); i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test many odd numbers OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;

        for (int i = 0; i < 1000000; i++) {
            if (i % 2 == 0) {
                actualSl.push(i * 2);
                expectedVec.push_back(i * 2);
            }
            actualSl.push(i);
            expectedVec.push_back(i);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < actualSl.lenSl(); i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test many alternating numbers OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;

        for (int i = 0; i < 1000000; i++) {
            actualSl.push(i);
            expectedVec.push_back(i);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < actualSl.lenSl(); i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test ordered slice OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;

        for (int i = 0; i < 1000000; i++) {
            actualSl.push(i);
            expectedVec.push_back(i);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);
        assert(actualSl.lenSl() == expectedVec.size());
        std::cout << "test correct len of slice OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;

        for (int i = 1000; i >= 1; i--) {
            actualSl.push(0);
            expectedVec.push_back(0);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < actualSl.lenSl(); i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test zero slice OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;

        for (int i = 2000000; i >= 0; i--) {
            actualSl.push(i);
            expectedVec.push_back(i);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < actualSl.lenSl(); i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test more than 10^6 numbers in slice OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;

        for (int i = 64; i >= 0; i--) {
            actualSl.push(1 << i);
            expectedVec.push_back(1 << i);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < actualSl.lenSl(); i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test powers of two in slice OK\n";
    }
    {
        mySlice<unsigned long long> actualSl;
        std::vector<unsigned long long> expectedVec;

        for (int i = 64; i >= 0; i--) {
            actualSl.push((1 << i) - 1);
            expectedVec.push_back((1 << i) - 1);
        }

        std::sort(expectedVec.begin(), expectedVec.end(),
                  [](const unsigned long long &l, const unsigned long long &r) {
                      return l < r;
                  });
        sortBinaryQuick(actualSl);

        for (int i = 0; i < actualSl.lenSl(); i++) {
            assert(actualSl[i] == expectedVec[i]);
        }
        std::cout << "test 2^x -1 in slice OK\n";
    }
    {
        mySlice<unsigned long long> sl;
        sl.push(4);
        sortBinaryQuick(sl);
        assert(sl[0] == 4);
        std::cout << "test single number OK\n";
    }
    {
        mySlice<unsigned long long> sl;
        sl.push(1);
        sl.push(0);
        sortBinaryQuick(sl);
        assert(sl[0] == 0);
        assert(sl[1] == 1);
        std::cout << "test [1 0] slice OK\n";
    }
    {
        mySlice<unsigned long long> sl;
        sl.push(0);
        sl.push(1);
        sortBinaryQuick(sl);
        assert(sl[0] == 0);
        assert(sl[1] == 1);
        std::cout << "test [0 1] slice number OK\n";
    }
    {
        mySlice<unsigned long long> sl;
        try {
            sortBinaryQuick(sl);
        } catch (std::runtime_error &e) {
            assert(static_cast<std::string>(e.what()) ==
                   "in sortBinaryQuick can`t sort empty slice");
            assert(sl.lenSl() == 0);
        }
        std::cout << "test exception on empty slice OK\n";
    }

    std::cout << "testSortBinaryQuick OK";
}