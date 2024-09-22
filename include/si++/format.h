#ifndef SIXX_FORMAT_H

#include <fmt/format.h>

#include <algorithm>
#include <concepts>

import Rational;

template <typename T>
    requires std::integral<T>
class fmt::formatter<Rational<T>> {
  public:
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) {
        auto ctx_beg = ctx.begin();
        auto fmt_end = std::find(ctx_beg, ctx.end(), '}');
        if (fmt_end == ctx_beg) {
            return ctx_beg;
        }
        if (*ctx_beg == 'b') {
            bracketed = true;
        } else {
            throw fmt::format_error("invalid rational format specfication");
        }
        ctx.advance_to(std::next(ctx.begin()));
        return fmt_end;
    }

    template <typename FormatContext>
    auto format(Rational<T> const &r, FormatContext &ctx) const {
        if (r.q == 1) {
            return format_to(ctx.out(), "{}", r.p);
        } else {
            if (bracketed) {
                return format_to(ctx.out(), "({}/{})", r.p, r.q);
            } else {
                return format_to(ctx.out(), "{}/{}", r.p, r.q);
            }
        }
    }

  private:
    bool bracketed = false;
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
        auto out = format_to(ctx.out(), "");
        for (int i = 0; i < sizeof(base_names) / sizeof(char *); i++) {
            if (p[i] == 0) {
                continue;
            }
            out = format_to(ctx.out(), "{}{}", first ? "" : "·", base_names[i]);
            if (p[i] != 1) {
                out = format_to(ctx.out(), "^{:b}", p[i]);
            }
            first = false;
        }
        return out;
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
