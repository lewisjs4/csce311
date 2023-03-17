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
fork-pipe : fork_pipe.o
	$(link) $@ $^
#
fifo-manager: fifo_manager.o
	$(link) $@ $^
#
socket-client: domain_socket.o domain_socket_client.o
	$(link) $@ $^
#
socket-server: domain_socket.o domain_socket_server.o
	$(link) $@ $^

# objects
#
fork_pipe.o : fork_pipe.cc
	$(compile) $@ $<
#
fifo_manager.o : fifo_manager.cc
	$(compile) $@ $<
#
domain_socket_client.o : domain_socket_client.cc domain_socket_client.h  \
                         domain_socket.h
	$(compile) $@ $<
#
domain_socket_server.o : domain_socket_server.cc domain_socket_server.h  \
                         domain_socket.h
	$(compile) $@ $<
#
domain_socket.o : domain_socket.cc domain_socket.h
	$(compile) $@ $<

clean:
	$(RM) *.o

