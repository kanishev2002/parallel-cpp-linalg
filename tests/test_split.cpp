#include <gtest/gtest.h>

#include <utility>

#include "../include/matrix.h"
#include "../src/matrix/matrix.cpp"

#include "../include/split_row_single_thread.h"
#include "../src/Split/split_row_single_thread.cpp"
#include "../include/split_column_single_thread.h"
#include "../src/Split/split_column_single_thread.cpp"

#include "../include/split_row.h"
#include "../src/Split/split_row.cpp"
#include "../include/split_column.h"
#include "../src/Split/split_column.cpp"


TEST(Matrix, Split_simple_row) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  Matrix<int> mat_split_single = Split_row_single_thread(mat,0, mat.shape().first,1);
  Matrix<int> mat_split = Split_row(mat,0, mat.shape().first,1);
  ASSERT_EQ(mat,mat_split_single);
  ASSERT_EQ(mat,mat_split);
}

TEST(Matrix, Split_simple_reverse_row) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  Matrix<int> mat_rev({{10,11,12},{7,8,9},{4,5,6},{1,2,3}});
  Matrix<int> mat_split_single_rev = Split_row_single_thread(mat,mat.shape().first-1,-1, -1);
  Matrix<int> mat_split_rev = Split_row(mat,mat.shape().first-1,-1, -1);
  ASSERT_EQ(mat_split_single_rev,mat_rev);
  ASSERT_EQ(mat_split_rev,mat_rev);
}

TEST(Matrix, Split_step_row) {
  Matrix<int> mat({{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0}});
  Matrix<int> mat_check_1({{1,0},{3,0},{5,0},{7,0},{9,0}});
  Matrix<int> mat_split_1_single = Split_row_single_thread(mat,0, mat.shape().first, 2);
  Matrix<int> mat_split_1 = Split_row(mat,0, mat.shape().first, 2);

  ASSERT_EQ(mat_check_1,mat_split_1_single);
  ASSERT_EQ(mat_check_1,mat_split_1);

  Matrix<int> mat_check_2({{8,0},{6,0},{4,0},{2,0}});
  Matrix<int> mat_split_2_single = Split_row_single_thread(mat, mat.shape().first-2,0, -2);
  Matrix<int> mat_split_2 = Split_row(mat, mat.shape().first-2,0, -2);
  ASSERT_EQ(mat_check_2,mat_split_2_single);
  ASSERT_EQ(mat_check_2,mat_split_2);

  Matrix<int> mat_check_3({{9,0},{7,0},{5,0},{3,0}});
  Matrix<int> mat_split_3_single = Split_row_single_thread(mat, mat.shape().first-1,0, -2);
  Matrix<int> mat_split_3 = Split_row(mat, mat.shape().first-1,0, -2);
  ASSERT_EQ(mat_check_3,mat_split_3_single);
  ASSERT_EQ(mat_check_3,mat_split_3);
}

TEST(Matrix, Split_row_simple_big) {
  constexpr size_t rows = 10000;
  constexpr size_t columns = 100000;
  Matrix<int> big_matrix(rows, columns);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      big_matrix[i][j] = i;
    }
  }

  auto single_begin = std::chrono::high_resolution_clock::now();
  Matrix<int> mat_split_single = Split_row_single_thread(big_matrix,0, big_matrix.shape().first,1);
  auto single_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff_single(single_end - single_begin);

  auto begin = std::chrono::high_resolution_clock::now();
  Matrix<int> mat_split = Split_row(big_matrix,0, big_matrix.shape().first,1);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff(end - begin);

  std::cout<<"\nsingle: "<< diff_single.count() << "\t\tparallel: " << diff.count() << "\t\tratio: "
              <<(diff.count())/(diff_single.count()) << "\n";

  EXPECT_LE(diff.count(), diff_single.count());
}

TEST(Matrix, Split_simple_column) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  Matrix<int> mat_split_single = Split_column_single_thread(mat,0, mat.shape().second,1);
  Matrix<int> mat_split = Split_column(mat,0, mat.shape().second,1);
  ASSERT_EQ(mat,mat_split_single);
  ASSERT_EQ(mat,mat_split);
}

TEST(Matrix, Split_simple_reverse_column) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  Matrix<int> mat_rev({{3,2,1},{6,5,4},{9,8,7},{12,11,10}});
  Matrix<int> mat_split_single_rev = Split_column_single_thread(mat,mat.shape().second-1,-1, -1);
  Matrix<int> mat_split_rev = Split_column(mat,mat.shape().second-1,-1, -1);
  ASSERT_EQ(mat_split_single_rev,mat_rev);
  ASSERT_EQ(mat_split_rev,mat_rev);
}

TEST(Matrix, Split_step_column) {
  Matrix<int> mat({{0,1,2,3,4,5,6,7,8,9},{0,1,2,3,4,5,6,7,8,9}});
  Matrix<int> mat_check_1({{0,2,4,6,8},{0,2,4,6,8}});
  Matrix<int> mat_split_1_single = Split_column_single_thread(mat,0, mat.shape().second, 2);
  Matrix<int> mat_split_1 = Split_column(mat,0, mat.shape().second, 2);
  ASSERT_EQ(mat_check_1,mat_split_1_single);
  ASSERT_EQ(mat_check_1,mat_split_1);

  Matrix<int> mat_check_2({{8,6,4,2},{8,6,4,2}});
  Matrix<int> mat_split_2_single = Split_column_single_thread(mat, mat.shape().second-2,0, -2);
  Matrix<int> mat_split_2 = Split_column(mat, mat.shape().second-2,0, -2);
  ASSERT_EQ(mat_check_2,mat_split_2_single);
  ASSERT_EQ(mat_check_2,mat_split_2);

  Matrix<int> mat_check_3({{9,7,5,3,1},{9,7,5,3,1}});
  Matrix<int> mat_split_3_single = Split_column_single_thread(mat, mat.shape().second-1,0, -2);
  Matrix<int> mat_split_3 = Split_column(mat, mat.shape().second-1,0, -2);
  ASSERT_EQ(mat_check_3,mat_split_3_single);
  ASSERT_EQ(mat_check_3,mat_split_3);
}

TEST(Matrix, Split_column_simple_big) {
  constexpr size_t rows = 10000;
  constexpr size_t columns = 10000;
  Matrix<int> big_matrix(rows, columns);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      big_matrix[i][j] = i;
    }
  }

  auto single_begin = std::chrono::high_resolution_clock::now();
  Matrix<int> mat_split_single = Split_column_single_thread(big_matrix,0, big_matrix.shape().second,1);
  auto single_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff_single(single_end - single_begin);

  auto begin = std::chrono::high_resolution_clock::now();
  Matrix<int> mat_split = Split_column(big_matrix,0, big_matrix.shape().second,1);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff(end - begin);

  std::cout<<"\nsingle: "<< diff_single.count() << "\t\tparallel: " << diff.count() << "\t\tratio: "
                <<(diff.count())/(diff_single.count()) << "\n";

  EXPECT_LE(diff.count(), diff_single.count());
}
