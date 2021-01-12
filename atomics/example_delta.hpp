
#ifndef _EXAMPLE_DELTA__HPP
#define _EXAMPLE_DELTA__HPP

#include <concepts>
#include <compare>
#include <iostream>
#include <limits>

struct example_delta{
    double delta_red;
    double delta_green;
    double delta_blue;

    auto operator<=>(const example_delta&) const = default;
    bool operator==(const example_delta&) const = default;

    /* DELTA += DELTA *
    constexpr example_delta& operator+=(const example_delta& rhs){
        delta_red   += rhs.delta_red;
        delta_green += rhs.delta_green;
        delta_blue  += rhs.delta_blue;
        return *this;
    }

    /* DELTA -= DELTA */
    constexpr example_delta& operator-=(const example_delta& rhs){
        delta_red   -= rhs.delta_red;
        delta_green -= rhs.delta_green;
        delta_blue  -= rhs.delta_blue;
        return *this;
    }

};

std::ostream& operator<<(std::ostream& os, const example_delta& pop){
    return os << "["
              << pop.delta_red   << ", "
              << pop.delta_green << ", "
              << pop.delta_blue  << "]";
}

std::istream& operator>>(std::istream& is, example_delta& pop){
    is.ignore(std::numeric_limits<std::streamsize>::max(), '[');

    (is >> pop.delta_red)  .ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.delta_green).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.delta_blue) .ignore(std::numeric_limits<std::streamsize>::max(), ']');

    return is;

}

#endif /* _EXAMPLE_POPULATION__HPP */

