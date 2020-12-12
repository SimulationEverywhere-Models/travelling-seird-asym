
#ifndef _BANDED_ZHONG_POPULATION__HPP
#define _BANDED_ZHONG_POPULATION__HPP

#include <cstdlib>

#include "../atomics/zhong_population.hpp"

template<std::size_t BANDS, std::size_t DURATION>
struct banded_zhong_population{

    zhong_population<DURATION> band[BANDS];

    auto operator<=>(const banded_zhong_population&) const = default;
    bool operator==(const banded_zhong_population&) const = default;
};

template<std::size_t BANDS, std::size_t DURATION>
std::ostream& operator<<(std::ostream& os, const banded_zhong_population<BANDS, DURATION>& pop){
    os << "[";

    for(std::size_t i = 0; i<BANDS; i++){
        if(i){os << ", ";}
        os << pop.band[i];
    }

    return os << "]";
}

template<std::size_t BANDS, std::size_t DURATION>
std::istream& operator>>(std::istream& is, banded_zhong_population<BANDS, DURATION>& pop){

    for(std::size_t i = 0; i<BANDS; i++){
        if(i == 0){
            is.ignore(std::numeric_limits<std::streamsize>::max(), '[');
        }else{
            is.ignore(std::numeric_limits<std::streamsize>::max(), ',');
        }
        is >> pop.band[i];
    }

    is.ignore(std::numeric_limits<std::streamsize>::max(), ']');
    return is;
}


/* bz_p_l += bz_p_r */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION>& operator+=(banded_zhong_population<BANDS, DURATION>& lhs, const banded_zhong_population<BANDS, DURATION>& rhs){
    for(std::size_t i = 0; i<BANDS; i++){
        lhs.band[i] += rhs.band[i];
    }
    return lhs;
}

/* bz_p_l -= bz_p_r */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION>& operator-=(banded_zhong_population<BANDS, DURATION>& lhs, const banded_zhong_population<BANDS, DURATION>& rhs){
    for(std::size_t i = 0; i<BANDS; i++){
        lhs.band[i] -= rhs.band[i];
    }
    return lhs;
}

/* bz_p_l *= bz_p_r */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION>& operator*=(banded_zhong_population<BANDS, DURATION>& lhs, const banded_zhong_population<BANDS, DURATION>& rhs){
    for(std::size_t i = 0; i<BANDS; i++){
        lhs.band[i] *= rhs.band[i];
    }
    return lhs;
}

/* bz_p_l *= number */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION>& operator*=(banded_zhong_population<BANDS, DURATION>& lhs, auto&& rhs) requires(std::integral<std::remove_cvref_t<decltype(rhs)>> || std::floating_point<std::remove_cvref_t<decltype(rhs)>>){
    for(std::size_t i = 0; i<BANDS; i++){
        lhs.band[i] *= rhs;
    }
    return lhs;
}

/* bz_p_l + bz_p_r */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION> operator+(const banded_zhong_population<BANDS, DURATION>& lhs, const banded_zhong_population<BANDS, DURATION>&& rhs){
    zhong_population<DURATION> temp{lhs};
    return temp += rhs;
}

/* bz_p_l - bz_p_r */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION> operator-(const banded_zhong_population<BANDS, DURATION>& lhs, const banded_zhong_population<BANDS, DURATION>&& rhs){
    zhong_population<DURATION> temp{lhs};
    return temp -= rhs;
}

/* bz_p_l * bz_p_r */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION> operator*(const banded_zhong_population<BANDS, DURATION>& lhs, const banded_zhong_population<BANDS, DURATION>&& rhs){
    banded_zhong_population<BANDS, DURATION> temp{lhs};
    return temp *= rhs;
}

/* bz_p_l * number */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION> operator*(const banded_zhong_population<BANDS, DURATION>& lhs,  auto&& rhs) requires(std::integral<std::remove_cvref_t<decltype(rhs)>> || std::floating_point<std::remove_cvref_t<decltype(rhs)>>){
    banded_zhong_population<BANDS, DURATION> temp{lhs};
    return temp += rhs;
}

/* number * bz_p_l */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION> operator*(auto&& lhs, const banded_zhong_population<BANDS, DURATION>& rhs) requires(std::integral<std::remove_cvref_t<decltype(lhs)>> || std::floating_point<std::remove_cvref_t<decltype(lhs)>>){
    return rhs*lhs;
}

/* +bz_p */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION> operator+(const banded_zhong_population<BANDS, DURATION>& p){
    return {p};
}

/* -bz_p */
template<std::size_t BANDS, std::size_t DURATION>
constexpr banded_zhong_population<BANDS, DURATION> operator-(const banded_zhong_population<BANDS, DURATION>& p){
    return p*(-1.0);
}


#endif /* _BANDED_ZHONG_POPULATION__HPP */
