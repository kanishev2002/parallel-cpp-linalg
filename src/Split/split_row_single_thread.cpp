#include "../../include/split_row_single_thread.h"

#include <shared_mutex>
#include <thread>

template<typename T>
Matrix<T> Split_row_single_thread(const Matrix<T>& mat, const int64_t from, const int64_t to, const int64_t step) {
  if ((step < 0 && from < to) || (step > 0 && from > to) || step == 0) {
    throw std::invalid_argument("Impossible arguments\n");
  }
  std::shared_lock sh_lock(mat.shared_mtx_);
  std::vector<std::vector<T>> res(((std::abs(to - from) - 1) / std::abs(step) + 1), std::vector<T>(mat.shape().second));
  //std::cout<<"\n"<<"     "<<((std::abs(to - from) - 1) / std::abs(step) + 1)<<"\n";
  for (int64_t row = from, new_row = 0; (row < to && step>0) || (row > to && step<0); row += step, new_row++) {
    //std::cout<<new_row<<"   "<<row<<"\n";
    res[new_row] = mat[row];
  }
  return (Matrix<T>(std::move(res)));
}
