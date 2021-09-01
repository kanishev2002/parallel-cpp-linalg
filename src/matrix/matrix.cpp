#include <include/matrix.h>
#include <include/thread_pool.h>

#include <atomic>
#include <iostream>
#include <thread>


constexpr Matrix<T>::Matrix(size_t rows, size_t columns) {
  matrix_.resize(rows, std::vector<T>(columns));
}

template <typename T>
Matrix<T>::Matrix(const std::vector<T>& vec) {
  matrix_.resize(1, vec);
}

template <typename T>
Matrix<T>::Matrix(std::vector<T>&& vec) {
  matrix_.resize(1, std::move(vec));
}

template <typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& matrix) {
  matrix_ = matrix;
}

template <typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>>&& matrix) {
  matrix_ = std::move(matrix);
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other) {
  std::unique_lock this_un_lock(shared_mtx_);
  std::shared_lock other_sh_lock(other.shared_mtx_);
  matrix_ = other.matrix_;
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& other) noexcept {
  std::unique_lock this_un_lock(shared_mtx_);
  std::unique_lock other_un_lock(other.shared_mtx_);
  matrix_ = std::move(other.matrix_);

}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
  std::unique_lock this_un_lock(shared_mtx_);
  std::shared_lock other_sh_lock(other.shared_mtx_);

  if (this == &other) {
    return *this;
  }

  matrix_ = other.matrix_;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept {
  std::unique_lock this_un_lock(shared_mtx_);
  std::unique_lock other_un_lock(other.shared_mtx_);

  if (this == &other) {
    return *this;
  }

  matrix_ = std::move(other.matrix_);
  
  return *this;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) const {
  std::shared_lock this_sh_lock(shared_mtx_);
  std::shared_lock other_sh_lock(other.shared_mtx_);

  if (shape() != other.shape()) {
    return false;
  }

  std::atomic<bool> equal = true;
  auto compare_vectors = [&](const auto& row1, const auto& row2) {
    if (row1 != row2) {
      equal.store(false);
    }
    return;
  };

  {
    ThreadPool pool;
    for (size_t i = 0; i < shape().first; ++i) {
      pool.enqueue_task(
          [&, i]() { compare_vectors(matrix_[i], other.matrix_[i]); });
    }
  }
  return equal.load();
}

template <typename T>
const std::vector<T>& Matrix<T>::operator[](size_t index) const {
  std::shared_lock sh_lock(shared_mtx_);
  return matrix_.at(index);
}

template <typename T>
std::vector<T>& Matrix<T>::operator[](size_t index) {
  std::unique_lock un_lock(shared_mtx_);
  return matrix_[index];
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T>& other) const {
  std::shared_lock this_sh_lock(shared_mtx_);
  std::shared_lock other_sh_lock(other.shared_mtx_);
  return !(*this == other);
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
  std::shared_lock this_sh_lock(shared_mtx_);
  std::shared_lock other_sh_lock(other.shared_mtx_);
  return basic_binary_op_(other, '+');
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const {
  std::shared_lock this_sh_lock(shared_mtx_);
  std::shared_lock other_sh_lock(other.shared_mtx_);
  return basic_binary_op_(other, '-');
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
  std::shared_lock this_sh_lock(shared_mtx_);
  std::shared_lock other_sh_lock(other.shared_mtx_);
  return basic_binary_op_(other, '*');
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T& other_const) const {
  std::shared_lock sh_lock(shared_mtx_);
  auto [rows, columns] = shape();

  Matrix<T> result(rows, 0);
  {
    ThreadPool pool;
    for (size_t i = 0; i < shape().first; ++i) {
      pool.enqueue_task([&, i]() {
        std::vector<T> new_row(matrix_[i]);
        for (size_t j = 0; j < shape().second; ++j) {
          new_row[j] *= other_const;
        }
        result.matrix_[i] = std::move(new_row);
      });
    }
  }
  
  return result;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other) {
  std::unique_lock this_un_lock(shared_mtx_);
  std::shared_lock other_sh_lock(other.shared_mtx_);
  matrix_ = std::move(basic_binary_op_(other, '+').matrix_);
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& other) {
  std::unique_lock this_un_lock(shared_mtx_);
  std::shared_lock other_sh_lock(other.shared_mtx_);
  matrix_ = std::move(basic_binary_op_(other, '-').matrix_);
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& other) {
  std::unique_lock this_un_lock(shared_mtx_);
  std::shared_lock other_sh_lock(other.shared_mtx_);
  matrix_ = std::move(basic_binary_op_(other, '*').matrix_);
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const T& other_const) {
  std::unique_lock un_lock(shared_mtx_);
  auto [rows, columns] = shape();

  std::vector<std::vector<T>> result(rows);
  {
    ThreadPool pool;
    for (size_t i = 0; i < shape().first; ++i) {
      pool.enqueue_task([&, i]() {
        std::vector<T> new_row(matrix_[i]);
        for (size_t j = 0; j < shape().second; ++j) {
          new_row[j] *= other_const;
        }
        result[i] = std::move(new_row);
      });
    }
  }
  matrix_ = std::move(result);
  return *this;
}

template <typename T>
std::pair<size_t, size_t> Matrix<T>::shape() const {
  std::shared_lock sh_lock(shared_mtx_);
  if (matrix_.empty()) {
    return {0, 0};
  }
  return {matrix_.size(), matrix_.front().size()};
}

template <typename T>
Matrix<T> Matrix<T>::basic_binary_op_(const Matrix<T>& other,
                                      char func_type) const {
  if (shape() != other.shape()) {
    throw std::invalid_argument("Matrixes have different shapes\n");
  }
  auto func = [&](const T& a, const T& b) {
    switch (func_type) {
      case '*':
        return a * b;
      case '+':
        return a + b;
      case '-':
        return a - b;
      default:
        throw std::invalid_argument("Unknown operation\n");
    }
  };
  auto [rows, columns] = shape();

  Matrix<T> result(rows, 0);
  {
    ThreadPool pool;
    for (size_t i = 0; i < shape().first; ++i) {
      pool.enqueue_task([&, i]() {
        std::vector<T> new_row(matrix_[i]);
        for (size_t j = 0; j < shape().second; ++j) {
          new_row[j] = func(new_row[j], other.matrix_[i][j]);
        }
        result.matrix_[i] = std::move(new_row);
      });
    }
  }
  return result;
}

template <typename T>
void Matrix<T>::print(std::ostream& out, char column_separator,
                      char row_separator) const {
  std::shared_lock sh_lock(shared_mtx_);
  const auto [rows, columns] = shape();
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      out << matrix_[i][j] << column_separator;
    }
    out << row_separator;
  }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& mat) {
  mat.print(out, '\t', '\n');
  return out;
}
