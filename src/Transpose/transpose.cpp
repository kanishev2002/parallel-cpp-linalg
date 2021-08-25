#include <include/transpose.h>

#include <shared_mutex>
#include <vector>

template <typename T>
Matrix<T> Transpose(const Matrix<T>& matrix) {
  std::shared_lock sh_lock(matrix.shared_mtx_);
  const auto[mat_rows, mat_columns] = matrix.shape();
  size_t thread_limit = std::thread::hardware_concurrency();
  if(thread_limit == 0) {thread_limit = 8;}
  size_t rows = mat_rows;
  size_t columns = mat_columns;
  std::vector<std::thread> threads;
  threads.reserve(thread_limit);
  std::vector<std::vector<T>> res_data(columns);
  for (size_t thread_number = 0; thread_number < thread_limit; ++thread_number) {
    threads.emplace_back([&, thread_number] {
      for (size_t old_col = thread_number*columns/thread_limit; old_col < (thread_number+1)*columns/thread_limit; ++old_col) {
        std::vector<T> new_row(rows);
        for (size_t old_row = 0; old_row < rows; ++old_row) {
          new_row[old_row] = matrix[old_row][old_col];
        }
        res_data[old_col] = std::move(new_row);
      }
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  return Matrix<T>(std::move(res_data));
}