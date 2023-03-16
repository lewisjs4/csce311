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

# executable
#
generator: generator.o producer.o named_semaphore.o
	$(link) $@ $^ -lrt
#
writer: writer.o consumer.o named_semaphore.o
	$(link) $@ $^ -lrt

# objects
#
generator.o : generator.cc producer.h
	$(compile) $@ $<
#
writer.o : writer.cc consumer.h
	$(compile) $@ $<
#
consumer.o : consumer.cc consumer.h
	$(compile) $@ $<
#
producer.o : producer.cc producer.h
	$(compile) $@ $<
#
named_semaphore.o : named_semaphore.cc named_semaphore.h
	$(compile) $@ $<


# utils
#
clean:
	$(RM) *.o
