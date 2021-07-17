#include <gtest/gtest.h>
#include <parallel_linalg_lib/Matrix.h>

TEST(Matrix, Equals) {
  Matrix<int> m({{1, 2}, {3, 4}});
  ASSERT_TRUE(m == m);
}