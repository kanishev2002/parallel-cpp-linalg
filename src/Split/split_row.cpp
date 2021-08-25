#include <include/split_row.h>
#include <include/split_row_single_thread.h>
#include <include/thread_pool.h>

#include <shared_mutex>
#include <thread>


template<typename T>
Matrix<T> Split_row(const Matrix<T>& mat, const int64_t from, const int64_t to, const int64_t step) {
  if ((step < 0 && from < to) || (step > 0 && from > to) || step == 0) {
    throw std::invalid_argument("Impossible arguments\n");
  }
  const auto[rows, columns] = mat.shape();
  if(columns<rows*6+13000){
    return Split_row_single_thread(mat, from, to, step);
  }
  std::vector<std::vector<T>> res(((std::abs(to - from) - 1) / std::abs(step) + 1));
  std::shared_lock sh_lock(mat.shared_mtx_);
  {
    ThreadPool pool;
    for (int64_t row = from, new_row = 0; (row < to && step > 0) || (row > to && step < 0); row += step, new_row++) {
      pool.enqueue_task([&, row, new_row]() {
        res[new_row] = mat[row];
      });
    }
  }
  return (Matrix<T>(std::move(res)));
}
