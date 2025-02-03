// Copyright 2024 CSCE 311
//
#ifndef IPC_DOMAIN_SOCK_SERVER_H_
#define IPC_DOMAIN_SOCK_SERVER_H_

#include <ipc/domain_socket.h>

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

const char kSocket_path[] = "socket_example";

///
/// Domain Socket Server C++ Interface Class
///
class DomainSocketServer : public DomainSocket {
 public:
  using ::DomainSocket::DomainSocket;

  void Run();
};

#endif  // IPC_DOMAIN_SOCK_SERVER_H_
