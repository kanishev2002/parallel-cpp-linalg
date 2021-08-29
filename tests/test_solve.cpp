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
  ASSERT_EQ(ans, B);
}

TEST(Solve, Solve_complex) {
  auto A = Matrix<double>(std::vector<std::vector<double>>(
      {{12930, 17, 921}, {113, 76, 93}, {111, 23, 22}}));
  auto B =
      Matrix<double>(std::vector<std::vector<double>>({{100}, {200}, {300}}));
  auto ans = Solve(A, B);
  auto correct_ans = Matrix<double>(std::vector<std::vector<double>>(
      {{1.4546105}, {25.91052994}, {-20.79108884}}));  // calculated by numpy
  ASSERT_EQ(correct_ans.shape(), ans.shape());
  for (size_t i = 0; i < 3; ++i) {
    ASSERT_NEAR(ans[i][0], correct_ans[i][0], 0.001);
  }
}

TEST(Solve, No_solution) {
  auto A = Matrix<double>(
      std::vector<std::vector<double>>({{1, 0, 0}, {0, 1, 0}, {0, 0, 0}}));
  auto B = Matrix<double>(std::vector<std::vector<double>>({{1}, {2}, {3}}));
  ASSERT_THROW(Solve(A, B), std::invalid_argument);
}