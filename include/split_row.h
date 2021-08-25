#pragma once

#include "matrix.h"

template <typename T>
Matrix<T> Split_row(const Matrix<T>& mat, const int64_t from, const int64_t to, const int64_t step);