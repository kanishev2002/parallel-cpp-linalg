# Parallel C++ linalg library
A group project to create a basic linear algebra library in C++ that takes advantage of parallel programming.

## Objectives of the project
*TODO*

### Matrix products
* Matrix<T> **dot**(const Matrix<T>& a, const Matrix<T>& b) -- Dot product of two matrixes.
* Matrix<T> **matrix_power**(const Matrix<T>& a, size_t n) -- Raise a square matrix to the (natural) power n.

### Decompositions
* *TODO* **svd**(const Matrix<T>& a) -- Returns Singular Value Decomposition (three matrixes).

### Matrix eigenvalues
* *TODO* **eig**(const Matrix<T>& a) -- Compute the eigenvalues and (right) eigenvectors of a square array.

### Other numbers
* Scalar **det**(const Matrix<T>& a) -- Compute the determinant of a matrix.
* size_t **matrix_rank**(const Matrix<T>& a) -- Return matrix rank using SVD method.
* Scalar **trace**(const Matrix<T>& a) -- Return the sum along main diagonal of the matrix.

### Solving equations and inverting matrixes
* Matrix<T> **solve**(const Matrix<T>& a, const Matrix<T>& b) -- Solve a linear matrix equation, or system of linear scalar equations.
* Matrix<T> **inv**(const Matrix<T>& a) -- Compute the inverse of a matrix.

## How to compile
*TODO*, but probably using make.

## Authors
The project is done by me, [InfinityZxc](https://github.com/InfinityZxc) and [0masiza0](https://github.com/0masiza0)

## License
The library is licensed under the MIT license.
