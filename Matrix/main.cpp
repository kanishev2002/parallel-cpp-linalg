#include <iostream>

#include "Matrix.cpp"
#include "Matrix.h"

int main() {
  std::cout << "Hello, World!" << std::endl;
  Matrix<int> matrix({{1, 2}, {3, 4}});
  std::cout << (matrix == matrix) << '\n';
  return 0;
}
