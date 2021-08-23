#include "../../include/split_column.h"

#include <shared_mutex>
#include <thread>

template<typename T>
Matrix<T> Split_column(const Matrix<T>& mat, const int64_t from, const int64_t to, const int64_t step) {
  if ((step < 0 && from < to) || (step > 0 && from > to) || step == 0) {
    throw std::invalid_argument("Impossible arguments\n");
  }
  std::vector<std::thread> threads;
  std::vector<std::vector<T>> res(mat.shape().first);
  std::shared_lock sh_lock(mat.shared_mtx_);
  for(size_t row = 0; row < mat.shape().first; ++row) {
    threads.emplace_back([&, row] {
      std::vector<T> tmp_res((std::abs(to - from) - 1) / std::abs(step) + 1);
      for (int64_t column = from, new_column = 0; (column < to && step>0) || (column > to && step<0); column += step, new_column++) {
        tmp_res[new_column] = mat[row][column];
      }
      res[row] = std::move(tmp_res);
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }
  return (Matrix<T>(std::move(res)));
}
