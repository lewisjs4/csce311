# Copyright 2024 CSCE 311

cc = g++  # use g++ compiler
path = ..
standard = c++2a

flags = -std=$(standard)  # compile with standard
flags += -Wall  # compile with all warnings
flags += -pedantic  # even more warnings
flags += -I $(path)  # add parent to preprocessor include path
flags += -g  # include gdb instrumentation

link = $(cc) $(flags) -o  # final linked build to binary executable

compile = $(cc) $(flags) -c -o  # compilation to intermediary .o files

all: fork fork-ls

# executables
#
fork : fork.o
	$(link) $@ $^
#
fork-ls : fork_ls.o
	$(link) $@ $^
#
fork-pipe : fork_pipe.o
	$(link) $@ $^

clean:
	$(RM) *.o fork fork-ls fork-pipe
