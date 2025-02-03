# Copyright 2023 CSCE 311

cc = g++  # use g++ compiler
path = ..
standard = c++17

flags = -std=$(standard)  # compile with standard
flags += -Wall  # compile with all warnings
flags += -pedantic  # even more warnings
flags += -I $(path)  # add parent to preprocessor include path
flags += -g  # include gdb instrumentation

linker_flags = -pthread

link = $(cc) $(linker_flags) -o  # final linked build to binary executable

compile = $(cc) $(flags) -c -o  # compilation to intermediary .o files

all: thread thread-generator race-condition lamports-algo

# executables
#
thread : thread.o
	$(link) $@ $^
#
thread-generator : thread_generator.o
	$(link) $@ $^
#
race-condition : race_condition.o
	$(link) $@ $^
#
lamports-algo : lamports_algo.o
	$(link) $@ $^

# utils
#
clean:
	$(RM) *.o thread thread-generator race-condition lamports-algo
