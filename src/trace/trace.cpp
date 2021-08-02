#include "../../include/trace.h"
#include <thread>
#include <shared_mutex>

T Trace(const Matrix<T>& a) {
  std::shared_lock sh_lock(a.shared_mtx_);
  auto [rows, columns] = a.shape();
  if (rows != columns) {
    throw std::invalid_argument("Matrix should be square\n");
  }

  size_t n_threads = std::thread::hardware_concurrency();
  size_t range_size = std::max(1ul, rows / n_threads);

  std::vector<std::thread> threads;
  std::vector<T> cur_results(n_threads);
  size_t begin = 0;
  for (size_t i = 0; i < n_threads; ++i) {
    size_t end = i + 1 == n_threads ? rows : begin + range_size;
    threads.emplace_back([&, i, rows = rows](auto begin, auto end) {
      T cur_result = T(0);
      for (size_t index = begin; index < end; ++index) {
        cur_result += a[index][index];
      }
      cur_results[i] = cur_result;
    }, begin, end);
    begin += range_size;
  }

  T result = T(0);
  for (size_t i = 0; i < n_threads; ++i) {
    threads[i].join();
    result += cur_results[i];
  }
  return result;
}
