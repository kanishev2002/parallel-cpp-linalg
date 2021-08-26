#include <include/dot.h>

#include <shared_mutex>
#include <stdexcept>
#include <thread>
#include <vector>

// Finds the result of l and r dot product at given row and column
template <typename T>
static inline T CountAt(const Matrix<T>& l, const Matrix<T>& r, size_t row,
                        size_t col);

template <typename T>
Matrix<T> Dot(const Matrix<T>& l, const Matrix<T>& r) {
  std::shared_lock l_sh_lock(l.shared_mtx_);
  std::shared_lock r_sh_lock(r.shared_mtx_);
  const auto [rows1, cols1] = l.shape();
  const auto [rows2, cols2] = r.shape();
  if (cols1 != rows2) {
    throw std::invalid_argument(
        "Matrixes do not have correct shapes for multiplication");
  }
  size_t res_rows = rows1;
  size_t res_cols = cols2;
  Matrix<T> result(res_rows, 0);

  std::vector<std::thread> threads;

  for (size_t row = 0; row < res_rows; ++row) {
    threads.emplace_back([&, row] {
      std::vector<T> new_row(res_cols);
      for (size_t col = 0; col < res_cols; ++col) {
        new_row[col] = CountAt(l, r, row, col);
      }
      result[row] = std::move(new_row);
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }
  return result;
}

template <typename T>
static inline T CountAt(const Matrix<T>& l, const Matrix<T>& r, size_t row,
                        size_t col) {
  T product_sum = T{};
  for (size_t i = 0; i < l.shape().second; ++i) {
    product_sum += l[row][i] * r[i][col];
  }
  return product_sum;
}
