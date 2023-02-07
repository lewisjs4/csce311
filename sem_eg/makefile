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

# executables
#
multiplex : multiplex.o thread_sem_manager.o
	$(link) $@ $^
#
mutex : mutex.o thread_sem_manager.o
	$(link) $@ $^
#
signal : signal.o thread_sem_manager.o
	$(link) $@ $^

# objects
#
multiplex.o : multiplex.cc thread_sem_manager.h
	$(compile) $@ $<
#
mutex.o : mutex.cc thread_sem_manager.h
	$(compile) $@ $<
#
signal.o : signal.cc thread_sem_manager.h
	$(compile) $@ $<
#
thread_sem_manager.o : thread_sem_manager.cc thread_sem_manager.h
	$(compile) $@ $<


# utils
#
clean:
	$(RM) *.o
