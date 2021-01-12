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

First, define all of the types of `district`s, `district_model`s, and `injector`s that you plan to use

Second, create the `district`s. In the example, this is done with initializer lists, but any other way of creating districts would work just as well.

Third, create the `distric_model`s using `make_dynamic_atomic_model`. These take a `district` of the relevant type, and a `TIME` for `dt`. If `dt` is omitted, the value of `{1}` is used by default.

Fourth, if you plan to use any `injectors` to change the paramiters of a `district` or to add or remove population at some `time>0`, do that now, in much the same way as the `district_model`s. Remember that the time values used are absolute, not reletive to the last change, and the id mush be equal to the id of the district that it effects in order to work.

Fifth, lay out the connections between models. Each `district_model` *must* be connected to itself. Any pair of districts that have any amount of travel *must* be connected. Any `injector` *must* be connected to every district that it effects.

Sixth, run as usuall.

### Making new pieces

See the example models for how.

## Run the model

> ./bin/seird (linux)

> ./bin/seird.exe (windows)

## Visualize the results

./plotter.py is a python3 file that assumes that you are in a linux like environment and that it is run from the root of this repo.

It should serve as an example of how to read the output from this simulator. It generates `infective_pop.png` a chart of log10(infective) v Time in days for each district in the model
