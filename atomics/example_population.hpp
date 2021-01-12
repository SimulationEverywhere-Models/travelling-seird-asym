
#ifndef _EXAMPLE_POPULATION__HPP
#define _EXAMPLE_POPULATION__HPP

#include <concepts>
#include <compare>
#include <iostream>
#include <limits>

#include "../atomics/example_delta.hpp"

struct example_population{
    double red;
    double green;
    double blue;

    auto operator<=>(const example_population&) const = default;
    bool operator==(const example_population&) const = default;

    /* POP += DELTA */
    constexpr example_population& operator+=(const example_delta& rhs){
        red   += rhs.delta_red;
        green += rhs.delta_green;
        blue  += rhs.delta_blue;
        return *this;
    }

};

std::ostream& operator<<(std::ostream& os, const example_population& pop){
    return os << "["
              << pop.red    << ", "
              << pop.green  << ", "
              << pop.blue   << "]";
}

std::istream& operator>>(std::istream& is, example_population& pop){
    is.ignore(std::numeric_limits<std::streamsize>::max(), '[');

    (is >> pop.red)  .ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.green).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.blue) .ignore(std::numeric_limits<std::streamsize>::max(), ']');

    return is;

}


#endif /* _EXAMPLE_POPULATION__HPP */

