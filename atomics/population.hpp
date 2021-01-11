

#ifndef _POPULATION__HPP
#define _POPULATION__HPP

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

struct population {
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

    auto operator<=>(const population&) const = default;
    bool operator==(const population&) const = default;

};

std::ostream& operator<<(std::ostream& os, const population& pop){
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

std::istream& operator>>(std::istream& is, population& pop){
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

/* pop_l += pop_r */
constexpr population& operator+=(auto& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>){
    lhs.susceptible    += rhs.susceptible;
    lhs.susceptible_q  += rhs.susceptible_q;
    lhs.exposed        += rhs.exposed;
    lhs.exposed_q      += rhs.exposed_q;
    lhs.infective      += rhs.infective;
    lhs.infective_q    += rhs.infective_q;
    lhs.asymptomatic   += rhs.asymptomatic;
    lhs.asymptomatic_q += rhs.asymptomatic_q;
    lhs.recovered      += rhs.recovered;
    lhs.deceased       += rhs.deceased;
    return lhs;
}

/* pop_l -= pop_r */
constexpr population& operator-=(auto& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>){
    lhs.susceptible    -= rhs.susceptible;
    lhs.susceptible_q  -= rhs.susceptible_q;
    lhs.exposed        -= rhs.exposed;
    lhs.exposed_q      -= rhs.exposed_q;
    lhs.infective      -= rhs.infective;
    lhs.infective_q    -= rhs.infective_q;
    lhs.asymptomatic   -= rhs.asymptomatic;
    lhs.asymptomatic_q -= rhs.asymptomatic_q;
    lhs.recovered      -= rhs.recovered;
    lhs.deceased       -= rhs.deceased;
    return lhs;
}

/* pop_l *= pop_r */
constexpr population& operator*=(auto& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>){
    lhs.susceptible    *= rhs.susceptible;
    lhs.susceptible_q  *= rhs.susceptible_q;
    lhs.exposed        *= rhs.exposed;
    lhs.exposed_q      *= rhs.exposed_q;
    lhs.infective      *= rhs.infective;
    lhs.infective_q    *= rhs.infective_q;
    lhs.asymptomatic   *= rhs.asymptomatic;
    lhs.asymptomatic_q *= rhs.asymptomatic_q;
    lhs.recovered      *= rhs.recovered;
    lhs.deceased       *= rhs.deceased;
    return lhs;
}

/* pop_l *= number */
constexpr population& operator*=(auto& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && (std::integral<std::remove_cvref_t<decltype(rhs)>> || std::floating_point<std::remove_cvref_t<decltype(rhs)>>)){
    lhs.susceptible    *= rhs;
    lhs.susceptible_q  *= rhs;
    lhs.exposed        *= rhs;
    lhs.exposed_q      *= rhs;
    lhs.infective      *= rhs;
    lhs.infective_q    *= rhs;
    lhs.asymptomatic   *= rhs;
    lhs.asymptomatic_q *= rhs;
    lhs.recovered      *= rhs;
    lhs.deceased       *= rhs;
    return lhs;
}

/* pop_l + pop_r */
__attribute__((flatten)) constexpr population operator+(auto&& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>){
    population temp{lhs};
    return temp += rhs;
}

/* pop_l - pop_r */
__attribute__((flatten)) constexpr population operator-(auto&& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>){
    population temp{lhs};
    return temp -= rhs;
}

/* pop_l * pop_r */
/* pop_l * number */
__attribute__((flatten)) constexpr population operator*(auto&& lhs, auto&& rhs)
        requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && (
            std::same_as<population, std::remove_cvref_t<decltype(rhs)>> ||
            std::integral<std::remove_cvref_t<decltype(rhs)>> ||
            std::floating_point<std::remove_cvref_t<decltype(rhs)>>))
{
    population temp{lhs};
    return temp *= rhs;
}

/* number * pop_r */
__attribute__((flatten)) constexpr population operator*(auto& lhs, auto&& rhs)
        requires((std::integral<std::remove_cvref_t<decltype(lhs)>> || std::floating_point<std::remove_cvref_t<decltype(lhs)>>)
            && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>)
{
    return rhs*lhs;
}

/* +pop */
__attribute__((flatten)) constexpr population operator+(auto&& p) requires std::same_as<population, std::remove_cvref_t<decltype(p)>> {
    return {p};
}

/* -pop */
__attribute__((flatten)) constexpr population operator-(auto&& p) requires std::same_as<population, std::remove_cvref_t<decltype(p)>> {
    return p*(-1);
}

//#undef constexpr
#endif /* _POPULATION__HPP */

