#pragma once

#include "matrix.h"

namespace Equation {
template <typename T>
std::vector<T> compose_rows(const std::vector<T>& row1,
                            const std::vector<T>& row2, const T& coef = T(1));

template <typename T>
std::vector<T> multiply_by_scalar(const std::vector<T>& row, const T& scalar);

template <typename T>
void swap_rows(std::vector<T>& row1, std::vector<T>& row2);
}  // namespace Equation
