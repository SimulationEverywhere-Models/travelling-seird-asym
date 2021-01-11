
#ifndef _DISTRICT__HPP
#define _DISTRICT__HPP

#include "./population.hpp"

#include <iostream>
#include <iomanip>
#include <optional>
#include <vector>
#include <concepts>



template<typename THING>
concept streamable =
    requires(THING a){
        std::cout << a;
        std::cin >> a;
    };

template<typename LHS, typename RHS>
    concept multipliable =
        requires(LHS lhs, RHS rhs){
            rhs *= lhs;
            rhs = lhs*rhs;
        };

template<typename THING>
    concept addable =
        requires(THING a){
            a += a;
            a = a + a;
            a -= a;
            a = a - a;
        };

template<typename VALUE, typename PARAMS>
    concept stepable =
        requires(VALUE v, PARAMS p, double t){
            v = delta(v, p, {t});
        };

template<typename PARAMS, addable VALUE, std::equality_comparable IDENTIFIER=std::string, typename SCALAR=VALUE>
    requires(
        streamable<IDENTIFIER> && streamable<PARAMS> && streamable<VALUE> && streamable<SCALAR> &&
        multipliable<SCALAR, VALUE> && stepable<VALUE, PARAMS>
    )
struct district {
    IDENTIFIER id;
    PARAMS params;
    VALUE  val;
    std::vector<std::pair<IDENTIFIER, SCALAR>> connectivity;

    std::optional<SCALAR> get_connectivity(IDENTIFIER target){
        for(const auto& [t, s] : connectivity){
            if(t == target){
                return s;
            }
        }
        return {};
    }

    std::optional<VALUE> get_travel(IDENTIFIER target){
        auto s = get_connectivity(target);
        if(s){
            return (*s)*val;
        }else{
            return {};
        }
    }

    void set_connectivity(IDENTIFIER target, SCALAR scalar){
        for(auto& [t, s] : connectivity){
            if(t == target){
                s = scalar;
                return;
            }
        }
        connectivity.push_back({target, scalar});
    }

    friend std::ostream& operator<<(std::ostream& os, const district<PARAMS, VALUE, IDENTIFIER, SCALAR>& d){
        os << "[";
        if constexpr(std::same_as<IDENTIFIER, std::string>){
            os << std::quoted(d.id);
        }else{
            os << d.id;
        }
        os << ", ";
        if constexpr(std::same_as<PARAMS, std::string>){
            os << std::quoted(d.params);
        }else{
            os << d.params;
        }
        os << ", ";
        if constexpr(std::same_as<VALUE, std::string>){
            os << std::quoted(d.val);
        }else{
            os << d.val;
        }
        os << ", " << d.connectivity.size() << ", [";

        bool first_one = true;
        for(const auto& [t, s] : d.connectivity){
            if(!first_one){os << ", ";}else{first_one = false;}

            if constexpr(std::same_as<IDENTIFIER, std::string>){
                os << std::quoted(t);
            }else{
                os << t;
            }
            os << ", ";
            if constexpr(std::same_as<SCALAR, std::string>){
                os << std::quoted(s);
            }else{
                os << s;
            }
        }
        os << "]]";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, district<PARAMS, VALUE, IDENTIFIER, SCALAR>& d){
        is.ignore(std::numeric_limits<std::streamsize>::max(), '[');


        if constexpr(std::same_as<IDENTIFIER, std::string>){
            is >> std::quoted(d.id);
        }else{
            is >> d.id;
        }

        is.ignore(std::numeric_limits<std::streamsize>::max(), ',');

        if constexpr(std::same_as<PARAMS, std::string>){
            is >> std::quoted(d.params);
        }else{
            is >> d.params;
        }

        is.ignore(std::numeric_limits<std::streamsize>::max(), ',');

        if constexpr(std::same_as<VALUE, std::string>){
            is >> std::quoted(d.val);
        }else{
            is >> d.val;
        }
        is.ignore(std::numeric_limits<std::streamsize>::max(), ',');

        size_t cons{0};

        is >> cons;

        d.connectivity.clear();

        for(size_t i = 0; i<cons; i++){

            std::pair<IDENTIFIER, SCALAR> p;
            auto& [t, s] = p;

            is.ignore(std::numeric_limits<std::streamsize>::max(), '[');
            if constexpr(std::same_as<IDENTIFIER, std::string>){
                is >> std::quoted(t);
            }else{
                is >> t;
            }

            is.ignore(std::numeric_limits<std::streamsize>::max(), ',');
            if constexpr(std::same_as<SCALAR, std::string>){
                is >> std::quoted(s);
            }else{
                is >> s;
            }

        }

        is.ignore(std::numeric_limits<std::streamsize>::max(), ']');
        is.ignore(std::numeric_limits<std::streamsize>::max(), ']');

        return is;
    }
};

template<typename PARAMS, typename VALUE, typename IDENTIFIER, typename SCALAR>
concept makes_a_valid_district =
    requires(district<PARAMS, VALUE, IDENTIFIER, SCALAR> d){true;};


#endif /* _DISTRICT__HPP */
