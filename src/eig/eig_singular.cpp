#include "../../include/eig_singular.h"
#include "../../include/equation.h"
#include "../../include/det.h"
#include "../../include/dot.h"
#include "../../include/transpose.h"
#include <cmath>

template<typename T>
std::pair<Matrix<T>, Matrix<T>> FindQRSingular(const Matrix<T>& a) {
  std::shared_lock sh_lock(a.shared_mtx_);

  auto [rows, columns] = a.shape();
  if (rows != columns) {
    throw std::invalid_argument("Matrix should be square\n");
  }
  auto mat_tr = Transpose(a);

  // Transposed orthogonal matrix (vectors are stored in rows)
  std::vector<std::vector<T>> Q_tr;
  Q_tr.reserve(columns);
  Q_tr.emplace_back(mat_tr[0]);


  // Vector with scalar products of (Q_tr[i], Q_tr[i]),
  // because these values are used several times
  std::vector<T> scalar_products;
  scalar_products.reserve(rows);

  auto scalar_product = [](const std::vector<T>& v1, const std::vector<T>& v2) {
    T result = T();
    for (size_t i = 0; i < v1.size(); ++i) {
      result += v1[i] * v2[i];
    }
    return result;
  };

  // A function to orthogonalize an i-th vector of "a" via Gram-Schmidt ortogonalization
  // using precomputed vectors of Q_tr
  auto GrSchmidt = [&](size_t i) {
    auto result = mat_tr[i];
    scalar_products.emplace_back(scalar_product(Q_tr.back(), Q_tr.back()));
    for (size_t j = 0; j < i; ++j) {
      auto prod = scalar_product(mat_tr[i], Q_tr[j]);
      if (prod == T()) {
        continue;
      }
      result = Equation::compose_rows(
          result,
          Equation::multiply_by_scalar(
              Q_tr[j],
              prod / scalar_products[j]
          ),
          -T(1));
    }
    return result;
  };

  for (size_t i = 1; i < columns; ++i) {
    Q_tr.emplace_back(GrSchmidt(i));
  }

  for (auto &col : Q_tr) {
    col = Equation::multiply_by_scalar(col, T(1) / std::sqrt(scalar_product(col, col)));
  }

  auto Q_tr_mat = Matrix<T>(std::move(Q_tr));
  return std::make_pair(Transpose(Q_tr_mat), Dot(Q_tr_mat, a));
}

template<typename T>
bool IsUpTriangularSingular(const Matrix<T>& a, const T& delta) {
  std::shared_lock sh_lock(a.shared_mtx_);

  auto abs = [](const T& t) { return t > T() ? t : -t; };
  std::vector<bool> is_up_tr(a.shape().first, true);

  for (size_t i = 0; i < a.shape().first; ++i) {
    for (size_t j = 0; j < i; ++j) {
      if (abs(a[i][j]) > delta) {
        return false;
      }
    }
  }
  return true;
}

template<typename T>
std::vector<std::vector<T>> SolveMultipleSolutionsSingular(Matrix<T>&& a, const T& delta) {
  auto [rows_a, columns_a] = a.shape();

  if (rows_a != columns_a) {
    throw std::invalid_argument("Matrix \"a\" should be square\n");
  }
  auto is_zero = [&delta](const T& t) { return t > T() ? t < delta : -t < delta; };

  // Slightly modified algorithm from "Solution" function
  size_t cur_row = 0, cur_column = 0;

  std::vector<size_t> non_zero_positions;
  while (cur_row < rows_a) {
    size_t non_zero_row = rows_a, non_zero_column = columns_a;
    for (size_t j = cur_column; j < columns_a; ++j) {
      for (size_t i = cur_row; i < rows_a; ++i) {
        if (!is_zero(a[i][j])) {
          non_zero_row = i;
          non_zero_column = j;
          break;
        }
      }
      if (non_zero_row < rows_a) {
        break;
      }
    }

    if (non_zero_row == rows_a) {
      break;
    }

    Equation::swap_rows(a[non_zero_row], a[cur_row]);
    non_zero_positions.push_back(non_zero_column);

    for (size_t i = cur_row + 1; i < rows_a; ++i) {
      if (is_zero(a[i][non_zero_column])) {
        continue;
      }
      auto coef = -a[i][non_zero_column] / a[cur_row][non_zero_column];
      a[i] = Equation::compose_rows(a[i], a[cur_row], coef);
    }
    cur_row = non_zero_row + 1;
    cur_column = non_zero_column + 1;
  }

  for (size_t index = non_zero_positions.size() - (!non_zero_positions.empty()); index > 0; --index) {
    size_t column = non_zero_positions[index];
    for (size_t i = 0; i < index; ++i) {
      if (is_zero(a[index][column])) {
        continue;
      }
      auto coef = -a[i][index] / a[index][column];
      a[i] = Equation::compose_rows(a[i], a[index], coef);
    }
  }

  for (size_t index = 0; index < non_zero_positions.size(); ++index) {
    a[index] = Equation::multiply_by_scalar(a[index], T(1) / a[index][non_zero_positions[index]]);
  }

  std::vector<std::vector<T>> result(columns_a, std::vector<T>(columns_a - non_zero_positions.size()));

  std::vector<size_t> dependent_positions;
  auto next_non_zero_pos = non_zero_positions.begin();
  for (size_t i = 0; i < rows_a; ++i) {
    if (next_non_zero_pos == non_zero_positions.end() || i != *next_non_zero_pos) {
      dependent_positions.push_back(i);
    } else {
      ++next_non_zero_pos;
    }
  }

  for (size_t i = 0; i < non_zero_positions.size(); ++i) {
    auto row = non_zero_positions[i];
    for (size_t j = 0; j < dependent_positions.size(); ++j) {
      auto column = dependent_positions[j];
      result[row][j] = -a[i][column];
    }
  }
  for (size_t i = 0; i < dependent_positions.size(); ++i) {
    auto row = dependent_positions[i];
    for (size_t j = 0; j < dependent_positions.size(); ++j) {
      result[row][i] = T();
    }
    result[row][i] = T(1);
  }

  return result;
}

template<typename T>
std::pair<std::vector<T>, Matrix<T>> EigSingular(const Matrix<T>& a, const T& delta) {
  std::shared_lock sh_lock(a.shared_mtx_);

  if (Det(a) == T()) {
    throw std::invalid_argument("Singular matrix\n");
  }

  std::vector<T> eig_vals;
  auto [rows, columns] = a.shape();

  auto ComputeEigVec = [&, rows = rows](const T& eig_val) {
    auto tmp = a;
    for (size_t i = 0; i < rows; ++i) {
      tmp[i][i] -= eig_val;
    }
    return SolveMultipleSolutionsSingular(std::move(tmp));
  };

  Matrix<T> mat = a;
  while (!IsUpTriangularSingular(mat, delta)) {
    auto [Q, R] = FindQR(mat);
    mat = Dot(R, Q);
  }

  for (size_t i = 0; i < rows; ++i) {
    eig_vals.push_back(mat[i][i]);
  }
  auto result = ComputeEigVec(eig_vals[0]);
  for (size_t i = 1; i < eig_vals.size(); ++i) {
    auto tmp = ComputeEigVec(eig_vals[i]);
    for (size_t row = 0; row < result.size(); ++row) {
      result[row].reserve(result[i].size() + tmp[i].size());
      for (auto elem : tmp[row]) {
        result[row].push_back(elem);
      }
    }
  }
  return std::make_pair(eig_vals, Matrix<T>(std::move(result)));
}

