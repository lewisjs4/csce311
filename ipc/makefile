# Copyright 2023 CSCE 311

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


all = fork-pipe fifo-manager domain-socket-client domain-socket-server


# executables
#
fork-pipe : fork_pipe.o
	$(link) $@ $^
#
fifo-manager : fifo_manager.o
	$(link) $@ $^
#
domain-socket-client : domain_socket_client.o domain_socket.o
	$(link) $@ $^
#
domain-socket-server : domain_socket_server.o domain_socket.o
	$(link) $@ $^


# objects
#
domain_socket_client.o : domain_socket_client.cc domain_socket_client.h \
                         domain_socket.h
	$(compile) $@ $<
#
domain_socket_server.o : domain_socket_server.cc domain_socket_server.h \
                         domain_socket.h
	$(compile) $@ $<
#
domain_socket.o : domain_socket.cc domain_socket.h
	$(compile) $@ $<

clean:
	$(RM) *.o $(all)
