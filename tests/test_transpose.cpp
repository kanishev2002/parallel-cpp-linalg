#include <gtest/gtest.h>
#include <include/Matrix.h>
#include <include/Transpose.h>

#include <src/Matrix/Matrix.cpp>
#include <src/Transpose/Transpose.cpp>

TEST(Matrix, Transpose_2_by_3) {
  Matrix<int> mat({{1, 2, 3}, {4, 5, 6}});
  Matrix<int> mat_t = Transpose(mat);
  mat_t.print(std::cout);
  Matrix<int> mat_t_check({{1, 4}, {2, 5},{3, 6}});
  ASSERT_EQ(mat_t.shape(),mat_t_check.shape());
  ASSERT_EQ(mat_t,mat_t_check);
  mat_t = Transpose(mat_t);
ASSERT_EQ(mat_t,mat);
}

TEST(Matrix, Transpose_main_diag_symmetry) {
  Matrix<int> mat({{1, 2}, {2, 3}});
  Matrix<int> mat_t = Transpose(mat);
  //mat_t.print(std::cout);
  ASSERT_EQ(mat_t.shape(),mat.shape());
  ASSERT_EQ(mat_t,mat);
}

TEST(Matrix, Transpose_blank) {
  Matrix<int> mat(0,0);
  Matrix<int> mat_t = Transpose(mat);
  //mat_t.print(std::cout);
  ASSERT_EQ(mat_t.shape(),mat.shape());
  ASSERT_EQ(mat_t,mat);
}

TEST(Matrix, Transpose_big_one) {
  int rows = 1000;
  int columns = 2000;
  Matrix<int> mat(rows,columns);
  Matrix<int> mat_t_check(columns,rows);
  for(int i=0;i<rows;++i){
    for(int j=0;j<columns;++j){
      mat[i][j] = i;
      mat_t_check[j][i] = i;
    }
  }
  Matrix<int> mat_t = Transpose(mat);
  ASSERT_EQ(mat_t.shape(),mat_t_check.shape());
  ASSERT_EQ(mat_t,mat_t_check);
}
