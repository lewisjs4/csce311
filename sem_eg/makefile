# Copyright 2025 CSCE 311

cc = g++  # use g++ compiler
path = ..
standard = c++20

flags = -std=$(standard)  # compile with standard
flags += -Wall  # compile with all warnings
flags += -pedantic  # even more warnings
flags += -I $(path)  # add parent to preprocessor include path
flags += -g  # include gdb instrumentation

linker_flags = -pthread

link = $(cc) $(linker_flags) -o  # final linked build to binary executable

compile = $(cc) $(flags) -c -o  # compilation to intermediary .o files

executables = barrier monitor monitor-cond-var multiplex mutex producer-consumer readers-writers signal 

all : $(executables)
	@echo "All Executables Built"

# executables
#
barrier : barrier.o thread_sem_manager.o
	$(link) $@ $^
#
monitor : monitor.o thread_sem_manager.o
	$(link) $@ $^
#
monitor-cond-var : monitor_cond_var.o thread_sem_manager.o
	$(link) $@ $^
#
multiplex : multiplex.o thread_sem_manager.o
	$(link) $@ $^
#
mutex : mutex.o thread_sem_manager.o
	$(link) $@ $^
#
producer-consumer : producer_consumer.o thread_sem_manager.o
	$(link) $@ $^
#
readers-writers: readers_writers.o thread_sem_manager.o
	$(link) $@ $^
#
signal : signal.o thread_sem_manager.o
	$(link) $@ $^

# objects
#
barrier.o : barrier.cc thread_sem_manager.h thread.h
	$(compile) $@ $<
#
monitor.o : monitor.cc thread_sem_manager.h
	$(compile) $@ $<
#
monitor_cond_var.o : monitor_cond_var.cc
	$(compile) $@ $<
#
multiplex.o : multiplex.cc thread_sem_manager.h
	$(compile) $@ $<
#
mutex.o : mutex.cc thread_sem_manager.h
	$(compile) $@ $<
#
producer_consumer.o : producer_consumer.cc thread_sem_manager.h thread.h
	$(compile) $@ $<
#
readers_writers.o : readers_writers.cc thread_sem_manager.h thread.h
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
	$(RM) *.o $(executables)
