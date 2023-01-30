// Copyright 2023 CSCE 311
//

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


class UnixDomainSocket {
 public:
  ///
  /// Initializes the socket address struct (::sockaddr_un)
  ///
  /// @param socket_path A null-terminated C-string containing the shared path
  ///                    name for socket.
  /// @param abstract Defaults to abstract socket path names, pass false for
  ///                 non-abstract naming.
  /// 
  explicit UnixDomainSocket(const char *socket_path, bool abstract = true) {
    socket_path_ = std::string(socket_path);  // std::string manages char *

    sock_addr_ = {};  // init struct (replaces memset from C)
    sock_addr_.sun_family = AF_UNIX;  // set to Unix domain socket (e.g. instead
                                      //   of internet domain socket)
    if (abstract)                                  
      // leaving leading null char sets abstract socket
      strncpy(sock_addr_.sun_path + 1,           // strncpy to limit copy for
              socket_path,                       //   portability
              sizeof(sock_addr_.sun_path) - 1);  // -2 for leading/trailing \0s
    else
      // copy string from socket path without leading \0
      strncpy(sock_addr_.sun_path,               // strncpy to limit copy for
              socket_path,                       //   portability
              sizeof(sock_addr_.sun_path));  //
  }

 protected:
  ::sockaddr_un sock_addr_;  // socket address from sys/un.h

  std::string socket_path_;  // std::string stores socket_path (no raw pointers)
};


///
/// Domain Socket Server C++ Interface Class
///
class DomainSocketServer : public UnixDomainSocket {
 public:
  using ::UnixDomainSocket::UnixDomainSocket;

  void RunServer() const {
    int sock_fd;  // unnamed socket file descriptor
    int client_req_sock_fd;  // client connect request socket file descriptor

    // (1) create a socket
    //       AF_UNIX -> file system pathnames
    //       SOCK_STREAM -> sequenced bytestream
    //       0 -> default protocol (let OS decide correct protocol)
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if ( sock_fd < 0 ) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    // (2) bind socket to address for the server
    unlink(socket_path_.c_str());  // sys call to delete file if it already
                                   //   exists (using Unix system calls for
                                   //   sockets, no reason to be non-Unix
                                   //   portable now).  :-/
    int success = bind(sock_fd,
                       // sockaddr_un is a Unix sockaddr and so may be cast "up"
                       //   to that pointer type (think of it as C polymorphism)
                       reinterpret_cast<const sockaddr*>(&sock_addr_),
                       // size needs be known due to underlying data layout,
                       //   i.e., there may be a size difference between parent
                       //   and child
                       sizeof(sock_addr_));

    // log errors
    if (success < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    // (3) Listen for connections from clients
    size_t kMax_client_conns = 5;
    success = listen(sock_fd, kMax_client_conns);
    if (success < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    const size_t kRead_buffer_size = 32;  // read 4 byte increaments
    char read_buffer[kRead_buffer_size];
    int bytes_read;
    while (true) {
      // (4) Accept connection from a client
      client_req_sock_fd = accept(sock_fd, nullptr, nullptr);
      if (client_req_sock_fd  < 0) {
        std::cerr << strerror(errno) << std::endl;
        continue;
      }

      std::cout << "Client connected" << std::endl;

      // (5) Receive data from client(s)
      bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
      const char kKill_msg[] = "quit";  // TODO(lewisjs): trim whitespace
                                        //   from read_buffer for comparison
      while (bytes_read > 0) {
        if (strcmp(read_buffer, kKill_msg) == 0) {
          std::cout << "Server shutting down..." << std::endl;

          bytes_read = 0;  // message handled, disconnect client
          exit(0);
        }

        std::cout << "read " << bytes_read << " bytes: ";
        std::cout.write(read_buffer, bytes_read) << std::endl;

        bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
      }

      if (bytes_read == 0) {
        std::cout << "Client disconnected" << std::endl;

        close(client_req_sock_fd);
      } else if (bytes_read < 0) {
        std::cerr << strerror(errno) << std::endl;

        exit(-1);
      }
    }
  }
};


///
/// Domain Socket Client C++ Interface Class
///
class DomainSocketClient : public UnixDomainSocket {
 public:
  using UnixDomainSocket::UnixDomainSocket;

  void RunClient() {
    // (1) open nameless Unix socket
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    // (2) connect to an existing socket
    int success = connect(socket_fd,
                          // sockaddr_un is a Unix sockaddr
                          reinterpret_cast<const sockaddr*>(&sock_addr_),
                          sizeof(sock_addr_));
    if (success < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    // (3) write to socket
    const ssize_t kWrite_buffer_size = 64;
    char write_buffer[kWrite_buffer_size];
    while (true) {
      std::cin.getline(write_buffer, kWrite_buffer_size);
      while (std::cin.gcount() > 0) {
        if (std::cin.gcount() == kWrite_buffer_size - 1 && std::cin.fail())
          std::cin.clear();
        // write() is equivalent to send() with no flags in send's 3rd param
        ssize_t bytes_wrote = write(socket_fd, write_buffer, std::cin.gcount());
        std::cout << "sent " << std::cin.gcount() << " bytes" << std::endl;
        if (bytes_wrote < 0) {
          std::cerr << strerror(errno) << std::endl;

          exit(-1);
        }

        if (bytes_wrote == 0) {
          std::clog << "Server dropped connection!" << std::endl;
          exit(-2);
        }

        std::cin.getline(write_buffer, kWrite_buffer_size);
      }
    }
  }
};


const char kSocket_path[] = "socket_cli_srv_domain_socket";

int main(int argc, char *argv[]) {
  if (argc != 2)
    return 1;

  if (strcmp(argv[1], "server") == 0) {
    DomainSocketServer dss(kSocket_path);
    dss.RunServer();
  } else if (strcmp(argv[1], "client") == 0) {
    DomainSocketClient dsc(kSocket_path);
    dsc.RunClient();
  }

  return 0;
}


