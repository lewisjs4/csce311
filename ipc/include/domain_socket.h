// Copyright 2025 CSCE 311
//
// This file defines the UnixDomainSocket base class, along with its subclasses
// DomainSocketServer and DomainSocketClient. These classes provide an
// abstraction for Unix domain sockets, facilitating interprocess communication.
//
#ifndef IPC_DOMAIN_SOCKET_H_
#define IPC_DOMAIN_SOCKET_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <string>

//
// Base class for Unix domain sockets (shared functionality for both clients and servers)
//
class UnixDomainSocket {
 public:
  explicit UnixDomainSocket(const char* socket_path, bool abstract = true);

  virtual ~UnixDomainSocket();

 protected:
  // Servers must call this when finished with client socket. Both Clients and
  // Servers use desctructors to automatically call this on their own socket (RAII).
  void Close(int socket_fd) const;

  // //
  // You may ignore most, if not all of the rest of this class. Skip to the
  // Server and Client subclasses, which follow.
  // //

  // Begins construction of Unix domain socket, server or client should finish
  bool Init();

  // Read until end of transmission character and store in buffer
  ::ssize_t Read(int socket_file_descriptor,
                 char end_of_transmission,
                 std::string* buffer) const;

  // Read set number of bytes and store in buffer
  ::ssize_t Read(int socket_file_descriptor,
                 std::size_t return_after_bytes,
                 std::string* buffer) const;

  // Write message and then send end of transmission character
  ::ssize_t Write(int socket_file_descriptor,
                  const std::string& message,
                  char eot) const;

  int socket_fd_;        // server or client's socket file descriptor
  std::string socket_path_;  // name of socket
  ::sockaddr_un sock_addr_;  // Unix socket address structure

 private:
  ::ssize_t Read(int socket_fd, char buffer[], std::size_t buffer_size) const;
};


//
// Server subclass (handles binding, listening, and accepting connections)
//
class DomainSocketServer : public UnixDomainSocket {
 public:
  explicit DomainSocketServer(const char* socket_path,
                              char us, char eot, bool abstract = true)
      : UnixDomainSocket(socket_path, abstract), us_(us), eot_(eot) {
    // empty
  }

  // Create, initialize, and begin listening to socket
  bool Init(std::size_t max_connections);

  // Call after Init to get a client file descriptor when client connects. This
  // is a blocking call.
  int Accept();  

  // pass the file descriptor from Accept to read a client's message
  ::ssize_t Read(int socket_file_descriptor, std::string* message) const;

  // pass the first descriptor from Accept to write message to a client
  ::ssize_t Write(int socket_file_descriptor, const std::string& message) const;

 protected:
  char us_;
  char eot_;
};


//
// Client subclass (handles connecting to a server socket)
//
class DomainSocketClient : public UnixDomainSocket {
 public:
   using UnixDomainSocket::UnixDomainSocket;

  // Connect to an existing Unix domain socket
  bool Init();

  // Read the given number of bytes and store in buffer
  ::ssize_t Read(std::size_t return_after_bytes, std::string* buffer) const;

  // Read until end of transmission character and store in buffer
  ::ssize_t Read(char eot, std::string* buffer) const;

  // Write message and send end of transmission character
  ::ssize_t Write(const std::string& message, char eot) const;
};

#endif  // IPC_DOMAIN_SOCKET_H_
