// Copyright 2024 CSCE 311
//

#include <ipc/domain_socket_server.h>


void DomainSocketServer::Run() {
  int socket_fd;  // client connect request socket file descriptor

  // (1) create a socket
  std::cout << "DomainSocketServer initializing..." << std::endl;
  if (!Init())
    exit(-1);

  // (2) bind socket to address for the server
  std::cout << "DomainSocketServer binding socket to address..." << std::endl;
  if (!Bind())
    exit(-2);

  // (3) Listen for connections from clients
  std::cout << "DomainSocketServer listening for client connections..."
    << std::endl;
  if (!Listen())
    exit(-3);

  while (true) {
    // (4) Accept connection from a client
    socket_fd = ::accept(socket_fd_, nullptr, nullptr);
    if (socket_fd  < 0) {
      std::cerr << "Socket connection: " << ::strerror(errno) << std::endl;
      continue;
    }
    std::cout << "Client connected" << std::endl;

    while (true) {
      // (5) Receive data from client(s)
      std::string msg;
      ::ssize_t bytes_read = Read(&msg, socket_fd);

      if (bytes_read < 0) {
        std::cerr << "Server shutting down..." << std::endl;

        exit(0);
      } else if (!bytes_read) {
        std::cout << "Client disconnected" << std::endl;

        close(socket_fd);
        break;
      }

      std::cout << "read " << bytes_read << " bytes: ";
      std::cout << msg << std::endl;
    }
  }
}


int main(int argc, char *argv[]) {
  DomainSocketServer dss(kSocket_path);

  dss.Run();

  return 0;
}


