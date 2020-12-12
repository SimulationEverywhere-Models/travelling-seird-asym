
#ifndef _ZHONG_POPULATION__HPP
#define _ZHONG_POPULATION__HPP

#include <cstdlib>
#include <iostream>
#include <limits>
#include <concepts>

template<std::size_t DURATION>
struct zhong_population{
    double susceptable;
    double stage[DURATION];
    double deceased;

    auto operator<=>(const zhong_population&) const = default;
    bool operator==(const zhong_population&) const = default;

};

template<std::size_t DURATION>
std::ostream& operator<<(std::ostream& os, const zhong_population<DURATION>& pop){
    os << "[" << pop.susceptable << ", ";

    for(std::size_t i = 0; i<DURATION; i++){
        os << pop.stage[i] << ", ";
    }

    return os << pop.deceased << "]";
}

template<std::size_t DURATION>
std::istream& operator>>(std::istream& is, zhong_population<DURATION>& pop){
    is.ignore(std::numeric_limits<std::streamsize>::max(), '[');
    (is >> pop.susceptable).ignore(std::numeric_limits<std::streamsize>::max(), ',');

    for(std::size_t i = 0; i<DURATION; i++){
        (is >> pop.stage[i]).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    }

    (is >> pop.deceased).ignore(std::numeric_limits<std::streamsize>::max(), ']');

    return is;
}

/* z_p_l += z_p_r */
template<std::size_t DURATION>
constexpr auto& operator+=(zhong_population<DURATION>& lhs, const zhong_population<DURATION>& rhs){
    lhs.susceptable += rhs.susceptable;
    for(std::size_t i = 0; i<DURATION; i++){
        lhs.stage[i] += rhs.stage[i];
    }
    lhs.dead += rhs.dead;
    return lhs;
}

/* z_p_l -= z_p_r  */
template<std::size_t DURATION>
constexpr auto& operator-=(zhong_population<DURATION>& lhs, const zhong_population<DURATION>& rhs){
    lhs.susceptable -= rhs.susceptable;
    for(std::size_t i = 0; i<DURATION; i++){
        lhs.stage[i] -= rhs.stage[i];
    }
    lhs.dead -= rhs.dead;
    return lhs;
}

/* z_p_l *= z_p_r  */
template<std::size_t DURATION>
constexpr auto& operator*=(zhong_population<DURATION>& lhs, const zhong_population<DURATION>& rhs){
    lhs.susceptable *= rhs.susceptable;
    for(std::size_t i = 0; i<DURATION; i++){
        lhs.stage[i] *= rhs.stage[i];
    }
    lhs.dead *= rhs.dead;
    return lhs;
}

/* z_p_l *= number */
template<std::size_t DURATION>
constexpr auto& operator*=(zhong_population<DURATION>& lhs, auto&& rhs) requires(std::integral<std::remove_cvref_t<decltype(rhs)>> || std::floating_point<std::remove_cvref_t<decltype(rhs)>>){
    lhs.susceptable *= rhs;
    for(std::size_t i = 0; i<DURATION; i++){
        lhs.stage[i] *= rhs;
    }
    lhs.dead *= rhs;
    return lhs;
}

/* z_p_l + z_p_r */
template<std::size_t DURATION>
constexpr zhong_population<DURATION> operator+(const zhong_population<DURATION>& lhs, const zhong_population<DURATION>& rhs){
    zhong_population<DURATION> temp{lhs};
    return temp += rhs;
}

/* z_p_l - z_p_r */
template<std::size_t DURATION>
constexpr zhong_population<DURATION> operator-(const zhong_population<DURATION>& lhs, const zhong_population<DURATION>& rhs){
    zhong_population<DURATION> temp{lhs};
    return temp -= rhs;
}

/* z_p_l * z_p_r */
template<std::size_t DURATION>
constexpr zhong_population<DURATION> operator*(const zhong_population<DURATION>& lhs, const zhong_population<DURATION>& rhs){
    zhong_population<DURATION> temp{lhs};
    return temp *= rhs;
}

/* z_p_l * number */
template<std::size_t DURATION>
constexpr zhong_population<DURATION> operator*(const zhong_population<DURATION>& lhs,  auto&& rhs) requires(std::integral<std::remove_cvref_t<decltype(rhs)>> || std::floating_point<std::remove_cvref_t<decltype(rhs)>>){
    zhong_population<DURATION> temp{lhs};
    return temp += rhs;
}

/* number * z_p_l */
template<std::size_t DURATION>
constexpr zhong_population<DURATION> operator*(auto&& lhs, const zhong_population<DURATION>& rhs) requires(std::integral<std::remove_cvref_t<decltype(lhs)>> || std::floating_point<std::remove_cvref_t<decltype(lhs)>>){
    return rhs*lhs;
}

/* +z_p */
template<std::size_t DURATION>
constexpr zhong_population<DURATION> operator+(const zhong_population<DURATION>& p){
    return {p};
}

/* -z_p */
template<std::size_t DURATION>
constexpr zhong_population<DURATION> operator-(const zhong_population<DURATION>& p){
    return p*(-1.0);
}

#endif /* _ZHONG_POPULATION__HPP */
