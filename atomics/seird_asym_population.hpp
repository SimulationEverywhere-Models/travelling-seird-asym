
#ifndef _SEIRD_ASYM_POPULATION__HPP
#define _SEIRD_ASYM_POPULATION__HPP

#include <concepts>
#include <compare>
#include <iostream>
#include <limits>

/*


__attribute__((flatten)) => tells the compiler to inline every made by this function

constexpr => tells the compiler that this is an inlineable function that will allways return the same output givin the same input, and that it can be run at compile time

auto operator<=>(const foo&) const = default; => a shortcut for implementing all of < > <= >=, using structural comparasons.


const everything that can be const

perfer non-friend non-member functions

*/
//#define constexpr inline

struct seird_asym_population{
    double susceptible;
    double susceptible_q;
    double exposed;
    double exposed_q;
    double infective;
    double infective_q;
    double asymptomatic;
    double asymptomatic_q;
    double recovered;
    double deceased;

    auto operator<=>(const seird_asym_population&) const = default;
    bool operator==(const seird_asym_population&) const = default;

        /* POP += DELTA and DELTA += DELTA */
    constexpr seird_asym_population& operator+=(const seird_asym_population& rhs){
        susceptible    += rhs.susceptible;
        susceptible_q  += rhs.susceptible_q;
        exposed        += rhs.exposed;
        exposed_q      += rhs.exposed_q;
        infective      += rhs.infective;
        infective_q    += rhs.infective_q;
        asymptomatic   += rhs.asymptomatic;
        asymptomatic_q += rhs.asymptomatic_q;
        recovered      += rhs.recovered;
        deceased       += rhs.deceased;
        return *this;
    }

    /* DELTA -= DELTA */
    constexpr seird_asym_population& operator-=(const seird_asym_population& rhs){
        susceptible    -= rhs.susceptible;
        susceptible_q  -= rhs.susceptible_q;
        exposed        -= rhs.exposed;
        exposed_q      -= rhs.exposed_q;
        infective      -= rhs.infective;
        infective_q    -= rhs.infective_q;
        asymptomatic   -= rhs.asymptomatic;
        asymptomatic_q -= rhs.asymptomatic_q;
        recovered      -= rhs.recovered;
        deceased       -= rhs.deceased;
        return *this;
    }

};

std::ostream& operator<<(std::ostream& os, const seird_asym_population& pop){
    return os << "["
              << pop.susceptible    << ", "
              << pop.susceptible_q  << ", "
              << pop.exposed        << ", "
              << pop.exposed_q      << ", "
              << pop.infective      << ", "
              << pop.infective_q    << ", "
              << pop.asymptomatic   << ", "
              << pop.asymptomatic_q << ", "
              << pop.recovered      << ", "
              << pop.deceased       << "]";
}

std::istream& operator>>(std::istream& is, seird_asym_population& pop){
    is.ignore(std::numeric_limits<std::streamsize>::max(), '[');

    (is >> pop.susceptible)   .ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.susceptible_q) .ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.exposed)       .ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.exposed_q)     .ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.infective)     .ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.infective_q)   .ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.asymptomatic)  .ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.asymptomatic_q).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.recovered)     .ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> pop.deceased)      .ignore(std::numeric_limits<std::streamsize>::max(), ']');

    return is;

}

constexpr seird_asym_population travel(const seird_asym_population& t, const seird_asym_population& pop, auto&& dt)
    requires(std::integral<std::remove_cvref_t<decltype(dt)>> || std::floating_point<std::remove_cvref_t<decltype(dt)>>)
    {
    return {
            dt * pop.susceptible    * t.susceptible,
            dt * pop.susceptible_q  * t.susceptible_q,
            dt * pop.exposed        * t.exposed,
            dt * pop.exposed_q      * t.exposed_q,
            dt * pop.infective      * t.infective,
            dt * pop.infective_q    * t.infective_q,
            dt * pop.asymptomatic   * t.asymptomatic,
            dt * pop.asymptomatic_q * t.asymptomatic_q,
            dt * pop.recovered      * t.recovered,
            dt * pop.deceased       * t.deceased
    };
}


#endif /* _SEIRD_ASYM_POPULATION__HPP */

