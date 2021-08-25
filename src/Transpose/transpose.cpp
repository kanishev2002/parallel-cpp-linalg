#include <include/thread_pool.h>
#include <include/transpose.h>

#include <shared_mutex>
#include <src/thread_pool/thread_pool.cpp>
#include <vector>

template <typename T>
Matrix<T> Transpose(const Matrix<T>& matrix) {
  std::shared_lock sh_lock(matrix.shared_mtx_);
  const auto [mat_rows, mat_columns] = matrix.shape();
  size_t rows = mat_rows;
  size_t columns = mat_columns;
  std::vector<std::thread> threads;
  std::vector<std::vector<T>> res_data(columns);
  {
    ThreadPool pool;
    for (size_t old_col = 0; old_col < columns; ++old_col) {
      pool.enqueue_task([&, old_col, rows] {
        std::vector<T> new_row(rows);
        for (size_t old_row = 0; old_row < rows; ++old_row) {
          new_row[old_row] = matrix[old_row][old_col];
        }
        res_data[old_col] = std::move(new_row);
      });
    }
  }
  return Matrix<T>(std::move(res_data));
}
