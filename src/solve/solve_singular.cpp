#include <include/equation.h>
#include <include/solve_singular.h>

template <typename T>
Matrix<T> SolveSingular(const Matrix<T>& a, const Matrix<T>& b) {
    size_t rows_a, columns_a;
    size_t rows_b, columns_b;
    Matrix<T> A, B;

    {
        std::shared_lock sh_lock_a(a.shared_mtx_);
        std::shared_lock sh_lock_b(b.shared_mtx_);
        std::tie(rows_a, columns_a) = a.shape();
        std::tie(rows_b, columns_b) = b.shape();

        if (rows_a != columns_a) {
            throw std::invalid_argument("Matrix A should be square\n");
        }
        if (rows_a != rows_b) {
            throw std::invalid_argument("Matrixes should be conformable\n");
        }

        A = a;
        B = b;
    }

    size_t cur_row = 0, cur_column = 0;

    while (cur_row < rows_a) {
        size_t non_zero_row = rows_a, non_zero_column = columns_a;
        for (size_t j = cur_column; j < columns_a; ++j) {
            for (size_t i = cur_row; i < rows_a; ++i) {
                if (A[i][j] != T()) {
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

        for (size_t i = cur_row + 1; i < rows_a; ++i) {
            if (A[i][non_zero_column] == T()) {
                continue;
            }
            auto coef = -A[i][non_zero_column] / A[cur_row][non_zero_column];
            A[i] = Equation::compose_rows(A[i], A[cur_row], coef);
            B[i] = Equation::compose_rows(B[i], B[cur_row], coef);
        }

        cur_row = non_zero_row + 1;
        cur_column = non_zero_column + 1;
    }

    for (size_t index = rows_a - 1; index > 0; --index) {
        if (A[index][index] == T()) {
            throw std::invalid_argument("Singular matrix\n");
        }

        for (size_t i = 0; i < index; ++i) {
            if (A[i][index] == T()) {
                continue;
            }
            auto coef = -A[i][index] / A[index][index];
            A[i] = Equation::compose_rows(A[i], A[index], coef);
            B[i] = Equation::compose_rows(B[i], B[index], coef);
        }
    }

    for (size_t index = 0; index < rows_a; ++index) {
        B[index] = Equation::multiply_by_scalar(B[index], T(1) / A[index][index]);
    }
    return B;
}
