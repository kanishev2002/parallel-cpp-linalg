#include "Matrix.h"

#include <atomic>
#include <thread>

template<typename T>
Matrix<T>::Matrix(size_t rows, size_t columns) {
    matrix_.resize(rows, std::vector<T>(columns));
}

template<typename T>
Matrix<T>::Matrix(const std::vector<T>& vec) {
    matrix_.resize(1, vec);
}

template<typename T>
Matrix<T>::Matrix(std::vector<T>&& vec) {
    matrix_.resize(1, std::move(vec));
}

template<typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& matrix) {
    matrix_ = matrix;
}

template<typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>>&& matrix) {
    matrix_ = std::move(matrix);
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& other) {
    matrix_ = other.matrix_;
}

template<typename T>
Matrix<T>::Matrix(Matrix<T>&& other) {
    matrix_ = other.matrix_;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &other) const {
    if (shape() != other.shape()) {
        return false;
    }

    std::atomic<bool> equal = true;
    auto compare_vectors = [&](const auto& row1, const auto& row2) {
        if (!equal.load()) {
            return;
        }
        equal.store(row1 == row2);
    };

    std::vector<std::thread> threads;
    for (size_t i = 0; i < shape().first; ++i) {
        threads.emplace_back(
                [&, i]() { compare_vectors(matrix_[i], other.matrix_[i]); });
    }

    for (auto& thr : threads) {
        thr.join();
        if (equal.load() == false) {
            return false;
        }
    }
    return equal.load();
}

template<typename T>
Matrix<T>& Matrix<T>::operator[](size_t index) {
    return matrix_[index];
}

template<typename T>
const Matrix<T>& Matrix<T>::operator[](size_t index) const {
    return matrix_.at(index);
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T>& other) const {
    return !(*this == other);
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const {
    return basic_binary_op_(other, '+');
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &other) const {
    return basic_binary_op_(other, '-');
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &other) const {
    return basic_binary_op_(other, '*');
}


template<typename T>
std::pair<size_t, size_t> Matrix<T>::shape() const {
    if (matrix_.empty()) {
        return {0, 0};
    }
    return {matrix_.size(), matrix_.front().size()};
}

template<typename T>
void Matrix<T>::print(std::ostream &out, char column_splitter, char row_splitter) const {
    for (size_t i = 0; i < matrix_.size(); ++i) {
        for (size_t j = 0; j < matrix_[0].size(); ++j) {
            out << matrix_[i][j] << column_splitter;
        }
        out << row_splitter;
    }
}

template<typename T>
Matrix<T> Matrix<T>::basic_binary_op_(const Matrix<T> &other, char func_type) const {
    if (shape() != other.shape()) {
        throw std::invalid_argument("Matrixes have different shapes\n");
    }
    auto func = [&](const T& a, const T& b) {
        switch (func_type) {
            case '*': return a * b;
            case '+': return a + b;
            case '-': return a - b;
            default: throw std::invalid_argument("Unknown operation\n");
        }
    };
    auto [rows, columns] = shape();
    Matrix<T> result(rows, columns);
    std::vector<std::thread> threads;
    for (size_t i = 0; i < shape().first; ++i) {
        threads.emplace_back([&, i]() {
            for (size_t j = 0; j < shape().second; ++j) {
                result.matrix_[i][j] = func(matrix_[i][j], other.matrix_[i][j]);
            }
        });
    }
    for (auto& thr : threads) {
        thr.join();
    }
    return result;
}
