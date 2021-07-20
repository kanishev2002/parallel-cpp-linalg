#include "../../include/matrix_power.h"
#include <cmath>

template<typename T>
Matrix<T> matrix_power(const Matrix<T>& a, size_t n) {
  if (a.shape().first != a.shape().second) {
    throw std::invalid_argument("Matrix should be square");
  }
  if (n == 0) {
    return Matrix<T>::eye(a.shape().first);
  }
  auto result = a;
  auto split_into_sum_of_pows = [&](size_t n_) {
    std::vector<size_t> powers;
    size_t i = 1;
    while (i <= n_) {
      if (n_ & i) {
        powers.push_back(i);
      }
      i = i << 1;
    }
    return powers;
  };
  auto powers = split_into_sum_of_pows(n);
  auto tmp = a;
  size_t tmp_pow = 1;
  for (auto it = powers.begin(); it != powers.end(); ++it) {
    while (tmp_pow != *it) {
      tmp = dot(tmp, tmp);
      tmp_pow *= 2;
    }
    result = dot(result, tmp);
  }
  return result;
}
