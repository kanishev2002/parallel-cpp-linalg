#include "../../include/transpose_single_thread.h"

#include <vector>

template <typename T>
Matrix<T> Transpose_single_thread(const Matrix<T>& matrix) {
  std::shared_lock sh_lock(matrix.shared_mtx_);
  const auto [rows, columns] = matrix.shape();
  std::vector<std::vector<T>> res_data(columns, std::vector<T>(rows));
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      res_data[j][i] = matrix[i][j];
    }
  }
  return Matrix<T>(std::move(res_data));
}
