#include "../../include/det.h"

template<typename T>
T det(const Matrix<T>& a) {
  size_t rows, columns;

  std::shared_lock sh_lock(a.shared_mtx_);
  std::tie(rows, columns) = a.shape();
  if (rows != columns) {
    throw std::invalid_argument("Matrix should be square\n");
  }

  using Fraction = Fraction<T>;
  Matrix <Fraction> matrix(rows, columns);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      matrix[i][j] = a[i][j];
    }
  }

  size_t cur_row = 0, cur_column = 0;

  while (cur_row < rows) {
    size_t non_zero_row = rows, non_zero_column = columns;
    for (size_t j = cur_column; j < columns; ++j) {
      for (size_t i = cur_row; i < rows; ++i) {
        if (matrix[i][j] != Fraction(0)) {
          non_zero_row = i;
          non_zero_column = j;
          break;
        }
      }
      if (non_zero_row < rows) {
        break;
      }
    }

    if (non_zero_row == rows) {
      break;
    }

    Equation::swap_rows(matrix[non_zero_row], matrix[cur_row]);

    {
      ThreadPool pool;
      for (size_t i = cur_row + 1; i < rows; ++i) {
        if (matrix[i][non_zero_column] == Fraction(0)) {
          continue;
        }
        pool.enqueue_task([&, i]() {
            matrix[i] = Equation::compose_rows(
                    Equation::multiply_by_scalar(
                            matrix[cur_row],
                            -matrix[i][non_zero_column] / matrix[cur_row][non_zero_column]
                            ),
                            matrix[i]
                    );
                });
            }
        }
        cur_row = non_zero_row + 1;
        cur_column = non_zero_column + 1;
    }

  auto result = Fraction(1);
  for (size_t i = 0; i < rows; ++i) {
    result *= matrix[i][i];
  }

  return result.Numerator() / result.Denominator();
}

