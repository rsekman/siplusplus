module;
#include <cassert>
#include <concepts>
#include <numeric>
#include <string>
#include <tuple>
export module Rational;

template <typename T>
    requires std::integral<T>
constexpr std::tuple<T, T> _simplify_fraction(T p, T q) {
    assert(q != 0);
    T g = std::gcd(p, q);
    p /= g;
    q /= g;
    if (q < 0) {
        p = -p;
        q = -q;
    }
    return {p, q};
}
template <typename T>
    requires std::integral<T>
constexpr T _get_numerator(T p, T q) {
    return std::get<0>(_simplify_fraction(p, q));
}
template <typename T>
    requires std::integral<T>
constexpr T _get_denominator(T p, T q) {
    return std::get<1>(_simplify_fraction(p, q));
}

export template <typename T>
    requires std::integral<T>
class Rational {
  public:
    constexpr Rational(T _p, T _q) :
        p(_get_numerator(_p, _q)), q(_get_denominator(_p, _q)) {};
    constexpr Rational(T _p) : p(_p), q(1) {};

    constexpr Rational<T> operator+(const Rational<T> &other) const {
        return {p * other.q + q * other.p, q * other.q};
    }
    constexpr Rational<T> operator-(const Rational<T> &other) const {
        return {p * other.q - q * other.p, q * other.q};
    }

    constexpr Rational<T> operator*(const Rational<T> &other) const {
        return {p * other.p, q * other.q};
    }
    constexpr Rational<T> operator*(T a) { return {p * a, q}; }

    constexpr Rational<T> operator/(const Rational<T> &other) const {
        return {p * other.q, q * other.p};
    }
    constexpr Rational<T> operator/(T a) { return {p, q * a}; }

    constexpr bool operator<=>(const Rational<T> &other) const {
        return p * other.q <=> q * other.p;
    }
    constexpr bool operator==(const Rational<T> &other) const {
        return p * other.q == q * other.p;
    }
    constexpr bool operator!=(const Rational<T> &other) const {
        return p * other.q != q * other.p;
    }
    constexpr bool operator<(const Rational<T> &other) const {
        return p * other.q < q * other.p;
    }
    constexpr bool operator>(const Rational<T> &other) const {
        return p * other.q > q * other.p;
    }
    constexpr bool operator>=(const Rational<T> &other) const {
        return p * other.q >= q * other.p;
    }
    constexpr bool operator<=(const Rational<T> &other) const {
        return p * other.q >= q * other.p;
    }

    // have to be public so that Rational is a structural type
    const T p;
    const T q;
};

export template <typename T>
    requires std::integral<T>
constexpr Rational<T> operator*(T a, Rational<T> b) {
    return b * a;
}
export template <typename T>
    requires std::integral<T>
constexpr Rational<T> operator/(T a, Rational<T> b) {
    return {a * b.q, b.p};
}
