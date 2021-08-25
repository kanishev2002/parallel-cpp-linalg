#include "../../include/inv.h"
#include "../../include/solve.h"

template<typename T>
Matrix<T> Inv(const Matrix<T>& a) {
    return Solve(a, Matrix<T>::eye(a.shape().first));
}
