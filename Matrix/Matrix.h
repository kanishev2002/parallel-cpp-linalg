#pragma once
#include <vector>

template <typename T>
class Matrix {
 private:
  std::vector<std::vector<T>> matrix_;

 public:
  Matrix() = default;
  Matrix(size_t rows, size_t columns);
  Matrix(std::vector<std::vector<T>> matrix);
  Matrix(const Matrix<T>& other);
  Matrix(Matrix<T>&& other);

  bool operator==(const Matrix<T>& other) const;
  bool operator!=(const Matrix<T>& other) const;

  void print(std::ostream& out, char column_spliter = '\t',
             char row_spliter = '\n') const;

  Matrix<T> operator+(const Matrix<T>& other) const;
  Matrix<T> operator-(const Matrix<T>& other) const;
  Matrix<T> operator*(const Matrix<T>& other) const;

  Matrix<T> operator[](size_t index);

  std::pair<size_t, size_t> shape() const;
};