// Решение задач данного раздела предполагает использование кучи,
//     реализованной в виде шаблонного класса. Решение должно поддерживать
//     передачу функции сравнения снаружи. Куча должна быть динамической.
//
//     4.3 Планировщик процессов В операционной системе Technux есть планировщик
//         процессов.Каждый процесс характеризуется : приоритетом P временем,
//         которое он уже отработал t временем, которое необходимо для
//         завершения работы процесса T Планировщик процессов выбирает процесс с
//         минимальным значением P *(t + 1), выполняет его время P и кладет
//         обратно в очередь процессов.
//
//         Если выполняется условие t>= T,
//         то процесс считается завершенным и удаляется из очереди.Требуется
//         посчитать кол-во переключений процессора.Формат входных данных:
//         Сначала вводится кол-во процессов.После этого процессы в формате P T
//         Формат выходных данных: Кол-во переключений процессора.

#include <fstream>
#include <iostream>

// Здесь реализована структура динамический массив, похожий на слайс в
// гошке(взята с прошлых задач).

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

struct Process {
    int param_priority_;

    // Время, которое процессор уже отработал
    int worked_time_;

    // Время, которое нужно отработать до завершения процесса
    int total_time_;

    bool operator<(const Process &other) const {
        return this->calculPriority() < other.calculPriority();
    }

    // Чтобы Process можно было выводить в offstream
    friend std::ostream &operator<<(std::ostream &output,
                                    const Process &process) {
        output << "param_priority_ : " << process.param_priority_
               << ", worked_time_: " << process.worked_time_
               << ", total_time_: " << process.total_time_ << std::endl;

        return output;
    }

  private:
    int calculPriority() const { return param_priority_ * (worked_time_ + 1); }
};

template <typename T> struct DefaultComparator {
    bool operator()(const T &l, const T &r) const { return l < r; }
};

// Здесь начинается реализация бинарной кучи
template <typename T, typename Comparator = DefaultComparator<T>> class Heap {
  public:
    // init empty Heap
    Heap(Comparator comp = Comparator()) : comp_(comp){};

    // build heap in unordered mySlice
    Heap(mySlice<T> &sl, Comparator comp = Comparator())
        : comp_(comp), sl_(sl) {
        heapify();
    };

    // const чтобы снаружи нельзя было изменить элемент кучи в обход
    // реализованных возможностей
    const T &top();
    void pop();
    void push(const T &elem);

    bool isEmpty() const { return sl_.lenSl() == 0; };
    size_t size() const { return sl_.lenSl(); };

  private:
    void heapify();
    void siftUp(unsigned int i);
    void siftDown(unsigned int i);

    Comparator comp_;

    mySlice<T> sl_;
};

template <typename T, typename Comparator> const T &Heap<T, Comparator>::top() {
    if (this->isEmpty()) {
        throw std::runtime_error("in heap.top() can`t top() on empty Heap");
    }

    return sl_[0];
}

template <typename T, typename Comparator> void Heap<T, Comparator>::pop() {
    if (this->isEmpty()) {
        throw std::runtime_error("in heap.pop() can`t pop() on empty Heap");
    }
    std::swap(sl_[0], sl_[this->size() - 1]);
    sl_.pop();

    if (!this->isEmpty()) {
        this->siftDown(0);
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::push(const T &elem) {
    sl_.push(elem);
    siftUp(this->size() - 1);
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::siftDown(unsigned int i) {
    unsigned int left_idx = 2 * i + 1;
    unsigned int right_idx = 2 * i + 2;

    unsigned int min_idx = i;
    if (left_idx < this->size() && comp_(sl_[left_idx], sl_[min_idx])) {
        min_idx = left_idx;
    }
    if (right_idx < this->size() && comp_(sl_[right_idx], sl_[min_idx])) {
        min_idx = right_idx;
    }

    if (min_idx != i) {
        std::swap(sl_[min_idx], sl_[i]);
        siftDown(min_idx);
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::siftUp(unsigned int i) {
    while (i > 0) {
        int parent_idx = (i - 1) / 2;
        if (comp_(sl_[i], sl_[parent_idx])) {
            std::swap(sl_[i], sl_[parent_idx]);
            i = parent_idx;
        } else {
            break;
        }
    }
}

template <typename T, typename Comparator> void Heap<T, Comparator>::heapify() {
    for (int i = this->size() / 2 - 1; i >= 0; i--) {
        this->siftDown(i);
    }
};

// Вывод кучи
template <typename T, typename Comparator>
void printlnHeap(Heap<T, Comparator> heap) {
    int last = 0;
    for (int i = 0; i < heap.size(); i++) {
        if (i == last * 2 + 1) {
            std::cout << std::endl;
            last = i;
        }
        std::cout << heap.top() << " ";
        heap.pop();
    }
}

int calculSwitching(Heap<Process> &heap) {
    int counter = 0;
    while (!heap.isEmpty()) {
        Process tmp_Process = heap.top();
        heap.pop();
        tmp_Process.worked_time_ += tmp_Process.param_priority_;
        counter++;

        if (tmp_Process.worked_time_ < tmp_Process.total_time_) {
            heap.push(tmp_Process);
        }
    }

    return counter;
}

int run(std::istream &input, std::ostream &output) {
    int count = 0;
    input >> count;
    mySlice<Process> sl;
    for (int i = 0; i < static_cast<int>(count); i++) {
        int tmp_priority, tmp_total_time;
        input >> tmp_priority >> tmp_total_time;

        Process tmp_Process;
        tmp_Process.param_priority_ = tmp_priority;
        tmp_Process.total_time_ = tmp_total_time;
        tmp_Process.worked_time_ = 0;
        sl.push(tmp_Process);
    }

    int counter;

    Heap<Process> heap(sl);
    output << calculSwitching(heap);

    return 0;
}

int main() { return run(std::cin, std::cout); }