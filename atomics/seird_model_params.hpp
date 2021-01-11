#ifndef _SEIRD_MODEL__HPP
#define _SEIRD_MODEL__HPP

/*

see: github.com/SimulationEverywhere-Models/Cadmium-SEIRD for this model on it's own

*/

#include <iostream>
#include <concepts>
#include <algorithm>

/*the ./atomics/../atomics/foo.hpp works just fine*/

#include "../atomics/population.hpp"

struct seird_model_params{
    double mortality;
    double infectivity_period;
    double incubation_period;
    double transmission_rate;

    auto operator<=>(const seird_model_params&) const = default;
    bool operator==(const seird_model_params&) const = default;
};

std::ostream& operator<<(std::ostream& os, const seird_model_params& mp){
    return os                    << "["
        << mp.mortality          << ", "
        << mp.infectivity_period << ", "
        << mp.incubation_period  << ", "
        << mp.transmission_rate  << "]";
}

std::istream& operator>>(std::istream& is, seird_model_params& mp){
    is.ignore(std::numeric_limits<std::streamsize>::max(), '[');

    (is >> mp.mortality         ).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.infectivity_period).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.incubation_period ).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.transmission_rate ).ignore(std::numeric_limits<std::streamsize>::max(), ']');

    return is;
}

/*population fields

double pop.susceptible;
double pop.susceptible_q;
double pop.exposed;
double pop.exposed_q;
double pop.infective;
double pop.infective_q;
double pop.asymptomatic;
double pop.asymptomatic_q;
double pop.recovered;
double pop.deceased;

*/

/*
    We need to agree with the fields in the population struct. This implements the SEIRD model when all normally unused fields are 0 and makes a best effort to use the other fields sensebly

    I assume that q leads to more q, that q does not move around, and that asymptomatic do not die
*/
__attribute__((flatten)) constexpr  population delta(const population& pop, const seird_model_params& params, const double dt){
    auto total_moving_pop = pop.susceptible + pop.exposed + pop.infective + pop.asymptomatic + pop.recovered;

    auto susceptible_to_exposed = std::min(dt * params.transmission_rate * pop.susceptible * pop.infective / total_moving_pop, pop.susceptible);

    auto exposed_to_infective     = dt * pop.exposed  /params.incubation_period;
    auto exposed_q_to_infective_q = dt * pop.exposed_q/params.incubation_period;

    auto infective_to_deceased   = dt*params.mortality*pop.infective  /params.infectivity_period;
    auto infective_q_to_deceased = dt*params.mortality*pop.infective_q/params.infectivity_period;

    auto infective_to_recovered   = dt*(1-params.mortality)*pop.infective     /params.infectivity_period;
    auto infective_q_to_recovered = dt*(1-params.mortality)*pop.infective_q   /params.infectivity_period;

    auto asymptomatic_to_recovered   = dt*pop.asymptomatic  /params.infectivity_period;
    auto asymptomatic_q_to_recovered = dt*pop.asymptomatic_q/params.infectivity_period;

    return {
        /* susceptible */
            -susceptible_to_exposed
        ,/* susceptible_q */
            0
        ,/* exposed */
            +susceptible_to_exposed
            -exposed_to_infective
        ,/* exposed_q */
            -exposed_q_to_infective_q
        ,/* infective */
            +exposed_to_infective
            -infective_to_deceased
            -infective_to_recovered
        ,/* infective_q */
            +exposed_q_to_infective_q
            -infective_q_to_deceased
            -infective_q_to_recovered
        ,/* asymptomatic */
            -asymptomatic_to_recovered
        ,/* asymptomatic_q */
            -asymptomatic_q_to_recovered
        ,/* recovered */
            +infective_to_recovered
            +infective_q_to_recovered
            +asymptomatic_to_recovered
            +asymptomatic_q_to_recovered
        ,/* deceased */
            +infective_to_deceased
            +infective_q_to_deceased
    };
}

#endif /* _SEIRD_MODEL__HPP */
