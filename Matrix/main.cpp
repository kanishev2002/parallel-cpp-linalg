#include <iostream>

#include <vector>
#include "Matrix.h"
#include "Matrix.cpp"

int main() {
  Matrix<int> matrix_1({{1, 2}, {3, 4}});
  Matrix<int> matrix_2 = matrix_1;
  std::cout << (matrix_1 == matrix_2) << '\n';
  std::cout << '\n';
  matrix_1.print(std::cout);
  std::cout << '\n';
  Matrix<int> matrix_3 = matrix_1 + matrix_2;
  matrix_3.print(std::cout);
  std::cout << '\n';
  //std::cout << matrix_1 << "\n";
  return 0;
}

