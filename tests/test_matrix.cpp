#include <gtest/gtest.h>

#include <utility>

#include "../include/matrix.h"
#include "../src/matrix/matrix.cpp"

TEST(Matrix, DefaultConstructors) {
  Matrix<int> empty, default_mat(3, 4);
  ASSERT_EQ(empty.shape(), std::make_pair(0ul, 0ul));
  ASSERT_EQ(default_mat.shape(), std::make_pair(3ul, 4ul));
}

TEST(Matrix, Constructors) {
  std::vector<int> v1 = {1, 2, 3};
  Matrix<int> m1(v1);
  Matrix<int> m2({1, 2, 3});
  Matrix<int> m3(m1);
  Matrix<int> m4(Matrix<int>({1, 2, 3}));
  for (size_t j = 0; j < 3; ++j) {
    ASSERT_EQ(m1[0][j], v1[j]);
    ASSERT_EQ(m2[0][j], v1[j]);
    ASSERT_EQ(m3[0][j], v1[j]);
    ASSERT_EQ(m4[0][j], v1[j]);
  }

  std::vector<std::vector<int>> v2 = {{1, 2, 3}, {4, 5, 6}};
  Matrix<int> m5(v2);
  Matrix<int> m6(Matrix<int>({{1, 2, 3}, {4, 5, 6}}));

  for (size_t i = 0; i < 2; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      ASSERT_EQ(m5[i][j], v2[i][j]);
      ASSERT_EQ(m6[i][j], v2[i][j]);
    }
  }
}

TEST(Matrix, ExceptionConstructors) {
  std::vector<std::vector<int>> half_empty = {{}, {1}, {2}};
  size_t exception_counter = 0;
  try {
    auto m = Matrix(half_empty);
    (void)m;
  } catch (std::invalid_argument&) {
    ++exception_counter;
  } catch (...) {
    std::cerr << "Unknown exception\n";
  }

  std::vector<std::vector<int>> incorrect1 = {{1, 2}, {3}, {4, 5}};
  try {
    auto m = Matrix(incorrect1);
    (void)m;
  } catch (std::invalid_argument&) {
    ++exception_counter;
  } catch (...) {
    std::cerr << "Unknown exception\n";
  }

  std::vector<std::vector<int>> incorrect2 = {{1, 2}, {3, 4}, {5}};
  try {
    auto m = Matrix(incorrect2);
    (void)m;
  } catch (std::invalid_argument&) {
    ++exception_counter;
  } catch (...) {
    std::cerr << "Unknown exception\n";
  }
}

TEST(Matrix, AssignmentOperators) {
  Matrix<int> empty;
  std::vector<std::vector<int>> v = {{1, 2, 3}, {4, 5, 6}};
  empty = Matrix<int>(v);
  ASSERT_EQ(empty[0], v[0]);
  ASSERT_EQ(empty[1], v[1]);

  empty = Matrix<int>({{1, 2}, {3, 4}, {5, 6}});
  ASSERT_EQ(empty[0], std::vector({1, 2}));
  ASSERT_EQ(empty[1], std::vector({3, 4}));
  ASSERT_EQ(empty[2], std::vector({5, 6}));

  empty = Matrix<int>();
  ASSERT_EQ(empty.shape(), std::make_pair(0ul, 0ul));
}

TEST(Matrix, CompareOperators) {
  Matrix<int> empty1, empty2;
  ASSERT_TRUE(empty1 == empty1);
  ASSERT_TRUE(empty1 == empty2);

  ASSERT_FALSE(empty1 != empty1);
  ASSERT_FALSE(empty1 != empty2);

  Matrix<int> matrix1({{1, 2, 3, 4}, {5, 6, 7, 8}});
  ASSERT_FALSE(empty1 == matrix1);
  ASSERT_FALSE(matrix1 == empty1);
  ASSERT_TRUE(matrix1 == matrix1);

  ASSERT_TRUE(empty1 != matrix1);
  ASSERT_TRUE(matrix1 != empty1);
  ASSERT_FALSE(matrix1 != matrix1);

  Matrix<int> matrix2({{1, 2, 3, 4}, {5, 6, 7, 9}});
  ASSERT_FALSE(matrix1 == matrix2);

  ASSERT_TRUE(matrix1 != matrix2);
}

