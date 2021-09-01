# Parallel C++ linalg library
A group project to create a basic linear algebra library in C++ that takes advantage of parallel programming.

## Objectives of the project

### Matrix products
* Matrix<T> **dot**(const Matrix<T>& a, const Matrix<T>& b) -- Dot product of two matrixes.
* Matrix<T> **matrix_power**(const Matrix<T>& a, size_t n) -- Raise a square matrix to the (natural) power n.

### Matrix eigenvalues
* *TODO* **eig**(const Matrix<T>& a) -- Compute the eigenvalues and (right) eigenvectors of a square array.

### Other numbers
* Scalar **det**(const Matrix<T>& a) -- Compute the determinant of a matrix.
* Scalar **trace**(const Matrix<T>& a) -- Return the sum along main diagonal of the matrix.

### Solving equations and inverting matrixes
* Matrix<T> **solve**(const Matrix<T>& a, const Matrix<T>& b) -- Solve a linear matrix equation, or system of linear scalar equations.
* Matrix<T> **inv**(const Matrix<T>& a) -- Compute the inverse of a matrix.

## How to compile
To run tests, do the following:
  1. Clone this repository
  2. Go to the directory with the repository and run `cmake . && make -j`
  3. To run a specific test, do `./tests/build/test_<name of the test>`
  4. To run all tests, do `./tests/build/all_tests`

## Authors
The project is done by me, [InfinityZxc](https://github.com/InfinityZxc) and [0masiza0](https://github.com/0masiza0)

## License
The library is licensed under the MIT license.

## Documentation
For docs see [DOCUMENTATION](./DOCUMENTATION.md)
