#include "../../include/Dot.h"

#include <shared_mutex>
#include <stdexcept>

template <typename T>
Matrix<T> dot(const Matrix<T>& l, const Matrix<T>& r) {
  std::shared_lock l_sh_lock(l.shared_mtx_);
  std::shared_lock r_sh_lock(r.shared_mtx_);
  const auto [rows1, cols1] = l.shape();
  const auto [rows2, cols2] = r.shape();
  if (cols1 != rows2) {
    throw std::invalid_argument(
        "Matrixes do not have correct shapes for multiplication");
  }
  Matrix<T> result(rows1, cols2);
  auto multiply = [&](size_t row, size_t col) {
    // TODO
  };
}