TEST(Matrix, PlusOperator) {
  Matrix<int> empty;
  ASSERT_TRUE(empty + empty == empty);

  Matrix<int> matrix1({{1, 2, 3, 4}, {5, 6, 7, 8}});
  ASSERT_EQ(matrix1 + matrix1, Matrix<int>({{2, 4, 6, 8}, {10, 12, 14, 16}}));
  Matrix<int> zeroes({{0, 0, 0, 0}, {0, 0, 0, 0}});
  ASSERT_EQ(matrix1 + zeroes, matrix1);

  Matrix<int> dif_size_zeroes({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}});
  bool caught_error = false;
  try {
    auto tmp = matrix1 + dif_size_zeroes;
    (void)tmp;
  } catch (std::invalid_argument& invalid_argument) {
    ASSERT_STREQ(invalid_argument.what(), "Matrixes have different shapes\n");
    caught_error = true;
  } catch (...) {
    std::cerr << "Unknown exception\n";
  }
  ASSERT_TRUE(caught_error);
}

TEST(Matrix, MinusOperator) {
  Matrix<int> empty;
  ASSERT_TRUE(empty - empty == empty);

  Matrix<int> matrix1({{1, 2, 3, 4}, {5, 6, 7, 8}});
  Matrix<int> zeroes({{0, 0, 0, 0}, {0, 0, 0, 0}});
  ASSERT_EQ(matrix1 - matrix1, zeroes);
  ASSERT_EQ(zeroes - matrix1,
            Matrix<int>({{-1, -2, -3, -4}, {-5, -6, -7, -8}}));

  Matrix<int> dif_size_zeroes({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}});
  bool caught_error = false;
  try {
    auto tmp = matrix1 - dif_size_zeroes;
    (void)tmp;
  } catch (std::invalid_argument& invalid_argument) {
    ASSERT_STREQ(invalid_argument.what(), "Matrixes have different shapes\n");
    caught_error = true;
  } catch (...) {
    std::cerr << "Unknown exception\n";
  }
  ASSERT_TRUE(caught_error);
}

TEST(Matrix, MultiplpyOperator) {
  Matrix<int> empty;
  ASSERT_TRUE(empty * empty == empty);

  Matrix<int> matrix1({{1, 2, 3, 4}, {5, 6, 7, 8}});
  ASSERT_EQ(matrix1 * matrix1, Matrix<int>({{1, 4, 9, 16}, {25, 36, 49, 64}}));
  Matrix<int> ones({{1, 1, 1, 1}, {1, 1, 1, 1}});
  ASSERT_EQ(matrix1 * ones, matrix1);

  Matrix<int> zeroes({{0, 0, 0, 0}, {0, 0, 0, 0}});
  ASSERT_EQ(matrix1 * zeroes, zeroes);

  Matrix<int> dif_size({{1, 2, 3}, {4, 5, 6}});
  bool caught_error = false;
  try {
    auto tmp = matrix1 * dif_size;
    (void)tmp;
  } catch (std::invalid_argument& invalid_argument) {
    ASSERT_STREQ(invalid_argument.what(), "Matrixes have different shapes\n");
    caught_error = true;
  } catch (...) {
    std::cerr << "Unknown exception\n";
  }
  ASSERT_TRUE(caught_error);
}

TEST(Matrix, Shape) {
  auto zero = Matrix<int>(0, 0);
  ASSERT_EQ(zero.shape(), std::make_pair(0ul, 0ul));
  auto half_zero = Matrix<int>(0, 1);
  ASSERT_EQ(half_zero.shape(), std::make_pair(0ul, 0ul));
  auto half_zero1 = Matrix<int>(1, 0);
  ASSERT_EQ(half_zero1.shape(), std::make_pair(1ul, 0ul));
  auto m = Matrix<int>(1, 1);
  ASSERT_EQ(m.shape(), std::make_pair(1ul, 1ul));
  auto m1 = Matrix<int>(1, 2);
  ASSERT_EQ(m1.shape(), std::make_pair(1ul, 2ul));
  auto m2 = Matrix<int>(2, 1);
  ASSERT_EQ(m2.shape(), std::make_pair(2ul, 1ul));
}
