# Copyright 2023 LittleCube

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

VPATH = $(source)  # tell make where to look for .c/.cc files

link = $(cc) $(flags) -o  # final linked build to binary executable
compile = $(cc) $(flags) -c -o # compilation to intermediary .o files

.PHONY : clean

all : sem_count sem_count_critical sem_count_multiplex sem_count_broken sem_count_parallel

# executables
#
sem_count : sem_count.o semaphore_wrapper.o
	$(link) $@ $(filter %.o, $^)

sem_count_critical : sem_count_critical.o semaphore_wrapper.o
	$(link) $@ $(filter %.o, $^)

sem_count_multiplex : sem_count_multiplex.o semaphore_wrapper.o
	$(link) $@ $(filter %.o, $^)

sem_count_broken : sem_count_broken.o semaphore_wrapper.o
	$(link) $@ $(filter %.o, $^)

sem_count_parallel : sem_count_parallel.o semaphore_wrapper.o
	$(link) $@ $(filter %.o, $^)

# objects
#
%.o: %.cc %.h
	$(linter) --root=.. $^
	$(compile) $@ $<

%.o: %.cc
	$(linter) --root=.. $^
	$(compile) $@ $<

# clean rule
#
clean:
	$(RM) *.o sem_count sem_count_critical sem_count_multiplex sem_count_broken sem_count_parallel
