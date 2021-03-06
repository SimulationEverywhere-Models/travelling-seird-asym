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


def parse_messages_file(in_file):
    time = -1.0
    pops = {}
    for line in in_file:
        if re.match(r'^-?\d+(?:\.\d+)?$', line):
            time = float(line)
        elif line.startswith('[injector'):
            name = line[line.rfind(']')+len('] generated by model '):].strip()
            yield time, name, None
        elif '[' in line:
            try:
                pop = None
                if not line.startswith('[injector'):
                    pop = json.loads(line[line.find('{')+1:line.find('}')])

                name = line[line.rfind(']')+len('] generated by model '):].strip()
                yield time, name, pop
                #print(time, name, pop)
            except:
                print("####")
                print("")
                print(line.strip())
                print("")
                print("####")
                #exit()


def plot_file(in_file, use_state, show_injections = True):
    xmin, xmax, ymin, ymax = 0, 55, 1, 5.5

    plt.figure(figsize=(11, 8.5))
    plt.title("Log10(Infective population) v Time in Days")
    plt.xlim(xmin, xmax)
    plt.ylim(ymin, ymax)

    vals = {}
    times = {}
    peeks = {}
    injections = {}

    parse_file = [parse_state_file, parse_messages_file][use_state]
    for time, name, pop in parse_file(in_file):
    #for time, name, pop in parse_messages_file(in_file):
        #print(time, len(pops))
        if pop is None:
            if name not in injections:
                injections[name] = set()
            injections[name].add(time)
        else:
            infective = pop[4]
            if name not in vals:
                vals[name] = []
                times[name] = []
            if (name not in peeks) or (peeks[name][1] < infective):
                peeks[name] = (time, infective)
            vals[name].append(infective)
            times[name].append(time)

    for name, val in vals.items():
        plt.plot(times[name], [math.log10(i) for i in val], label=name)

    if show_injections:
        for name in injections:
            #for time in injections[name]:
            plt.vlines(x=list(injections[name]), ymin=ymin, ymax=ymax, label=name, linestyle='dotted')


    plt.legend()

    figname = "infective_pop.png"
    print(figname)
    plt.savefig(figname)
    plt.close()
    for name, p in peeks.items():
        print(f"{name}, {p[0]-12:0.5}, {p[1]}")



use_state = True

with open(os.path.join(".", "simulation_results", ["output_state.txt", "output_messages.txt"][use_state])) as in_file:
#with open(os.path.join(".", "simulation_results", )) as in_file:
    plot_file(in_file, use_state)

