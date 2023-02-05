# Copyright 2023 CSCE 311

cc = g++  # use g++ compiler
linter = cpplint  # use Google style guidelines
path = ..
source = src
include = inc
standard = c++17

flags = -std=$(standard)  # compile with standard
flags += -Wall  # compile with all warnings
flags += -pedantic  # even more warnings
flags += -I $(path)  # add parent to preprocessor include path
flags += -I $(include)  # add include directory to preprocessor include path
flags += -g  # include gdb instrumentation
flags += -pthread  # include thread library

VPATH = $(source)  # make sure makefile knows where to look for sources

link = $(cc) $(flags) -o  # final linked build to binary executable
compile = $(cc) $(flags) -c -o # compilation to intermediary .o files

# object lists
sem_count_objs = sem_count.o cubiphore.o
sem_count_broken_objs = sem_count_broken.o cubiphore.o
sem_count_parallel_objs = sem_count_parallel.o cubiphore.o

.PHONY : all lint clean

all : sem_count sem_count_broken sem_count_parallel

# executables
#
sem_count : lint $(sem_count_objs)
	$(link) $@ $(sem_count_objs)

sem_count_broken : lint $(sem_count_broken_objs)
	$(link) $@ $(sem_count_broken_objs)

sem_count_parallel : lint $(sem_count_parallel_objs)
	$(link) $@ $(sem_count_parallel_objs)

# objects
#
sem_count.o : sem_count.cc
	$(compile) $@ $<

sem_count_broken.o : sem_count_broken.cc
	$(compile) $@ $<

sem_count_parallel.o : sem_count_parallel.cc
	$(compile) $@ $<

cubiphore.o : cubiphore.cc
	$(compile) $@ $<

# lint rule
#
lint:
	$(linter) --root=.. $(source)/* $(include)/*

# clean rule
#
clean:
	$(RM) *.o sem_count sem_count_broken sem_count_parallel
