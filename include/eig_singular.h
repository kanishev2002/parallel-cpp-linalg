#pragma once

#include "matrix.h"

template<typename T>
std::pair<Matrix<T>, Matrix<T>> FindQRSingular(const Matrix<T>& a);

template<typename T>
bool IsUpTriangularSingular(const Matrix<T>& a, const T& delta);

template<typename T>
std::vector<std::vector<T>> SolveMultipleSolutionsSingular(Matrix<T>&& a, const T& delta = T(0.05));

template<typename T>
std::pair<std::vector<T>, Matrix<T>> EigSingular(const Matrix<T>& a, const T& delta = T(0.01));