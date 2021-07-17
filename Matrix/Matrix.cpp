#include "Matrix.h"

#include <atomic>
#include <iostream>
#include <thread>

template <typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>> matrix) {
  matrix_ = matrix;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) const {
  if (shape() != other.shape()) {
    return false;
  }

  std::atomic<bool> equal = true;
  auto compare_vectors = [&](const auto& row1, const auto& row2) {
    for (size_t i = 0; i < row1.size(); ++i) {
      if (!equal.exchange(row1[i] == row2[i])) {
        return false;
      }
    }
    return true;
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i < shape().first; ++i) {
    threads.emplace_back(
        [&]() { equal.store(equal.load() && compare_vectors); }, matrix_[i],
        other.matrix_[i]);
  }
  for (auto& thr : threads) {
    thr.join();
  }
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T>& other) const {
  return !(*this == other);
}

template <typename T>
void Matrix<T>::print(std::ostream& out, char column_separator,
                      char row_separator) const {
  const auto [rows, columns] = shape();
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      out << matrix_[i][j] << column_separator;
    }
    out << row_separator;
  }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& mat) {
  mat.print(out, '\t', '\n');
  return out;
}
