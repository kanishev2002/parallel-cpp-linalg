#include "../../include/fraction.h"

template<typename T>
T Fraction<T>::gcd_(T a, T b) {
    if (b == T(0))
        return a;
    else
        return gcd_(b, a % b);
}

template<typename T>
std::pair<T, T> Fraction<T>::Reduce(T num, T denom) {
    T gcd = gcd_(num, denom);
    T sign = (num * denom < T(0)) ? T(-1) : T(1);
    num = sign * (num > T(0) ? num : -num) / gcd;
    denom = (denom > T(0) ? denom : -denom) / gcd;
    return { num, denom };
}

template<typename T>
Fraction<T>::Fraction(T num, T denom) {
    std::tie(num_, denom_) = Reduce(num, denom);
}

template<typename T>
T Fraction<T>::Numerator() const {
    return num_;
}

template<typename T>
T Fraction<T>::Denominator() const {
    return denom_;
}

template<typename T>
bool Fraction<T>::operator==(const Fraction& other) const {
    return (num_ == other.num_ && denom_ == other.denom_);
}

template<typename T>
bool Fraction<T>::operator!=(const Fraction& other) const {
    return !(*this == other);
}

template<typename T>
Fraction Fraction<T>::operator+() const {
    return *this;
}

template<typename T>
Fraction& Fraction<T>::operator++() {
    *this += T(1);
    return *this;
}

template<typename T>
Fraction Fraction<T>::operator++(int) {
    Fraction old = *this;
    ++(*this);
    return old;
}

template<typename T>
Fraction Fraction<T>::operator-() const {
    return Fraction(-num_, denom_);
}

template<typename T>
Fraction& Fraction<T>::operator--() {
    *this -= T(1);
    return *this;
}

template<typename T>
Fraction Fraction<T>::operator--(int) {
    Fraction old = *this;
    --(*this);
    return old;
}

template<typename T>
Fraction Fraction<T>::operator+(const Fraction& other) const {
    return Fraction(num_ * other.denom_ + denom_ * other.num_,
                    denom_ * other.denom_);
}

template<typename T>
Fraction& Fraction<T>::operator+=(const Fraction& other) {
    num_ = num_ * other.denom_ + denom_ * other.num_;
    denom_ *= other.denom_;
    std::tie(num_, denom_) = Reduce(num_, denom_);
    return *this;
}

template<typename T>
Fraction Fraction<T>::operator-(const Fraction& other) const {
    return *this + (-other);
}

template<typename T>
Fraction& Fraction<T>::operator-=(const Fraction& other) {
    *this += (-other);
    return *this;
}

template<typename T>
Fraction Fraction<T>::operator*(const Fraction& other) const {
    return Fraction(num_ * other.num_, denom_ * other.denom_);
}

template<typename T>
Fraction& Fraction<T>::operator*=(const Fraction& other) {
    num_ *= other.num_;
    denom_ *= other.denom_;
    std::tie(num_, denom_) = Reduce(num_, denom_);
    return *this;
}

template<typename T>
Fraction Fraction<T>::operator/(const Fraction& other) const {
    return Fraction(num_ * other.denom_, denom_ * other.num_);
}

template<typename T>
Fraction& Fraction<T>::operator/=(const Fraction& other) {
    *this *= Fraction(other.denom_, other.num_);
    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Fraction<T>& fraction) {
    return out << fraction.Numerator() << "/" << fraction.Denominator();
}

template<typename T>
Fraction<T> operator+(T t, const Fraction<T>& fraction) {
    return Fraction(t) + fraction;
}

template<typename T>
Fraction<T> operator-(T t, const Fraction<T>& fraction) {
    return Fraction(t) - fraction;
}

template<typename T>
Fraction<T> operator*(T t, const Fraction<T>& fraction) {
    return Fraction(t) * fraction;
}

template<typename T>
Fraction<T> operator/(T t, const Fraction<T>& fraction) {
    return Fraction(t) / fraction;
}
