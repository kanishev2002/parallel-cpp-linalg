#include <gtest/gtest.h>

#include <utility>

#include "../include/matrix.h"
#include "../src/matrix/matrix.cpp"
#include "../include/split_row_single_thread.h"
#include "../src/Split/split_row_single_thread.cpp"
#include "../include/split_column_single_thread.h"
#include "../src/Split/split_column_single_thread.cpp"


TEST(Matrix, Split_simple_row) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  Matrix<int> mat_split = Split_row_single_thread(mat,0, mat.shape().first,1);
  //std::cout<<mat_split;
  ASSERT_EQ(mat,mat_split);
}

TEST(Matrix, Split_simple_reverse_row) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  Matrix<int> mat_rev({{10,11,12},{7,8,9},{4,5,6},{1,2,3}});
  Matrix<int> mat_split_rev = Split_row_single_thread(mat,mat.shape().first-1,-1, -1);
  //std::cout<<"  "<<mat_split_rev.shape().first<<"     "<<mat_split_rev.shape().second<<"\n";
  //std::cout<<mat_split_rev;
  ASSERT_EQ(mat_split_rev,mat_rev);
}

TEST(Matrix, Split_step_row) {
  Matrix<int> mat({{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0}});
  Matrix<int> mat_check_1({{1,0},{3,0},{5,0},{7,0},{9,0}});
  Matrix<int> mat_split_1 = Split_row_single_thread(mat,0, mat.shape().first, 2);
  //std::cout<<mat_split_1<<"\n\n";
  ASSERT_EQ(mat_check_1,mat_split_1);

  Matrix<int> mat_check_2({{8,0},{6,0},{4,0},{2,0}});
  Matrix<int> mat_split_2 = Split_row_single_thread(mat, mat.shape().first-2,0, -2);
  //std::cout<<mat_split_2<<"\n\n";
  ASSERT_EQ(mat_check_2,mat_split_2);

  Matrix<int> mat_check_3({{9,0},{7,0},{5,0},{3,0}});
  Matrix<int> mat_split_3 = Split_row_single_thread(mat, mat.shape().first-1,0, -2);
  //std::cout<<mat_split_3<<"\n\n";
  ASSERT_EQ(mat_check_3,mat_split_3);
}



TEST(Matrix, Split_simple_column) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  Matrix<int> mat_split = Split_column_single_thread(mat,0, mat.shape().second,1);
  std::cout<<mat_split;
  ASSERT_EQ(mat,mat_split);
}

TEST(Matrix, Split_simple_reverse_column) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  Matrix<int> mat_rev({{3,2,1},{6,5,4},{9,8,7},{12,11,10}});
  Matrix<int> mat_split_rev = Split_column_single_thread(mat,mat.shape().second-1,-1, -1);
  //std::cout<<"  "<<mat_split_rev.shape().first<<"     "<<mat_split_rev.shape().second<<"\n";
  std::cout<<mat_split_rev;
  ASSERT_EQ(mat_split_rev,mat_rev);
}

TEST(Matrix, Split_step_column) {
  Matrix<int> mat({{0,1,2,3,4,5,6,7,8,9},{0,1,2,3,4,5,6,7,8,9}});
  Matrix<int> mat_check_1({{0,2,4,6,8},{0,2,4,6,8}});
  Matrix<int> mat_split_1 = Split_column_single_thread(mat,0, mat.shape().second, 2);
  std::cout<<mat_split_1<<"\n\n";
  ASSERT_EQ(mat_check_1,mat_split_1);

  Matrix<int> mat_check_2({{8,6,4,2},{8,6,4,2}});
  Matrix<int> mat_split_2 = Split_column_single_thread(mat, mat.shape().second-2,0, -2);
  std::cout<<mat_split_2<<"\n\n";
  ASSERT_EQ(mat_check_2,mat_split_2);

  Matrix<int> mat_check_3({{9,7,5,3,1},{9,7,5,3,1}});
  Matrix<int> mat_split_3 = Split_column_single_thread(mat, mat.shape().second-1,0, -2);
  std::cout<<mat_split_3<<"\n\n";
  ASSERT_EQ(mat_check_3,mat_split_3);
}
