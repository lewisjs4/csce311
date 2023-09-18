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


all: fork-pipe fifo-manager socket-cli-srv

# executables
#
fork-pipe : fork_pipe.o
	$(link) $@ $^
#
fifo-manager : fifo_manager.o
	$(link) $@ $^
#
socket-cli-srv : socket_cli_srv.o
	$(link) $@ $^

clean:
	$(RM) *.o fork-pipe fifo-manager socket-cli-srv
