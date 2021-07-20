#include "../../include/dot.h"

#include <shared_mutex>
#include <vector>

template <typename T>
Matrix<T> transpose(const Matrix<T>& matrix) const {
  std::shared_lock<T>(matrix.shared_mtx_);
  const auto[rows, columns] = matrix.shape();
  std::vector<std::vector<T>> res_data(columns, std::vector<T>(rows));
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++i) {
      res_data[j][i] = matrix[i][j];
    }
  }

  return Matrix<T>(res_data);
}

