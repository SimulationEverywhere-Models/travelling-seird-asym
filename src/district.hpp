
#ifndef _DISTRICT__HPP
#define _DISTRICT__HPP

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

template<typename TIME, typename PARAMS, typename POP, typename DELTA, typename TRAVEL, typename IDENTIFIER>
concept makes_a_valid_district =
    requires(TIME dt, PARAMS params, POP pop, DELTA delta_pop, TRAVEL travel_params, IDENTIFIER id){
        {id == id}                       -> std::convertible_to<bool>;
        {delta_pop += delta_pop}         -> std::convertible_to<DELTA&>;
        {delta_pop -= delta_pop}         -> std::convertible_to<DELTA&>;
        {travel(travel_params, pop, dt)} -> std::convertible_to<DELTA>;
        {delta(params, pop, dt)}         -> std::convertible_to<DELTA>;
        {pop += delta_pop}               -> std::convertible_to<POP&>;
    };

/* district<TIME, PARAMS, POP, DELTA, TRAVEL, IDENTIFIER> */
template<typename TIME, streamable PARAMS, streamable POP, streamable DELTA=POP, streamable TRAVEL=POP, streamable IDENTIFIER=std::string>
    requires(
        makes_a_valid_district<TIME, PARAMS, POP, DELTA, TRAVEL, IDENTIFIER>
    )
struct district {
    IDENTIFIER id;
    PARAMS params;
    POP  pop;
    std::vector<std::pair<IDENTIFIER, TRAVEL>> connectivity;

    std::optional<TRAVEL> get_connectivity(IDENTIFIER target) const{
        for(const auto& [t, s] : connectivity){
            if(t == target){
                return s;
            }
        }
        return {};
    }

    std::optional<DELTA> get_travel(IDENTIFIER target, TIME dt) const{
        auto s = get_connectivity(target);
        if(s){
            return travel((*s), pop, dt);
        }else{
            return {};
        }
    }

    void set_connectivity(IDENTIFIER target, TRAVEL t){
        for(auto& [key, val] : connectivity){
            if(key == target){
                val = t;
                return;
            }
        }
        connectivity.push_back({target, t});
    }

    DELTA get_delta(TIME t) const{
        return delta(params, pop, t);
    }

    friend std::ostream& operator<<(std::ostream& os, const district<TIME, PARAMS, POP, DELTA, TRAVEL, IDENTIFIER>& d){
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
        if constexpr(std::same_as<POP, std::string>){
            os << std::quoted(d.pop);
        }else{
            os << d.pop;
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
            if constexpr(std::same_as<TRAVEL, std::string>){
                os << std::quoted(s);
            }else{
                os << s;
            }
        }
        os << "]]";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, district<TIME, PARAMS, POP, DELTA, TRAVEL, IDENTIFIER>& d){
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

        if constexpr(std::same_as<POP, std::string>){
            is >> std::quoted(d.pop);
        }else{
            is >> d.pop;
        }
        is.ignore(std::numeric_limits<std::streamsize>::max(), ',');

        size_t cons{0};

        is >> cons;

        d.connectivity.clear();

        for(size_t i = 0; i<cons; i++){

            std::pair<IDENTIFIER, TRAVEL> p;
            auto& [t, s] = p;

            is.ignore(std::numeric_limits<std::streamsize>::max(), '[');
            if constexpr(std::same_as<IDENTIFIER, std::string>){
                is >> std::quoted(t);
            }else{
                is >> t;
            }

            is.ignore(std::numeric_limits<std::streamsize>::max(), ',');
            if constexpr(std::same_as<TRAVEL, std::string>){
                is >> std::quoted(s);
            }else{
                is >> s;
            }

            d.connectivity.push_back(p);

        }

        is.ignore(std::numeric_limits<std::streamsize>::max(), ']');
        is.ignore(std::numeric_limits<std::streamsize>::max(), ']');

        return is;
    }
};


#endif /* _DISTRICT__HPP */
