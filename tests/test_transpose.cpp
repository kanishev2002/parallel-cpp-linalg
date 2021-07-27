#include <gtest/gtest.h>
#include <include/matrix.h>
#include <include/transpose.h>

#include <chrono>
#include <src/Matrix/matrix.cpp>
#include <src/Transpose/transpose.cpp>
#include <src/Transpose/transpose_single_thread.cpp>

TEST(Matrix, Transpose_2_by_3) {
  Matrix<int> mat({{1, 2, 3}, {4, 5, 6}});
  Matrix<int> mat_t = Transpose(mat);
  mat_t.print(std::cout);
  Matrix<int> mat_t_check({{1, 4}, {2, 5}, {3, 6}});
  ASSERT_EQ(mat_t.shape(), mat_t_check.shape());
  ASSERT_EQ(mat_t, mat_t_check);
  mat_t = Transpose(mat_t);
  ASSERT_EQ(mat_t, mat);
}

TEST(Matrix, Transpose_main_diag_symmetry) {
  Matrix<int> mat({{1, 2}, {2, 3}});
  Matrix<int> mat_t = Transpose(mat);
  // mat_t.print(std::cout);
  ASSERT_EQ(mat_t.shape(), mat.shape());
  ASSERT_EQ(mat_t, mat);
}

TEST(Matrix, Transpose_blank) {
  Matrix<int> mat(0, 0);
  Matrix<int> mat_t = Transpose(mat);
  // mat_t.print(std::cout);
  ASSERT_EQ(mat_t.shape(), mat.shape());
  ASSERT_EQ(mat_t, mat);
}

TEST(Matrix, Transpose_big_one) {
  constexpr size_t rows = 1000;
  constexpr size_t columns = 2000;
  Matrix<int> mat(rows, columns);
  Matrix<int> mat_t_check(columns, rows);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      mat[i][j] = i;
      mat_t_check[j][i] = i;
    }
  }
  Matrix<int> mat_t = Transpose(mat);
  ASSERT_EQ(mat_t.shape(), mat_t_check.shape());
  ASSERT_EQ(mat_t, mat_t_check);
}

TEST(Matrix, Compare_to_single_thread) {
  constexpr size_t rows = 100;
  constexpr size_t columns = 200;
  Matrix<int> big_matrix(rows, columns);
  Matrix<int> big_matrix_transposed(columns, rows);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      big_matrix[i][j] = i;
      big_matrix_transposed[j][i] = i;
    }
  }
  Matrix<int> big_matrix1 = big_matrix;
  auto st_begin = std::chrono::high_resolution_clock::now();
  auto Transposed_single_thread = Transpose_single_thread(big_matrix1);
  auto st_end = std::chrono::high_resolution_clock::now();

  auto mt_begin = std::chrono::high_resolution_clock::now();
  auto Transposed_multithread = Transpose(big_matrix);
  auto mt_end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> st_res(st_end - st_begin);
  std::chrono::duration<double> mt_res(mt_end - mt_begin);

  ASSERT_EQ(Transposed_multithread, Transposed_single_thread);
  EXPECT_LE(mt_res.count(), st_res.count());
}

TEST(Matrix, Compare_to_single_thread_big) {
  constexpr size_t rows = 1000;
  constexpr size_t columns = 2000;
  Matrix<int> big_matrix(rows, columns);
  Matrix<int> big_matrix_transposed(columns, rows);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      big_matrix[i][j] = i;
      big_matrix_transposed[j][i] = i;
    }
  }
  Matrix<int> big_matrix1 = big_matrix;
  auto st_begin = std::chrono::high_resolution_clock::now();
  auto Transposed_single_thread = Transpose_single_thread(big_matrix1);
  auto st_end = std::chrono::high_resolution_clock::now();

  auto mt_begin = std::chrono::high_resolution_clock::now();
  auto Transposed_multithread = Transpose(big_matrix);
  auto mt_end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> st_res(st_end - st_begin);
  std::chrono::duration<double> mt_res(mt_end - mt_begin);

  ASSERT_EQ(Transposed_multithread, Transposed_single_thread);
  EXPECT_LE(mt_res.count(), st_res.count());
}
