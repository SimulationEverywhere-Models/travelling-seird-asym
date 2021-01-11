#ifndef _SEIRD_ASYM_PAPER_PARAMS__HPP
#define _SEIRD_ASYM_PAPER_PARAMS__HPP

/*

see: github.com/SimulationEverywhere-Models/Cadmium-SEIR-Asyn for this model on it's own

paper:

Tang, B., Wang, X., Li, Q., Bragazzi, N. L., Tang, S., Xiao, Y., & Wu, J. (2020).
Estimation of the Transmission Risk of the 2019-nCoV and Its Implication for
Public Health Interventions. Journal of clinical medicine, 9(2), 462.
https://doi.org/10.3390/jcm9020462

*/

#include <iostream>
#include <concepts>

/*the ./atomics/../atomics/foo.hpp works just fine*/

#include "../atomics/population.hpp"

struct seird_asym_paper_params {
    double c;  /* contact_rate  */
    double b;  /* infectivity_from_contact */
    double q;  /* quarantined_chance_from_contact */
    double e;  /* advacement_e */
    double l;  /* advacement_sq */
    double n;  /* proportion_with_symptoms */
    double di; /* self_quarantined_rate */
    double dq; /* advacement_eq */
    double yi; /* recovery_rate_i */
    double ya; /* recovery_rate_a */
    double yh; /* recovery_rate_q */
    double a;  /* death_rate */
    double t;  /* asymptomatic_infectivity_conversion_factor */

    auto operator<=>(const seird_asym_paper_params&) const = default;
    bool operator==(const seird_asym_paper_params&) const = default;
};

std::ostream& operator<<(std::ostream& os, const seird_asym_paper_params& mp){
    return os    << "["
        << mp.c  << ", " /* contact_rate  */
        << mp.b  << ", " /* infectivity_from_contact */
        << mp.q  << ", " /* quarantined_chance_from_contact */
        << mp.e  << ", " /* advacement_e */
        << mp.l  << ", " /* advacement_sq */
        << mp.n  << ", " /* proportion_with_symptoms */
        << mp.di << ", " /* self_quarantined_rate */
        << mp.dq << ", " /* advacement_eq */
        << mp.yi << ", " /* recovery_rate_i */
        << mp.ya << ", " /* recovery_rate_a */
        << mp.yh << ", " /* recovery_rate_q */
        << mp.a  << ", " /* death_rate */
        << mp.t  << "]"; /* asymptomatic_infectivity_conversion_factor */
}

std::istream& operator>>(std::istream& is, seird_asym_paper_params& mp){
    is.ignore(std::numeric_limits<std::streamsize>::max(), '[');

    (is >> mp.c ).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.b ).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.q ).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.e ).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.l ).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.n ).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.di).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.dq).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.yi).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.ya).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.yh).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.a ).ignore(std::numeric_limits<std::streamsize>::max(), ',');
    (is >> mp.t ).ignore(std::numeric_limits<std::streamsize>::max(), ']');

    return is;
}

constexpr double susceptible_to_exposed      (const population& pop, const seird_asym_paper_params& params){
    return params.c * ( pop.infective + params.t * pop.asymptomatic ) * pop.susceptible *   params.b *(1-params.q);
}
constexpr double susceptible_to_exposed_q    (const population& pop, const seird_asym_paper_params& params){
    return params.c * ( pop.infective + params.t * pop.asymptomatic ) * pop.susceptible *   params.b *   params.q;
}
constexpr double susceptible_to_susceptible_q(const population& pop, const seird_asym_paper_params& params){
    return params.c * ( pop.infective + params.t * pop.asymptomatic ) * pop.susceptible *(1-params.b)*   params.q;
}

constexpr double susceptible_q_to_susceptible(const population& pop, const seird_asym_paper_params& params){return params.l * pop.susceptible_q;}

constexpr double exposed_to_infective       (const population& pop, const seird_asym_paper_params& params){return params.e *   params.n * pop.exposed;}
constexpr double exposed_to_asymptomatic    (const population& pop, const seird_asym_paper_params& params){return params.e *(1-params.n)* pop.exposed;}

constexpr double exposed_q_to_infective_q   (const population& pop, const seird_asym_paper_params& params){return params.dq * pop.exposed_q;}

constexpr double infective_to_infective_q(const population& pop, const seird_asym_paper_params& params){return params.di * pop.infective;}
constexpr double infective_to_recovered  (const population& pop, const seird_asym_paper_params& params){return params.yi * pop.infective;}
constexpr double infective_to_deceased   (const population& pop, const seird_asym_paper_params& params){return params.a  * pop.infective;}

constexpr double infective_q_to_recovered(const population& pop, const seird_asym_paper_params& params){return params.yh * pop.infective_q;}
constexpr double infective_q_to_deceased (const population& pop, const seird_asym_paper_params& params){return params.a  * pop.infective_q;}

constexpr double asymptomatic_to_recovered     (const population& pop, const seird_asym_paper_params& params){return params.ya * pop.asymptomatic;}

/* asymptomatic_q is not in the paper, so they get the same equations as infective_q*/
constexpr double asymptomatic_q_to_recovered(const population& pop, const seird_asym_paper_params& params){return params.yh * pop.asymptomatic_q;}
constexpr double asymptomatic_q_to_deceased (const population& pop, const seird_asym_paper_params& params){return params.a  * pop.asymptomatic_q;}

/*

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

*/
__attribute__((flatten)) constexpr  population delta(const population& pop, const seird_asym_paper_params& params, const double dt){
    population temp{
        /* susceptible */
            +susceptible_q_to_susceptible(pop, params)
            -susceptible_to_susceptible_q(pop, params)
            -susceptible_to_exposed(pop, params)
            -susceptible_to_exposed_q(pop, params)
        ,/* susceptible_q */
            +susceptible_to_susceptible_q(pop, params)
            -susceptible_q_to_susceptible(pop, params)
        ,/* exposed */
            +susceptible_to_exposed(pop, params)
            -exposed_to_infective(pop, params)
            -exposed_to_asymptomatic(pop, params)
        ,/* exposed_q */
            +susceptible_to_exposed_q(pop, params)
            -exposed_q_to_infective_q(pop, params)
        ,/* infective */
            +exposed_to_infective(pop, params)
            -infective_to_infective_q(pop, params)
            -infective_to_recovered(pop, params)
            -infective_to_deceased(pop, params)
        ,/* infective_q */
            +exposed_q_to_infective_q(pop, params)
            +infective_to_infective_q(pop, params)
            -infective_q_to_recovered(pop, params)
            -infective_q_to_deceased(pop, params)
        ,/* asymptomatic */
            +exposed_to_asymptomatic(pop, params)
            -asymptomatic_to_recovered(pop, params)
        ,/* asymptomatic_q */
            -asymptomatic_q_to_recovered(pop, params)
            -asymptomatic_q_to_deceased(pop, params)
        ,/* recovered */
            +infective_to_recovered(pop, params)
            +infective_q_to_recovered(pop, params)
            +asymptomatic_to_recovered(pop, params)
            +asymptomatic_q_to_recovered(pop, params)
        ,/* deceased */
            +infective_to_deceased(pop, params)
            +infective_q_to_deceased(pop, params)
            +asymptomatic_q_to_deceased(pop, params)
    };
    temp*=dt;
    return temp;
}

#endif /* _SEIRD_ASYM_PAPER_PARAMS__HPP */
