#include "../../include/dot_single_thread.h"

#include <shared_mutex>
#include <stdexcept>
#include <thread>
#include <vector>

template <typename T>
Matrix<T> Dot_single_thread(const Matrix<T>& l, const Matrix<T>& r) {
    std::shared_lock l_sh_lock(l.shared_mtx_);
    std::shared_lock r_sh_lock(r.shared_mtx_);
    const auto [rows1, cols1] = l.shape();
    const auto [rows2, cols2] = r.shape();
    if (cols1 != rows2) {
        throw std::invalid_argument(
                "Matrixes do not have correct shapes for multiplication");
    }
    size_t res_rows = rows1;
    size_t res_cols = cols2;
    std::vector<std::vector<T>> result(res_rows, std::vector<T>(res_cols));

    for (size_t row = 0; row < res_rows; ++row) {
            std::vector<T> new_row(res_cols);
            for (size_t col = 0; col < res_cols; ++col) {
                T product_sum = T{};
                for (size_t i = 0; i < l.shape().second; ++i) {
                    product_sum += l[row][i] * r[i][col];
                }
                result[row][col] = product_sum;
            }
    }

    return Matrix<T>(result);
}
