#include "../../include/matrix_power.h"
#include "../../include/dot.h"
#include <cmath>
#include <shared_mutex>

template<typename T>
Matrix<T> matrix_power(const Matrix<T>& a, size_t n) {
  std::shared_lock<Matrix<T>>(a.shared_mtx_);
  if (a.shape().first != a.shape().second) {
    throw std::invalid_argument("Matrix should be square");
  }
  auto result = Matrix<T>::eye(a.shape().first);
  auto tmp = a;
  while (n > 0) {
    if (n & 1) {
      result = dot(result, tmp);
      --n;
    } else {
      tmp = dot(tmp, tmp);
      n >>= 1;
    }
  }
  return result;
}
