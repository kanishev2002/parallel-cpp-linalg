#include "../../include/thread_pool.h"

ThreadPool::ThreadPool(size_t threads) {
  if (threads == 0) {
    throw std::invalid_argument("Number of threads equals zero\n");
  }
  threads_.reserve(threads);
  for (size_t i = 0; i < threads; ++i) {
    threads_.emplace_back([this]() {
      while (true) {
        func_ptr work{nullptr};

        {
          std::unique_lock un_lock(mtx_);
          cv_.wait(un_lock, [&]() { return !waiting_queue_.empty(); });
          work = std::move(waiting_queue_.front());
          waiting_queue_.pop();
        }

        if (work == nullptr) {
          break;
        } else {
          (*work)();
        }
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock un_lock(mtx_);
    for (auto& thr : threads_) {
      waiting_queue_.push(func_ptr{nullptr});
    }
    cv_.notify_all();
  }
  for (auto& thr : threads_) {
    thr.join();
  }
}

void ThreadPool::enqueue_task(func f) {
  auto task_ptr = std::make_unique<func>(std::move(f));
  {
    std::unique_lock un_lock(mtx_);
    waiting_queue_.push(std::move(task_ptr));
  }
  cv_.notify_all();
}
