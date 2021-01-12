#ifndef _EXAMPLE_TRAVEL_PARAMS__HPP
#define _EXAMPLE_TRAVEL_PARAMS__HPP

#include <iostream>
#include <concepts>

#include "../atomics/example_population.hpp"
#include "../atomics/example_delta.hpp"

struct example_travel_params {
    double bleed_rate;

    auto operator<=>(const example_travel_params&) const = default;
    bool operator==(const example_travel_params&) const = default;
};

std::ostream& operator<<(std::ostream& os, const example_travel_params& mp){
    return os    << "[" << mp.bleed_rate << "]";
}

std::istream& operator>>(std::istream& is, example_travel_params& mp){
    is.ignore(std::numeric_limits<std::streamsize>::max(), '[');

    (is >> mp.bleed_rate).ignore(std::numeric_limits<std::streamsize>::max(), ']');

    return is;
}

/* travel(TRAVEL, POP, TIME) -> DELTA */
/* how much moved from pop to the destination over dt */
constexpr example_delta travel(const example_travel_params& travel, const example_population& pop, auto&& dt)
    requires(std::integral<std::remove_cvref_t<decltype(dt)>> || std::floating_point<std::remove_cvref_t<decltype(dt)>>){
    return {
        pop.red  * travel.bleed_rate * dt,
        pop.green* travel.bleed_rate * dt,
        pop.blue * travel.bleed_rate * dt,
    };
}

#endif /* _EXAMPLE_TRAVEL_PARAMS__HPP */
