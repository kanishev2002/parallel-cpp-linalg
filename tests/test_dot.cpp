#include <gtest/gtest.h>
#include <include/dot.h>
#include <include/dot_single_thread.h>
#include <include/matrix.h>

#include <chrono>
#include <iostream>
#include <src/dot/dot.cpp>
#include <src/dot/dot_single_thread.cpp>
#include <src/matrix/matrix.cpp>
#include <utility>

TEST(Dot, Simple_dot) {
  Matrix<int> mat_a({{1, 2, 3}, {4, 5, 6}});
  Matrix<int> mat_b({{1, 2}, {3, 4}, {5, 6}});
  Matrix<int> mat_ab({{22, 28}, {49, 64}});
  ASSERT_EQ(mat_ab, Dot(mat_a, mat_b));
  ASSERT_EQ(mat_ab, Dot_single_thread(mat_a, mat_b));
}

TEST(Dot, Big_dot) {
  int row_1 = 200, col_1_row_2 = 200, col_2 = 200;
  Matrix<int> mat_a(row_1, col_1_row_2);
  Matrix<int> mat_b(col_1_row_2, col_2);
  for (int i = 0; i < row_1; ++i) {
    for (int j = 0; j < col_1_row_2; ++j) {
      mat_a[i][j] = 1;
    }
  }
  for (int i = 0; i < col_1_row_2; ++i) {
    for (int j = 0; j < col_2; ++j) {
      mat_b[i][j] = 1;
    }
  }
  Matrix<int> mat_ab(row_1, col_2);
  for (int i = 0; i < row_1; ++i) {
    for (int j = 0; j < col_2; ++j) {
      mat_ab[i][j] = col_1_row_2;
    }
  }
  auto start1 = std::chrono::high_resolution_clock::now();
  Matrix<int> mat_dot = Dot(mat_a, mat_b);
  auto end1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff1(end1 - start1);

  auto start2 = std::chrono::high_resolution_clock::now();
  Matrix<int> mat_dot_single_thread = Dot_single_thread(mat_a, mat_b);
  auto end2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff2(end2 - start2);
  std::cout << "Dot: " << diff1.count()
            << "   Dot_single_thread: " << diff2.count() << "\n";

  ASSERT_EQ(mat_ab, mat_dot);
  ASSERT_EQ(mat_ab, mat_dot_single_thread);
}
