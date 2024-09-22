#ifndef SIXX_FORMAT_H

#include <fmt/format.h>

#include <concepts>

import Rational;

template <typename T>
    requires std::integral<T>
class fmt::formatter<Rational<T>> {
  public:
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(Rational<T> const &r, FormatContext &ctx) {
        if (r.q == 1) {
            return format_to(ctx.out(), "{}", r.p);
        } else {
            return format_to(ctx.out(), "{}/{}", r.p, r.q);
        }
    }
};

template <>
class fmt::formatter<SI::SIUnit> {
  public:
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(SI::SIUnit const &u, FormatContext &ctx) const {
        auto p = reinterpret_cast<Rational<SI::siint_t> const *>(&u);
        bool first = true;
        std::string out;
        for (int i = 0; i < sizeof(base_names) / sizeof(char *); i++) {
            if (p[i] == 0) {
                continue;
            }
            if (!first) {
                out += "·";
            }
            out += base_names[i];
            // out += p[i] != 1 ? fmt::format("{:b}", p[i]) : "";
            first = false;
        }
        return format_to(ctx.out(), "{}", out);
    }

  private:
    static constexpr const char *base_names[] = {
        "m", "kg", "s", "A", "K", "mol", "cd"
    };
};

template <typename T, SI::SIUnit u>
    requires std::floating_point<T>
class fmt::formatter<SI::DimensionfulFloat<T, u>> : public fmt::formatter<T> {
  public:
    template <typename Context>
    auto format(SI::DimensionfulFloat<T, u> const &f, Context &ctx) const {
        if (u == SI::BaseUnits::Dimensionless) {
            return formatter<T>::format(*f, ctx);
        } else {
            formatter<T>::format(*f, ctx);
            return format_to(ctx.out(), " {}", u);
        }
    }
};

#endif
