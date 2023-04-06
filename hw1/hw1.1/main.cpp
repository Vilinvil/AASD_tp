// Во всех задачах необходимо использование битовых операций.Использование
// арифметических операций запрещено.Входное число лежит в диапазоне 0..2^32-1 и
// вводится в десятичном виде.
//
// 1.1 Подсчитать кол-во единичных бит в входном числе, стоящих на четных
// позициях.Позиции битов нумеруются с 0.

// Тесты закоментированы

#include <assert.h>
#include <iostream>
#include <sstream>

unsigned int calculSumEvenPositions(unsigned int num) {
    int res = 0;
    for (; num > 0;) {
        res += num & 1;
        num = num >> 2;
    }

    return res;
}

int run(std::istream &input, std::ostream &output) {
    unsigned int num;
    input >> num;
    output << calculSumEvenPositions(num);

    return 0;
};

// void testLogic() {
//     {
//         std::stringstream input;
//         std::stringstream output;
//         input << 4294967295;
//         assert(run(input, output) == 0);
//         assert(output.str() == "16");
//     }
//     {
//         std::stringstream input;
//         std::stringstream output;
//         input << 0;
//         assert(run(input, output) == 0);
//         assert(output.str() == "0");
//     }
//     {
//         std::stringstream input;
//         std::stringstream output;
//         input << 8;
//         assert(run(input, output) == 0);
//         assert(output.str() == "0");
//     }
//     {
//         std::stringstream input;
//         std::stringstream output;
//         input << 4;
//         assert(run(input, output) == 0);
//         assert(output.str() == "1");
//     }
//     {
//         std::stringstream input;
//         std::stringstream output;
//         input << 7;
//         assert(run(input, output) == 0);
//         assert(output.str() == "2");
//     }

//     std::cout << "testLogic OK\n";
// }

int main() {
    // testLogic();

    return run(std::cin, std::cout);
}
