#ifndef _DISTRICT_MODEL_HPP__
#define _DISTRICT_MODEL_HPP__

#include <limits>
#include <assert.h>
#include <string>
#include <random>
#include <cmath>
#include <concepts>

#include "../src/district.hpp"

#define concept concept_old
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#undef concept


using namespace cadmium;
using namespace std;


/* district_model<TIME, PARAMS, POP, DELTA, TRAVEL, IDENTIFIER> */
template<typename TIME, typename PARAMS, typename POP, typename DELTA=POP, typename TRAVEL=POP, typename IDENTIFIER=std::string>
    requires(makes_a_valid_district<TIME, PARAMS, POP, DELTA, TRAVEL, IDENTIFIER>)
class district_model{
    public:
    //Port definition
    struct ports{
        struct report          : public out_port<POP> { };
        struct people_out      : public out_port<pair<IDENTIFIER, DELTA>> { };

        struct new_travel_rate : public in_port<tuple<IDENTIFIER, IDENTIFIER, TRAVEL>> { };
        struct new_params      : public in_port<pair<IDENTIFIER, PARAMS>> { };
        struct people_in       : public in_port<pair<IDENTIFIER, DELTA>> { };
    };

    using district_type=district<TIME, PARAMS, POP, DELTA, TRAVEL, IDENTIFIER>;
    // ports definition
    using input_ports=std::tuple<
        typename ports::new_travel_rate,
        typename ports::new_params,
        typename ports::people_in
         >;

    using output_ports=std::tuple<
        typename ports::report,
        typename ports::people_out
         >;

    TIME dt = {1};
    // state definition
    struct state_type{
        district_type d;
        TIME time_till_report;

        auto operator<=>(const state_type&) const = default;
        bool operator==(const state_type&) const = default;
    };
    state_type state;

    // default constructor
	district_model() = default;

	// constructor with passed variable and default dt
    district_model(district_type var): dt(1) {
        state.d = var;
        state.time_till_report = 0;
	}

	// constructor with passed variable
    district_model(district_type var, TIME t): dt(t) {
        state.d = var;
        state.time_till_report = 0;
	}

    // internal transition
    void internal_transition() {
        state.time_till_report = dt;
    }

	// external transition
    void external_transition(TIME t, typename make_message_bags<input_ports>::type mbs) {
        state.time_till_report -= t;

        /* for each new travel rate, if they are talking to me, update my travel rates */
        for(const auto& [me, id, s] : get_messages<typename ports::new_travel_rate>(mbs)){
            if(me == state.d.id){
                state.d.set_connectivity(id, s);
            }
        }

        /* for each new set of morel params, if they are talking to me, update my params */
        for(const auto& [me, params] : get_messages<typename ports::new_params>(mbs)){
            if(me == state.d.id){
                state.d.params = params;
            }
        }

        /* for each new set of morel params, if they are talking to me, update my params */
        for(const auto& [me, pop] : get_messages<typename ports::people_in>(mbs)){
            if(me == state.d.id){
                state.d.pop += pop;
            }
        }
    }


    // time_advance function
    TIME time_advance() const {
		return {state.time_till_report};
    }

    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        /* Report your pop before this call */
        get_messages<typename ports::report>(bags).push_back(state.d.pop);

        /* Calculate how much you should tell yourself to change */
        auto d = state.d.get_delta(dt);

        /*
            For each connection, calculate how many people of what classes are going there from here
            Send them away, and subtract them from the number of people that we are going to have next step
         */
        for(auto [target, s] : state.d.connectivity){
            auto temp = *(state.d.get_travel(target, dt));
            d -= temp;
            get_messages<typename ports::people_out>(bags).push_back({target, temp});
        }

        /* Remember to send the message to your self */
        get_messages<typename ports::people_out>(bags).push_back({state.d.id, d});
        return bags;
    }

    // confluence transition
    void confluence_transition(TIME, typename make_message_bags<input_ports>::type mbs) {
		internal_transition();
        external_transition({}, move(mbs));
    }

    friend ostringstream& operator<<(ostringstream& os, const typename district_model<TIME, PARAMS, POP, DELTA, TRAVEL, IDENTIFIER>::state_type& i) {
        os << i.d;
		return os;
    }

};
#endif // _DISTRICT_MODEL_HPP__
