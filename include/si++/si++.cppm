module;
#include <concepts>
#include <string>
export module siplusplus;

import Rational;

export namespace SI {
typedef int siint_t;

struct SIUnit {
    const Rational<siint_t> m;
    const Rational<siint_t> kg;
    const Rational<siint_t> s;
    const Rational<siint_t> A;
    const Rational<siint_t> K;
    const Rational<siint_t> mol;
    const Rational<siint_t> cd;

    // clang-format off: for operator alignment
    constexpr SIUnit operator*(const SIUnit &other) const {
        return SIUnit(
            m   + other.m,
            kg  + other.kg,
            s   + other.s,
            A   + other.A,
            K   + other.K,
            mol + other.mol,
            cd  + other.cd
        );
    }
    constexpr SIUnit operator/(const SIUnit &other) const {
        return SIUnit(
            m   - other.m,
            kg  - other.kg,
            s   - other.s,
            A   - other.A,
            K   - other.K,
            mol - other.mol,
            cd  - other.cd
        );
    }

    constexpr bool operator==(const SIUnit & other) const {
        return   m == other.m
            &&  kg == other.kg
            &&   s == other.s
            &&   A == other.A
            &&   K == other.K
            && mol == other.mol
            &&  cd == other.cd
        ;
    }
    template <typename T>
        requires std::integral<T>
    constexpr SIUnit operator^(T p) const {
        return SIUnit(m * p, kg * p, s * p, A * p, K * p, mol * p, cd * p);
    }
    // clang-format on
};

template <typename T, SIUnit u>
    requires std::floating_point<T>
class DimensionfulFloat {
  public:
    DimensionfulFloat(T _value) : value(_value) {};

    constexpr T operator*() const { return value; }

    template <SIUnit u2>
    constexpr DimensionfulFloat<T, u * u2> operator*(
        const DimensionfulFloat<T, u2> &other
    ) const {
        return {value * other.value};
    }
    constexpr DimensionfulFloat<T, u> operator*(const T &other) const {
        return {value * other};
    }

    template <SIUnit u2>
    constexpr DimensionfulFloat<T, u / u2> operator/(
        const DimensionfulFloat<T, u2> &other
    ) {
        return {value / other.value};
    }
    constexpr DimensionfulFloat<T, u> operator/(const T &other) const {
        return {value / other};
    }

    const T value;
};

namespace BaseUnits {

constexpr SIUnit Dimensionless(0, 0, 0, 0, 0, 0, 0);

// clang-format off : for alignment
constexpr SIUnit m(1, 0, 0, 0, 0, 0, 0);
constexpr SIUnit kg(0, 1, 0, 0, 0, 0, 0);
constexpr SIUnit s(0, 0, 1, 0, 0, 0, 0);
constexpr SIUnit A(0, 0, 0, 1, 0, 0, 0);
constexpr SIUnit K(0, 0, 0, 0, 0, 0, 0);
constexpr SIUnit mol(0, 0, 0, 0, 0, 0, 0);
constexpr SIUnit cd(0, 0, 0, 0, 0, 0, 0);
// clang-format on

}  // namespace BaseUnits

namespace DerivedUnits {

using namespace BaseUnits;
constexpr SIUnit Hz = Dimensionless / s;

constexpr SIUnit N = kg * (m ^ 2) / (s ^ 2);
constexpr SIUnit J = N * m;
constexpr SIUnit Pa = N / (m ^ 2);
constexpr SIUnit W = J / s;

constexpr SIUnit C = A * s;
constexpr SIUnit V = J / C;
constexpr SIUnit F = C / V;
constexpr SIUnit Ω = V / A;
constexpr SIUnit S = A / V;
constexpr SIUnit T = V * s / (m ^ 2);
constexpr SIUnit Wb = T * (m ^ 2);
constexpr SIUnit H = Wb * A;

}  // namespace DerivedUnits

}  // namespace SI

export template <typename T, SI::SIUnit u>
constexpr SI::DimensionfulFloat<T, u> operator*(
    T f, const SI::DimensionfulFloat<T, u> &q
) {
    return q * f;
}

export template <typename T, SI::SIUnit u>
constexpr SI::DimensionfulFloat<T, SI::BaseUnits::Dimensionless / u> operator/(
    T f, const SI::DimensionfulFloat<T, u> &q
) {
    return {f / q.value};
}
