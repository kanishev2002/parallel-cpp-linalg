#include <iostream>
#include "Matrix.h"
#include "Matrix.cpp"

#include <atomic>
#include <thread>
#include <string>

int main() {
    Matrix<int> matrix({{1, 2},
                        {3, 4}});
    matrix.print(std::cout, '\t', '\n');
    std::cout << "\n" << matrix << "\n";
    return 0;
}
