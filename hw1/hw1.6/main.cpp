// Дано множество целых чисел из [0..10^9] размера n.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти
// следующие параметры множества: 10%  перцентиль; медиана; 90%  перцентиль

// Требования: к дополнительной памяти: O(n).
// Среднее время работы: O(n)
// Должна быть отдельно выделенная функция partition.
// Рекурсия запрещена.
// Решение должно поддерживать передачу функции сравнения снаружи.

// Функцию Partition следует реализовывать методом прохода двумя итераторами в
// одном направлении. Описание для случая прохода от начала массива к концу:
// Выбирается опорный элемент. Опорный элемент меняется с последним элементом
// массива. Во время работы Partition в начале массива содержатся элементы, не
// бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного. В
// конце массива лежат нерассмотренные элементы. Последним элементом лежит
// опорный. Итератор (индекс) i указывает на начало группы элементов, строго
// бОльших опорного. Итератор j больше i, итератор j указывает на первый
// нерассмотренный элемент. Шаг алгоритма. Рассматривается элемент, на который
// указывает j. Если он больше опорного, то сдвигаем j. Если он не больше
// опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j. В конце
// работы алгоритма меняем опорный и элемент, на который указывает итератор i.

// 6_4. Реализуйте стратегию выбора опорного элемента “случайный
// элемент”. Функцию Partition реализуйте методом прохода двумя итераторами от
// конца массива к началу.

#include <iostream>

// Возвращает рандомное число от left до right включительно
int randomIdx(int left, int right) {
    if (right <= left) {
        throw std::runtime_error(
            "in randomIdx right <=left => can`t division by zero");
    }

    return left + (std::rand() % (right - left + 1));
}

// Partion методом прохода итераторов от конца к началу
template <typename T, typename Comparator>
int Partion(T *arr, int left, int right, Comparator comp) {
    if (left == right) {
        return left;
    }

    int pivot_idx = randomIdx(left, right);
    T pivot = arr[pivot_idx];

    std::swap(arr[pivot_idx], arr[left]);

    int i = right;
    int j = right - 1;

    while (j > left && !comp(arr[i], pivot)) {
        i--;
        j--;
    }

    while (j > left) {
        for (; comp(arr[j], pivot); j--) {
        }

        if (j > left) {
            std::swap(arr[j], arr[i]);
            i--;
            j--;
        }
    }

    // Условие на случай, если не было ни одного элемента с верным условием
    // comp(...), т.е если i не указывает на начало области с элементами строго
    // comp(...) чем разделитель. В таком случае не нужно swap
    if (!comp(arr[i], arr[left])) {
        return left;
    }
    std::swap(arr[i], arr[left]);

    return i;
}

template <typename T, typename Comparator>
T kth_element(T *arr, int size, int k, Comparator comp) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int m = Partion(arr, left, right, comp);

        if (m == k) {
            return arr[m];
        } else if (m < k) {
            left = m + 1;
        } else {
            right = m - 1;
        }
    }
}

int run(std::istream &input, std::ostream &output) {
    int count;
    input >> count;
    int *arr = new int[count];
    for (int i = 0; i < count; i++) {
        int tmp;
        input >> tmp;
        arr[i] = tmp;
    }

    auto comp = [](int &left, int &right) { return left < right; };
    output << kth_element(arr, count, count / 10, comp) << std::endl;
    output << kth_element(arr, count, count / 2, comp) << std::endl;
    output << kth_element(arr, count, 9 * count / 10, comp) << std::endl;

    delete[] arr;

    return 0;
}

int main() {
    // Для смены последовательности псевдослучайных чисел при каждом старте
    srand(time(NULL));
    return run(std::cin, std::cout);
}