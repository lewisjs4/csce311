// Copyright 2024 CSCE 311
//
// This file defines the DomainSocket base class, along with its subclasses
// DomainSocketServer and DomainSocketClient. These classes provide an
// abstraction for Unix domain sockets, facilitating interprocess communication.
//
#ifndef IPC_DOMAIN_SOCKET_H_
#define IPC_DOMAIN_SOCKET_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <functional>

// Base class for Unix domain sockets (shared functionality for both clients and servers)
class DomainSocket {
public:
    static const char kEoT = '\004';  // End of transmission character
    static const char kUS = '\037';   // Unit separator

    explicit DomainSocket(const char* socket_path, bool abstract = true);
    virtual ~DomainSocket();

    bool Init(int domain = AF_UNIX, int type = SOCK_STREAM, int protocol = 0);
    void Close(int socket_fd = 0) const;

    ::ssize_t Read(std::string* buffer,
                   int socket_file_descriptor = 0,  // 0 uses member socket_fd_
                   std::size_t return_after_bytes = 0,
                   char end_of_transmission = DomainSocket::kEoT) const;

protected:
  ::ssize_t Read(int socket_fd, char buffer[], std::size_t buffer_size) const;

 ::ssize_t Write(const std::string& bytes, int socket_fd = 0,
                 char eot = kEoT) const;

 protected:
  int socket_fd_;        // File descriptor for the socket
  ::sockaddr_un sock_addr_;  // Unix socket address structure
  std::string socket_path_;
};

// Server subclass (handles binding, listening, and accepting connections)
class DomainSocketServer : public DomainSocket {
public:
  using DomainSocket::DomainSocket;  // Inherit constructor

  bool Bind();
  bool Listen(std::size_t max_connections = 1);
  bool Accept(int* client_request_socket_fd,
              std::function<void(int)> on_connection = nullptr);
};

// Client subclass (handles connecting to a server socket)
class DomainSocketClient : public DomainSocket {
public:
  using DomainSocket::DomainSocket;  // Inherit constructor
  bool Connect();
};

#endif  // IPC_DOMAIN_SOCKET_H_
