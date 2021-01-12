#ifndef _EXAMPLE_PARAMS__HPP
#define _EXAMPLE_PARAMS__HPP

#include <iostream>
#include <concepts>

#include "../atomics/example_population.hpp"
#include "../atomics/example_delta.hpp"

struct example_params {
    double darken_rate;

    auto operator<=>(const example_params&) const = default;
    bool operator==(const example_params&) const = default;
};

std::ostream& operator<<(std::ostream& os, const example_params& mp){
    return os    << "[" << mp.darken_rate << "]";
}

std::istream& operator>>(std::istream& is, example_params& mp){
    is.ignore(std::numeric_limits<std::streamsize>::max(), '[');

    (is >> mp.darken_rate).ignore(std::numeric_limits<std::streamsize>::max(), ']');

    return is;
}

/* delta(PARAMS, POP, TIME) -> DELTA */
/* How much pop goes up by after dt passes*/
constexpr example_delta delta(const example_params& params, const example_population& pop, auto&& dt)
    requires(std::integral<std::remove_cvref_t<decltype(dt)>> || std::floating_point<std::remove_cvref_t<decltype(dt)>>){
    return {
        -pop.red  * params.darken_rate * dt,
        -pop.green* params.darken_rate * dt,
        -pop.blue * params.darken_rate * dt
    };
}

#endif /* _EXAMPLE_PARAMS__HPP */
