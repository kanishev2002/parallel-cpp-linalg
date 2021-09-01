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
  EXPECT_LE(diff2.count(), diff1.count());
}

TEST(Dot, Eye_matrix) {
  constexpr size_t mat_size = 100;

  Matrix<int64_t> eye(mat_size, mat_size);
  for (size_t i = 0; i < mat_size; ++i) {
    eye[i][i] = 1ll;
  }
  Matrix<int64_t> mat(mat_size, mat_size);
  for (size_t i = 0; i < mat_size; ++i) {
    for (size_t j = 0; j < mat_size; ++j) {
      mat[i][j] = i | j;
    }
  }

  auto start1 = std::chrono::high_resolution_clock::now();
  auto st_dot = Dot_single_thread(eye, mat);
  auto end1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff1(end1 - start1);

  auto start2 = std::chrono::high_resolution_clock::now();
  auto mt_dot1 = Dot(eye, mat);
  auto end2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff2(end2 - start2);
  std::cout << "Single thread:" << diff1.count() << '\n'
            << "Multithread: " << diff2.count() << '\n';

  auto mt_dot2 = Dot(mat, eye);
  ASSERT_EQ(st_dot, mt_dot1);
  ASSERT_EQ(mt_dot1, mt_dot2);
}

TEST(Dot, Error_handling) {
  Matrix<double> a(20, 10), b(20, 20);
  try {
    ASSERT_NO_THROW(Dot(b, a));
  } catch (...) {
  }

  try {
    ASSERT_THROW(Dot(a, b), std::invalid_argument);
  } catch (const std::invalid_argument& e) {
    ASSERT_STREQ(e.what(),
                 "Matrixes do not have correct shapes for multiplication");
  }
}
