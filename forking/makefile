# Copyright 2023 CSCE 311

cc = g++  # use g++ compiler
path = ..
standard = c++17

flags = -std=$(standard)  # compile with standard
flags += -Wall  # compile with all warnings
flags += -pedantic  # even more warnings
flags += -I $(path)  # add parent to preprocessor include path
flags += -g  # include gdb instrumentation

link = $(cc) $(flags) -o  # final linked build to binary executable

compile = $(cc) $(flags) -c -o  # compilation to intermediary .o files

# executables
#
fork : fork.o
	$(link) $@ $^
#
fork-ls : fork_ls.o
	$(link) $@ $^
#
fork_pipe : fork_pipe.o
	$(link) $@ $^

# objects
#
fork.o : fork.cc
	$(compile) $@ $<
#
fork_ls.o : fork_ls.cc
	$(compile) $@ $<
#
fork_pipe.o : fork_pipe.cc
	$(compile) $@ $<

clean:
	$(RM) *.o
