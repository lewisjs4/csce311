# Copyright 2022  CSCE 311
#
cc = g++  # use g++ compiler

flags = -std=c++17  # compile with C++ 17 standard
flags += -Wall  # compile with all warnings
flags += -g  # produce debugging information
flags += -I .. # add pwd directory to src include path

linker_flags = $(flags) -pthread # support for POSIX threads

compile = $(cc) $(flags) -c

link = $(cc) $(linker_flags) -o


# sem-mutex 
#
sem-mutex: bin/sem_mutex.o
	$(link) $@ $^

bin/sem_mutex.o : src/sem_mutex.cc
	$(compile) $< -o $@

# sem-count
#
sem-count: bin/sem_count.o
	$(link) $@ $^

bin/sem_count.o : src/sem_count.cc
	$(compile) $< -o $@

# sem-order
#
sem-order: bin/sem_order.o
	$(link) $@ $^

bin/sem_order.o : src/sem_order.cc
	$(compile) $< -o $@

# Clean up
clean:
	$(RM) bin/* inc/*.gch sem-*
