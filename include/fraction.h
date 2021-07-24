#pragma once

#include <numeric>
#include <tuple>

template<typename T>
class Fraction {
 private:
  T num_, denom_;

  static T gcd_(T a, T b);

  static std::pair<T, T> Reduce(T num, T denom);

 public:
  Fraction(T num = T(0), T denom = T(1));

  T Numerator() const;
  T Denominator() const;
  bool operator==(const Fraction& other) const;
  bool operator!=(const Fraction& other) const;
  Fraction operator+() const;
  Fraction& operator++();
  Fraction operator++(int);
  Fraction operator-() const;
  Fraction& operator--();
  Fraction operator--(int);
  Fraction operator+(const Fraction& other) const;
  Fraction& operator+=(const Fraction& other);
  Fraction operator-(const Fraction& other) const;
  Fraction& operator-=(const Fraction& other);
  Fraction operator*(const Fraction& other) const;
  Fraction& operator*=(const Fraction& other);
  Fraction operator/(const Fraction& other) const;
  Fraction& operator/=(const Fraction& other);
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Fraction<T>& fraction);

template<typename T>
Fraction<T> operator+(T t, const Fraction<T>& fraction);

template<typename T>
Fraction<T> operator-(T t, const Fraction<T>& fraction);

template<typename T>
Fraction<T> operator*(T t, const Fraction<T>& fraction);

template<typename T>
Fraction<T> operator/(T t, const Fraction<T>& fraction);
