#pragma once

#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>

template <typename T>
class Matrix {
 private:
  std::vector<std::vector<T>> matrix_;

  Matrix<T> basic_binary_op_(const Matrix<T>& other, char func_type) const;

 public:
  Matrix() = default;
  Matrix(size_t rows, size_t columns);
  Matrix(const std::vector<T>& vec);
  Matrix(std::vector<T>&& vec);
  Matrix(const std::vector<std::vector<T>>& matrix);
  Matrix(std::vector<std::vector<T>>&& matrix);
  Matrix(const Matrix<T>& other);
  Matrix(Matrix<T>&& other) noexcept;

  Matrix<T>& operator=(const Matrix<T>& other);
  Matrix<T>& operator=(Matrix<T>&& other) noexcept;

  bool operator==(const Matrix<T>& other) const;
  bool operator!=(const Matrix<T>& other) const;

  void print(std::ostream& out, char column_separator = '\t',
             char row_separator = '\n') const;

  Matrix<T> operator+(const Matrix<T>& other) const;
  Matrix<T> operator-(const Matrix<T>& other) const;
  Matrix<T> operator*(const Matrix<T>& other) const;
  Matrix<T> operator*(const T& other) const;

  Matrix<T>& operator+=(const Matrix<T>& other);
  Matrix<T>& operator-=(const Matrix<T>& other);
  Matrix<T>& operator*=(const Matrix<T>& other);
  Matrix<T>& operator*=(const T& other);

  const std::vector<T>& operator[](size_t index) const;
  std::vector<T>& operator[](size_t index);

  std::pair<size_t, size_t> shape() const;

  mutable std::shared_mutex shared_mtx_;
};
