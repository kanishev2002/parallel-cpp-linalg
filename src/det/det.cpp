#include "../../include/det.h"

#include "../../include/equation.h"
#include "../../include/thread_pool.h"

template <typename T>
T det(const Matrix<T>& a) {
  std::shared_lock sh_lock(a.shared_mtx_);
  const auto [rows, columns] = a.shape();
  if (rows != columns) {
    throw std::invalid_argument("Matrix should be square\n");
  }

  Matrix<T> matrix = a;

  size_t cur_row = 0, cur_column = 0;

  while (cur_row < rows) {
    size_t non_zero_row = rows, non_zero_column = columns;
    for (size_t j = cur_column; j < columns; ++j) {
      for (size_t i = cur_row; i < rows; ++i) {
        if (matrix[i][j] != T(0)) {
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
        if (matrix[i][non_zero_column] == T(0)) {
          continue;
        }
        pool.enqueue_task([&, i, cur_row, cur_column]() {
          auto coef =
              -matrix[i][non_zero_column] / matrix[cur_row][non_zero_column];
          matrix[i] = Equation::compose_rows(
              Equation::multiply_by_scalar(matrix[cur_row], coef), matrix[i]);
        });
      }
    }
    cur_row = non_zero_row + 1;
    cur_column = non_zero_column + 1;
  }

  auto result = T(1);
  for (size_t i = 0; i < rows; ++i) {
    result *= matrix[i][i];
  }

  return result;
}
