#include "../../include/split_column_single_thread.h"

#include <shared_mutex>
#include <thread>

template<typename T>
Matrix<T> Split_column_single_thread(const Matrix<T>& mat, const int64_t from, const int64_t to, const int64_t step) {
  if ((step < 0 && from < to) || (step > 0 && from > to) || step == 0) {
    throw std::invalid_argument("Impossible arguments\n");
  }
  std::vector<std::vector<T>> res(mat.shape().first, std::vector<T>((std::abs(to - from) - 1) / std::abs(step) + 1));
  std::shared_lock sh_lock(mat.shared_mtx_);
  for (int64_t column = from, new_column = 0; (column < to && step>0) || (column > to && step<0); column += step, new_column++) {
    for(size_t row = 0; row < mat.shape().first; ++row){
        res[row][new_column] = mat[row][column];
    }
  }
  return (Matrix<T>(std::move(res)));
}
