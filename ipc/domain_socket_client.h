// Copyright 2024 CSCE 311
//
#ifndef IPC_DOMAIN_SOCKET_CLIENT_H_
#define IPC_DOMAIN_SOCKET_CLIENT_H_

#include <ipc/domain_socket.h>

#include <cstddef>  // using size_t
#include <cstdlib>  // exit

#include <string>
#include <iostream>


const char kSocket_path[] = "socket_example";


///
/// Domain Socket Client C++ Interface Class
///
class DomainSocketClient : public DomainSocket {
 public:
  using DomainSocket::DomainSocket;

  void Run();
};


#endif  // IPC_DOMAIN_SOCKET_CLIENT_H_
