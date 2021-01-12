CC=g++
CFLAGS=-std=c++20

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

default: all

example_main.o: top_model/example_main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/example_main.cpp -o build/example_main.o

example: example_main.o
	$(CC) -g -o bin/example build/example_main.o


paper_main.o: top_model/paper_main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/paper_main.cpp -o build/paper_main.o

paper: paper_main.o
	$(CC) -g -o bin/paper build/paper_main.o

#TARGET TO COMPILE EVERYTHING
all: example paper

#CLEAN COMMANDS
clean:
	rm -f bin/* build/*

