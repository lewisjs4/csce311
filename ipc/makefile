# Copyright 2021  CSCE240
cc = g++  # use g++ compiler

flags = -std=c++17  # compile with C++ 17 standard
flags += -Wall  # compile with all warnings
flags += -g  # produce debugging information
#flags += -I .. # add pwd directory to src include path
#flags += -pthread # support for POSIX threads

link = $(cc) $(flags) -o
compile = $(cc) $(flags) -c


# fork-pipe
#
fork-pipe: fork_pipe.cc
	$(link) $@ $^


# fifo-manager
#
fifo-manager: fifo_manager.cc
	$(link) $@ $^


# socket-cli-srv
#
socket-cli-srv: socket_cli_srv.cc
	$(link) $@ $^

# Clean up
clean:
	$(RM) fork-pipe fifo-manager socket-cli-srv
