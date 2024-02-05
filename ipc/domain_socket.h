// Copyright 2024 CSCE 311
//
#ifndef IPC_DOMAIN_SOCK_H_
#define IPC_DOMAIN_SOCK_H_

#include <sys/socket.h>  // Unix header for sockets, using socket
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <unistd.h>  // Unix standard header, using close

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // exit
#include <cstring>  // using strncpy, strerror

#include <string>
#include <iostream>


class DomainSocket {
 public:
  ///
  /// Initializes the socket address struct (::sockaddr_un)
  ///
  /// @param socket_path A null-terminated C-string containing the shared path
  ///                    name for socket.
  /// @param abstract Defaults to abstract socket path names, pass false for
  ///                 non-abstract naming.
  ///
  static const char kEoT = static_cast<char>(4);

  explicit DomainSocket(const char *socket_path, bool abstract = true);

  bool Init(int domain=AF_UNIX, int type=SOCK_STREAM, int protocol=0);

  bool Bind();

  bool Connect();

  bool Listen(std::size_t max_connections = 1);

  ::ssize_t Read(std::string* buffer,
                int socket_file_descriptor,
                char end_of_transmission = DomainSocket::kEoT);


  ::ssize_t Write(const std::string& bytes,
                  char end_of_transmission = DomainSocket::kEoT);

 protected:
  int socket_fd_;  // file descriptor for socket this class wraps
  ::sockaddr_un sock_addr_;  // socket address from sys/un.h

  std::string socket_path_;  // std::string stores socket_path (no raw pointers)
};

#endif  // IPC_DOMAIN_SOCK_H_
