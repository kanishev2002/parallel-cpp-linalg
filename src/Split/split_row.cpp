#include "../../include/split_row.h"

#include <shared_mutex>
#include <thread>


template<typename T>
Matrix<T> Split_row(const Matrix<T>& mat, const int64_t from, const int64_t to, const int64_t step) {
  if ((step < 0 && from < to) || (step > 0 && from > to) || step == 0) {
    throw std::invalid_argument("Impossible arguments\n");
  }

  std::vector<std::thread> threads;
  std::vector<std::vector<T>> res(((std::abs(to - from) - 1) / std::abs(step) + 1));
  std::shared_lock sh_lock(mat.shared_mtx_);
  for (int64_t row = from, new_row = 0; (row < to && step>0) || (row > to && step<0); row += step, new_row++) {
    threads.emplace_back([&, row, new_row]{
      res[new_row] = mat[row];
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  return (Matrix<T>(std::move(res)));
}
