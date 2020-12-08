# Travelling-SERID-Asym

This instructions assume that:

1. Cadmium has been installed following the instructions in the manual:
http://www.sce.carleton.ca/courses/sysc-5104/lib/exe/fetch.php?media=cadmiumusermanual.pdf

2. This repo is cloned inside the folder: Cadmium-Simulation-Environment/DEVS-Models
(see the installation manual)

## Compile the model

1. Install Cadmium following the instructions in the manual:
http://www.sce.carleton.ca/courses/sysc-5104/lib/exe/fetch.php?media=cadmiumusermanual.pdf

2. Clone the repository inside the folder: *Cadmium-Simulation-Environment/DEVS-Models*
https://github.com/SimulationEverywhere-Models/Cadmium-SEIR-Asyn

3. Compile the model using the make file

## Model implimentations

See https://github.com/SimulationEverywhere-Models/Cadmium-SEIR-Asyn and https://github.com/SimulationEverywhere-Models/Cadmium-SEIRD as both models in this repo are reimplementations of those

## Modify the model

### Using existing tools

Create the districts using ether the initilizer list constructors, or your own functions with the parameters that you want to use.

Optionally include travel rates in the form of {destination_name, population_scalar} pairs in the last field of the district.
That scalar represents the proportion of the population in the source district that moves to the destination district over a time step of 1.

Also optionally create some number or params_changers. These allow you to specify that at a specific time in the simulation, the parameters of a district should change.

Create atomic models with those districts as shown in the example ./top_model/main.cpp

Add an external connection for any district that you want to take final output from.

Add an internal connection between each district and itself, and between and source,destination pairs of districts that people can go between.

### Making new tools

#### New Params

A params is any struct with an `std::ostream& operator<<(std::ostream&, const T&);`, an `std::istream& operator>>(std::istream&, T&);`, and a `population delta(const population& pop, const T& params, const double& dt);`

`delta` calculates a change in pop given params over dt.

#### New populations

This repo does not have any other implementations of population yet.

A new population needs a pair of << and >> operations, +=, -=, \*=, +, -, \*, and unnary -.
It can optionally have OTHER*T to allow a scalar class other than itself.

If you make your own population, you should also make at least 1 params to go along with it.

#### Other template specializations

By default, districts use std::string as it's identifyer. This can be replaced with any class that implements ==, <<, and >>

By default, districts use the same class for it's population and it's scalar. This can be changed. Valid scalars are any class with <<, >>, and POPULATION=SCALAR\*POPULATION

## Run the model

> ./bin/seird (linux)

> ./bin/seird.exe (windows)

## Visualize the results

./plotter.py is a python3 file that assumes that you are in a linux like environment and that it is run from the root of this repo.

It should serve as an example of how to read the output from this simulator. It generates `log_of_infective.png` a chart of log10(infective) v Time in days for each district in the model
