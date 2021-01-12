
//Time class header
#include <NDTime.hpp>

//C++ libraries
#include <iostream>
#include <string>

//Atomic model headers
#define concept concept_old

//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Atomic model for inputs
#include <cadmium/basic_model/pdevs/iestream.hpp>

#undef concept

#include "../atomics/example_population.hpp"
#include "../atomics/example_delta.hpp"
#include "../atomics/example_params.hpp"
#include "../atomics/example_travel_params.hpp"

#include "../src/district.hpp"
#include "../src/district_model.hpp"


using namespace std;
using namespace cadmium;
//using namespace cadmium::basic_models::pdevs;

using TIME = double;

/***** Define input port for coupled models *****/
// none for this test


/***** Define output ports for coupled model *****/
struct SEIRD_defs{
    struct report : public out_port<example_population> { };
};


/*************** Loggers *******************/
    static ofstream out_messages("simulation_results/output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){
            return out_messages;
        }
    };
    static ofstream out_state("simulation_results/output_state.txt");
    struct oss_sink_state{
        static ostream& sink(){
            return out_state;
        }
    };

    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;
/******************************************************/


/* template specalizations for this main, we need to leave TIME unspecialized for dynamic::translate::make_dynamic_atomic_model() to work correctly */
using example_district = district<TIME, example_params, example_population, example_delta, example_travel_params>;
template<typename T> using example_model = district_model<T, example_params, example_population, example_delta, example_travel_params>;

int main(){
    TIME dt = 0.0001;
	/****** Station Passenger Generator instantiations *******************/
        /* see ../atomics/population.hpp and for a description of what is what. This can also be constructed piecemeal */

    example_district d_a{"Alpha",
        {0.00001},
        {255, 0, 0},
        {{"Bravo", {0.99}}, {"Charlie", {0.01}}}
    };

    example_district d_b{"Bravo",
        {0.00001},
        {0, 255, 0},
        {{"Charlie", {0.99}}, {"Alpha", {0.01}}}
    };

    example_district d_c{"Charlie",
        {0.00001},
        {0, 0, 255},
        {{"Alpha", {0.99}}, {"Bravo", {0.01}}}
    };

	shared_ptr<dynamic::modeling::model> m_a = dynamic::translate::make_dynamic_atomic_model<example_model, TIME, example_district, TIME>("Alpha",   std::move(d_a), TIME(dt));
	shared_ptr<dynamic::modeling::model> m_b = dynamic::translate::make_dynamic_atomic_model<example_model, TIME, example_district, TIME>("Bravo",   std::move(d_b), TIME(dt));
	shared_ptr<dynamic::modeling::model> m_c = dynamic::translate::make_dynamic_atomic_model<example_model, TIME, example_district, TIME>("Charlie", std::move(d_c), TIME(dt));

    dynamic::modeling::Ports iports_example = {};
    dynamic::modeling::Ports oports_example  = {
            typeid(SEIRD_defs::report)};

    dynamic::modeling::Models submodels_example  = {
            m_a, m_b, m_c};

    dynamic::modeling::EICs eics_example  = {};
    dynamic::modeling::EOCs eocs_example  = {
            dynamic::translate::make_EOC<example_model<TIME>::ports::report, SEIRD_defs::report>("Alpha"),
            dynamic::translate::make_EOC<example_model<TIME>::ports::report, SEIRD_defs::report>("Bravo"),
            dynamic::translate::make_EOC<example_model<TIME>::ports::report, SEIRD_defs::report>("Charlie")};

    /* Each model MUST be piped into itself to function */

    dynamic::modeling::ICs ics_example  = {
            dynamic::translate::make_IC<example_model<TIME>::ports::people_out, example_model<TIME>::ports::people_in>("Alpha", "Alpha"),
            dynamic::translate::make_IC<example_model<TIME>::ports::people_out, example_model<TIME>::ports::people_in>("Alpha", "Bravo"),
            dynamic::translate::make_IC<example_model<TIME>::ports::people_out, example_model<TIME>::ports::people_in>("Alpha", "Charlie"),
            dynamic::translate::make_IC<example_model<TIME>::ports::people_out, example_model<TIME>::ports::people_in>("Bravo", "Alpha"),
            dynamic::translate::make_IC<example_model<TIME>::ports::people_out, example_model<TIME>::ports::people_in>("Bravo", "Bravo"),
            dynamic::translate::make_IC<example_model<TIME>::ports::people_out, example_model<TIME>::ports::people_in>("Bravo", "Charlie"),
            dynamic::translate::make_IC<example_model<TIME>::ports::people_out, example_model<TIME>::ports::people_in>("Charlie", "Alpha"),
            dynamic::translate::make_IC<example_model<TIME>::ports::people_out, example_model<TIME>::ports::people_in>("Charlie", "Bravo"),
            dynamic::translate::make_IC<example_model<TIME>::ports::people_out, example_model<TIME>::ports::people_in>("Charlie", "Charlie")};

    shared_ptr<dynamic::modeling::coupled<TIME>> EXAMPLE  = make_shared<dynamic::modeling::coupled<TIME>>(
            "Example ", submodels_example , iports_example , oports_example , eics_example , eocs_example , ics_example
        );

    dynamic::engine::runner<TIME, logger_top> r(EXAMPLE, {0});
    r.run_until(5);

    return 0;
}
