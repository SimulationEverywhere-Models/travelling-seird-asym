#ifndef _DISTRICT_MODEL_HPP__
#define _DISTRICT_MODEL_HPP__

#include <limits>
#include <assert.h>
#include <string>
#include <random>
#include <cmath>
#include <concepts>

#include "./district.hpp"

#define concept concept_old
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#undef concept


using namespace cadmium;
using namespace std;



template<typename TIME, typename PARAMS, typename VALUE, typename IDENTIFIER=std::string, typename SCALAR=VALUE>
    requires(makes_a_valid_district<PARAMS, VALUE, IDENTIFIER, SCALAR>)
class district_model{
    public:
    //Port definition
    struct ports{
        struct report          : public out_port<VALUE> { };
        struct people_out      : public out_port<pair<IDENTIFIER, VALUE>> { };

        struct new_travel_rate : public in_port<tuple<IDENTIFIER, IDENTIFIER, SCALAR>> { };
        struct new_params      : public in_port<pair<IDENTIFIER, PARAMS>> { };
        struct people_in       : public in_port<pair<IDENTIFIER, VALUE>> { };
    };

    const TIME dt{0.01};
    using district_type=district<PARAMS, VALUE, IDENTIFIER, SCALAR>;
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

    // state definition
    struct state_type{
        district_type d;
    };
    state_type state;

    // default constructor
	district_model() = default;

	// constructor with passed variable
    district_model(district_type var) {
        state.d = var;
	}

    // internal transition
    void internal_transition() const {
        /*intentionally left blank*/
    }

	// external transition
    void external_transition(TIME, typename make_message_bags<input_ports>::type mbs) {
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
                state.d.val += pop;
            }
        }
    }


    // time_advance function
    TIME time_advance() const {
		return {dt};
    }

    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        /* Report your pop before this call */
        get_messages<typename ports::report>(bags).push_back(state.d.val);

        /* Calculate how much you should tell yourself to change */
        auto d = delta(state.d.val, state.d.params, dt);

        /*
            For each connection, calculate how many people of what classes are going there from here
            Send them away, and subtract them from the number of people that we are going to have next step
         */
        for(auto [target, s] : state.d.connectivity){
            auto temp = s*state.d.val*dt;
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

    friend ostringstream& operator<<(ostringstream& os, const typename district_model<TIME, PARAMS, VALUE, IDENTIFIER, SCALAR>::state_type& i) {
        os << i.d;
		return os;
    }

};
#endif // _DISTRICT_MODEL_HPP__
