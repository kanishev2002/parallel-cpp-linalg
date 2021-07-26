#include "../../include/equation.h"
#include "../../include/solve.h"
#include "../../include/thread_pool.h"

template<typename T>
Matrix<T> solve(const Matrix<T>& a, const Matrix<T>& b) {
  std::shared_lock sh_lock_a(a.shared_mtx_);
  std::shared_lock sh_lock_b(b.shared_mtx_);
  auto [rows_a, columns_a] = a.shape();
  auto [rows_b, columns_b] = b.shape();

  if (rows_a != columns_a) {
    throw std::invalid_argument("Matrix A should be square\n");
  }
  if (rows_a != rows_b) {
    throw std::invalid_argument("Matrixes should be conformable\n");
  }

  auto A = a;
  auto B = b;

  size_t cur_row = 0, cur_column = 0;

  while (cur_row < rows_a) {
    size_t non_zero_row = rows_a, non_zero_column = columns_a;
    for (size_t j = cur_column; j < columns_a; ++j) {
      for (size_t i = cur_row; i < rows_a; ++i) {
        if (A[i][j] != T(0)) {
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

    Equation::swap_rows(A[non_zero_row], A[cur_row]);
    Equation::swap_rows(B[non_zero_row], B[cur_row]);

    {
      ThreadPool pool;
      for (size_t i = cur_row + 1; i < rows_a; ++i) {
        if (A[i][non_zero_column] == T(0)) {
          continue;
        }
        pool.enqueue_task([&, i]() {
          auto coef = -A[i][non_zero_column] / A[cur_row][non_zero_column];
          A[i] = Equation::compose_rows(
              Equation::multiply_by_scalar(
                  A[cur_row],
                  coef
              ),
              A[i]
          );

          B[i] = Equation::compose_rows(
              Equation::multiply_by_scalar(
                  B[cur_row],
                  coef
              ),
              B[i]
          );
        });
      }
    }
    cur_row = non_zero_row + 1;
    cur_column = non_zero_column + 1;
  }

  /* Now matrix A looks like:
   *
   * |  #   *   *   *   ...     *   |
   * |  0   #   *   *   ...     *   |
   * |  0   0   #   *   ...     *   |
   * |            ...               |
   * |  0   0   0   0   ...     #   |
   *
   * In case the system has only one solution, otherwise
   * '#' is a nonzero element, '0' is a zero element and '*' could be both zero and nonzero.
   */

  for (size_t index = rows_a - 1; index > 0; --index) {
    if (A[index][index] == T(0)) {
      throw std::invalid_argument("Singular matrix\n");
    }
    ThreadPool pool;
    for (size_t i = 0; i < index; ++i) {
      if (A[i][index] == T(0)) {
        continue;
      }
      pool.enqueue_task([&, i]() {
        auto coef = -A[i][index] / A[index][index];
        A[i] = Equation::compose_rows(
            Equation::multiply_by_scalar(
                A[index],
                coef
            ),
            A[i]
        );

        B[i] = Equation::compose_rows(
            Equation::multiply_by_scalar(
                B[index],
                coef
            ),
            B[i]
        );
      });
    }
  }

  /* Now matrix A looks like:
   *
   * |  #   0   0   0   ...     0   |
   * |  0   #   0   0   ...     0   |
   * |  0   0   #   0   ...     0   |
   * |            ...               |
   * |  0   0   0   0   ...     #   |
   *
   * The answer can be extracted from B in case A looks like:
   *
   * |  1   0   0   0   ...     0   |
   * |  0   1   0   0   ...     0   |
   * |  0   0   1   0   ...     0   |
   * |            ...               |
   * |  0   0   0   0   ...     1   |
   *
   * There is no need to reduce elements of A though, we can just divide B[i] by A[i][i]
   * (the only nonzero element in i-th row)
   */

  ThreadPool pool;
  for (size_t index = 0; index < rows_a; ++index) {
    pool.enqueue_task([&, index]() {
      B[index] = Equation::multiply_by_scalar(B[index], T(1) / A[index][index]);
    });
  }
  return B;
}
