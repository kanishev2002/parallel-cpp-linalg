#include "Matrix.h"

#include <atomic>
#include <thread>

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) const {
  if (shape() != other.shape()) {
    return false;
  }

  std::atomic<bool> equal = true;
  auto compare_vectors = [&](const auto& row1, const auto& row2) {
    if (!equal.load()) {
      return;
    }
    equal.store(row1 == row2);
  };

  std::vector<std::thread> threads;
  for (size_t i = 0; i < shape().first; ++i) {
    threads.emplace_back(
        [&, i]() { compare_vectors(matrix_[i], other.matrix_[i]); });
    for (auto& thr : threads) {
      thr.join();
      if (equal.load() == false) {
        return false;
      }
    }
    return true;
  }
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T>& other) const {
  return !(*this == other);
}
