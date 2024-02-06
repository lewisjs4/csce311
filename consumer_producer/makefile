# Copyright 2024 CSCE 311

cc = g++  # use g++ compiler
path = ..
standard = c++2a

flags = -std=$(standard)  # compile with standard
flags += -Wall  # compile with all warnings
flags += -pedantic  # even more warnings
flags += -I $(path)  # add parent to preprocessor include path
flags += -g  # include gdb instrumentation

linker_flags = -pthread

link = $(cc) $(linker_flags) -o  # final linked build to binary executable

compile = $(cc) $(flags) -c -o  # compilation to intermediary .o files

# executable
#
consumer-producer : consumer_producer.o thread_sem_manager.o
	$(link) $@ $^

# objects
#
consumer_producer.o : consumer_producer.cc thread_sem_manager.h thread.h
	$(compile) $@ $<
#
thread_sem_manager.o : thread_sem_manager.cc thread_sem_manager.h
	$(compile) $@ $<


# utils
#
clean:
	$(RM) *.o consumer-producer
