#pragma once

#include "Matrix.h"

template<typename T>
std::pair<Matrix<T>, Matrix<T>> FindQR(const Matrix<T>& a);

template<typename T>
bool IsUpTriangular(const Matrix<T>& a, const T& delta);

template<typename T>
std::vector<std::vector<T>> SolveMultipleSolutions(Matrix<T>&& a, const T& delta = T(0.05));

template<typename T>
std::pair<std::vector<T>, Matrix<T>> Eig(const Matrix<T>& a, const T& delta = T(0.01));
