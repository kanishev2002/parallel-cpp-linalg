#include "../../include/equation.h"

template <typename T>
std::vector<T> Equation::compose_rows(const std::vector<T>& row1,
                                      const std::vector<T>& row2) {
  if (row1.size() != row2.size()) {
    throw std::invalid_argument("Vectors have different size");
  }
  std::vector<T> result;
  for (size_t i = 0; i < row1.size(); ++i) {
    result.emplace_back(row1[i] + row2[i]);
  }
  return result;
}

template <typename T>
std::vector<T> Equation::multiply_by_scalar(const std::vector<T>& row,
                                            const T& scalar) {
  if (scalar == T(0)) {
    throw std::invalid_argument("Cannot multiply by zero");
  }
  std::vector<T> result = row;
  for (auto& elem : result) {
    elem *= scalar;
  }
  return result;
}

template <typename T>
void Equation::swap_rows(std::vector<T>& row1, std::vector<T>& row2) {
  std::swap(row1, row2);
}
