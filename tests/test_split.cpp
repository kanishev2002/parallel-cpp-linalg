#include <gtest/gtest.h>

#include <utility>

#include "../include/matrix.h"
#include "../src/matrix/matrix.cpp"
#include "../include/split_row_single_thread.h"
#include "../src/Split/split_row_single_thread.cpp"


TEST(Matrix, Split_simple) {
  Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
  auto mat_split = Split_row_single_thread(mat,0, mat.shape().second,1);
  std::cout<<mat_split;
  ASSERT_EQ(mat,mat_split);
}

TEST(Matrix, Split_simple_reverse) {
Matrix<int> mat({{1,2,3},{4,5,6},{7,8,9},{10,11,12}});
Matrix<int> mat_rev({{10,11,12},{7,8,9},{4,5,6},{1,2,3}});
auto mat_split_rev = Split_row_single_thread(mat,mat.shape().second,0, -1);
std::cout<<mat_split_rev;
ASSERT_EQ(mat_split_rev,mat_rev);
}