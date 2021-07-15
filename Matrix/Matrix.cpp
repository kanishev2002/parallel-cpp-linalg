#include "Matrix.h"

#include <atomic>
#include <thread>

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &other) const {
    if (shape() != other.shape()) {
        return false;
    }

    std::atomic<bool> equal = true;
    auto compare_vectors = [&](const auto& row1, const auto& row2) {
        for (size_t i = 0; i < row1.size(); ++i) {
            if (!equal.exchange(row1[i] == row2[i])) {
                return false;
            }
        }
        return true;
    };

    std::vector<std::thread> threads;
    for (size_t i = 0; i < shape().first; ++i) {
        threads.emplace_back([&]() { equal.store(equal.load() && compare_vectors); }, matrix_[i], other.matrix_[i]);
    }
    for (auto& thr : threads) {
        thr.join();
    }
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T> &other) const {
    return !(*this == other);
}
