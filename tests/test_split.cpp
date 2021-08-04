#include <gtest/gtest.h>

#include <utility>

#include "../include/matrix.h"
#include "../src/matrix/matrix.cpp"
#include "../include/split_row_single_thread.h"
#include "../src/Split/split_row_single_thread.cpp"


TEST(Matrix, Split_simple) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  Matrix<int> mat_split = Split_row_single_thread(mat,0, mat.shape().first,1);
  std::cout<<mat_split;
  ASSERT_EQ(mat,mat_split);
}

TEST(Matrix, Split_simple_reverse) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  Matrix<int> mat_rev({{10,11,12},{7,8,9},{4,5,6}});
  Matrix<int> mat_split_rev = Split_row_single_thread(mat,mat.shape().first-1,0, -1);
  //std::cout<<"  "<<mat_split_rev.shape().first<<"     "<<mat_split_rev.shape().second<<"\n";
  std::cout<<mat_split_rev;
  ASSERT_EQ(mat_split_rev,mat_rev);
}

TEST(Matrix, Split_step) {
  Matrix<int> mat({{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0}});
  Matrix<int> mat_check_1({{1,0},{3,0},{5,0},{7,0},{9,0}});
  Matrix<int> mat_split_1 = Split_row_single_thread(mat,0, mat.shape().first, 2);
  std::cout<<mat_split_1<<"\n\n";
  ASSERT_EQ(mat_check_1,mat_split_1);

  Matrix<int> mat_check_2({{8,0},{6,0},{4,0},{2,0}});
  Matrix<int> mat_split_2 = Split_row_single_thread(mat, mat.shape().first-2,0, -2);
  std::cout<<mat_split_2<<"\n\n";
  ASSERT_EQ(mat_check_2,mat_split_2);

  Matrix<int> mat_check_3({{9,0},{7,0},{5,0},{3,0}});
  Matrix<int> mat_split_3 = Split_row_single_thread(mat, mat.shape().first-1,0, -2);
  std::cout<<mat_split_3<<"\n\n";
  ASSERT_EQ(mat_check_3,mat_split_3);
}
