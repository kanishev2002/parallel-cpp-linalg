#include <gtest/gtest.h>
#include <include/matrix.h>
#include <include/solve.h>

#include <src/matrix/matrix.cpp>
#include <src/solve/solve.cpp>

TEST(Solve, Solve_simple) {
  auto A = Matrix<double>(
      std::vector<std::vector<double>>({{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}));
  auto B = Matrix<double>(std::vector<std::vector<double>>({{1}, {2}, {3}}));
  auto ans = Solve(A, B);
  // ans.print(std::cout);
  ASSERT_EQ(ans, B);
}

TEST(Solve, Solve_complex) {
  auto A = Matrix<double>(std::vector<std::vector<double>>(
      {{12930, 17, 921}, {113, 76, 93}, {111, 23, 22}}));
  auto B =
      Matrix<double>(std::vector<std::vector<double>>({{100}, {200}, {300}}));
  auto ans = Solve(A, B);
  // ans.print(std::cout);
}