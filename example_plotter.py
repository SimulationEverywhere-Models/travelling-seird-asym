#!/bin/python3

import matplotlib.pyplot as plt
import re
import math
import itertools
import os
import json



def parse_state_file(in_file):
    time = -1.0
    injectors = {}
    for line in in_file:
        if re.match(r'^-?\d+(?:\.\d+)?$', line):
            time = float(line)
        elif '[' in line:
            try:
                name, params, pop, _, travel = json.loads(line[line.find('['):])
                yield time, name, pop
                #print(time, name, pop)
            except:
                print("####")
                print("")
                print(line.strip())
                print("")
                print("####")
                exit()
        else:
            "State for model rule_changer is 4 to go"
            name = line[len('State for model '):line.rfind(' is')]
            state = line[line.rfind(' is'):]
            if name not in injectors:
                injectors[name] = state
            elif injectors[name] != state:
                injectors[name] = state
                yield time, name, None

def plot_file(in_file):

    plt.figure(figsize=(11, 8.5))
    #plt.xlim(xmin, xmax)
    #plt.ylim(ymin, ymax)

    times  = []
    reds   = []
    greens = []
    blues  = []


    #parse_file = [parse_state_file, parse_messages_file][use_state]
    #for time, name, pop in parse_file(in_file):
    for time, name, pop in parse_state_file(in_file):
        #print(time, len(pops))
        if name == 'Alpha':
            times.append(time)
            reds.append(pop[0])
            greens.append(pop[1])
            blues.append(pop[2])


    plt.plot(times, reds,   label="Red",   c='Red')
    plt.plot(times, greens, label="Green", c='Green')
    plt.plot(times, blues,  label="Blue",  c='Blue')


    plt.legend()

    figname = "colour_change.png"
    print(figname)
    plt.savefig(figname)
    plt.close()



with open(os.path.join(".", "simulation_results", ["output_state.txt", "output_messages.txt"][False])) as in_file:
#with open(os.path.join(".", "simulation_results", )) as in_file:
    plot_file(in_file)

