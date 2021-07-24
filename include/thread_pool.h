#pragma once

#include <functional>
#include <thread>
#include <memory>
#include <vector>

class ThreadPool {
 private:
  using func = std::function<void(void)>;
  using func_ptr = std::unique_ptr<func>;

  std::vector<std::thread> threads_;
  std::queue<func_ptr> waiting_queue_;

  std::mutex mtx_;
  std::condition_variable cv_;

 public:
  ThreadPool(size_t threads = std::thread::hardware_concurrency());

  ~ThreadPool();

  void enqueue_task(func f);
};
