// Copyright 2025 CSCE 311
//

#include <domain_socket.h>
#include <cstring>

// DomainSocket constructor
UnixDomainSocket::UnixDomainSocket(const char* socket_path, bool abstract)
    : socket_fd_(0), socket_path_(socket_path) {
  sock_addr_ = {};  // equivalent to memset(0)
  sock_addr_.sun_family = AF_UNIX;
  if (abstract) {
      strncpy(sock_addr_.sun_path + 1, socket_path, sizeof(sock_addr_.sun_path) - 1);
  } else {
      strncpy(sock_addr_.sun_path, socket_path, sizeof(sock_addr_.sun_path));
  }
}


// DomainSocket destructor
UnixDomainSocket::~UnixDomainSocket() {
  Close(socket_fd_);
}

bool UnixDomainSocket::Init() {
  // (1) create a socket
  socket_fd_ = ::socket(AF_UNIX, SOCK_STREAM, 0);

  if (socket_fd_ < 1)
      std::cerr << "Socket Creation Error: " << ::strerror(errno) << std::endl;

  return socket_fd_ > 0;
}

void UnixDomainSocket::Close(int socket_fd) const {
  ::close(socket_fd);
}

::ssize_t UnixDomainSocket::Read(int socket_fd,
                                 char eot,
                                 std::string* output) const {
  const size_t kBufferSize = 64;
  char buffer[kBufferSize];

  ::ssize_t bytes_read = Read(socket_fd, buffer, kBufferSize);
  if (bytes_read <= 0)
      return bytes_read;

  ::ssize_t total_bytes_read = bytes_read;

  while (buffer[bytes_read - 1] != eot) {
      output->insert(output->size(), buffer, bytes_read);

      bytes_read = Read(socket_fd, buffer, kBufferSize);
      if (bytes_read <= 0)
          return total_bytes_read;
      total_bytes_read += bytes_read;
  }

  // If we are reading to EOT, omit the final EOT char
  output->insert(output->size(), buffer, bytes_read - 1);

  return total_bytes_read;
}

::ssize_t UnixDomainSocket::Read(int socket_fd,
                                 std::size_t byte_count,
                                 std::string* output) const {
  const size_t kBufferSize = 64;
  char buffer[kBufferSize];

  ::ssize_t bytes_read = Read(socket_fd,
                              buffer,
                              std::min(kBufferSize, byte_count));
  if (bytes_read <= 0)
      return bytes_read;

  ::ssize_t total_bytes_read = bytes_read;

  while (static_cast<std::size_t>(bytes_read) < byte_count) {
      output->append(buffer, bytes_read);

      bytes_read = Read(socket_fd,
                        buffer,
                        std::min(byte_count - total_bytes_read, kBufferSize));
      if (bytes_read <= 0)
          return total_bytes_read;
      total_bytes_read += bytes_read;
  }

  // If we are reading to EOT, omit the final EOT char
  output->insert(output->size(), buffer, byte_count ? bytes_read : bytes_read - 1);

  return total_bytes_read;
}

::ssize_t UnixDomainSocket::Read(int socket_fd,
                                 char buffer[],
                                 std::size_t buffer_size) const {
  ::ssize_t bytes_read = ::read(socket_fd, buffer, buffer_size);

  if (bytes_read == 0) {
      std::cout << "Writer disconnected" << std::endl;
  } else if (bytes_read < 0) {
      std::cerr << "Read Error: " << ::strerror(errno) << std::endl;
  }

  return bytes_read;
}


::ssize_t UnixDomainSocket::Write(int socket_fd,
                                  const std::string& bytes,
                                  char eot) const {
  ::ssize_t bytes_written = ::write(socket_fd, bytes.c_str(), bytes.size());
  if (bytes_written < 0 && errno == EPIPE) {
    // NOTE you may recieve SIGPIPE, which, if ignored, terminates
    // your app. Typically, you do not register a signal handler for
    // SIGPIPE, you just ignore:
    //   signal(SIGPIPE, SIG_IGN);
    // server dropped connection with client still writing
    std::cerr << strerror(errno) << std::endl;
    return bytes_written;
  }

  if (::write(socket_fd, &eot, 1) < 0 && errno == EPIPE) {  // send eot char
    std::cerr << strerror(errno) << std::endl;
    return -1;
  }

  return bytes_written + 1;
}


//
// Server methods
//
bool DomainSocketServer::Init(std::size_t max_connections) {
  // (1) create a socket
  std::cout << "DomainSocketServer initializing..." << std::endl;
  if (!UnixDomainSocket::Init())
    return false;

  // (2) bind socket to address for the server
  unlink(socket_path_.c_str());
  std::cout << "DomainSocketServer binding socket to address..." << std::endl;
  if (::bind(socket_fd_,
             reinterpret_cast<const sockaddr*>(&sock_addr_),
             sizeof(sock_addr_)) != 0) {
    std::cerr << ::strerror(errno) << std::endl;
    return false;
  }

  // (3) Listen for connections from clients
  std::cout << "DomainSocketServer listening for client connections..."
    << std::endl;
  if (::listen(socket_fd_, max_connections) != 0) {
    std::cerr << ::strerror(errno) << std::endl;
    return false;
  }

  return true;
}

int DomainSocketServer::Accept() {
      int client_req_sock_fd = ::accept(socket_fd_, nullptr, nullptr);

      if (client_req_sock_fd < 0)
        std::cerr << "DomainSocketServer::Accept Error: " << strerror(errno)
          << std::endl;

      return client_req_sock_fd;
}


::ssize_t DomainSocketServer::Read(int socket_fd, std::string* buffer) const {
  return UnixDomainSocket::Read(socket_fd, eot_, buffer);
}


::ssize_t DomainSocketServer::Write(int socket_fd,
                                    const std::string& bytes) const {
  return UnixDomainSocket::Write(socket_fd, bytes, eot_);
}


//
// Client methods
//
bool DomainSocketClient::Init() {
  if (!UnixDomainSocket::Init())
    return false;

  if (::connect(socket_fd_,
                reinterpret_cast<const sockaddr*>(&sock_addr_),
                sizeof(sock_addr_)) >= 0)
    return true;

  std::cerr << "Socket Init Error: " << strerror(errno) << std::endl;
  return false;
}

::ssize_t DomainSocketClient::Read(std::size_t return_after_bytes, std::string* buffer) const {
  return UnixDomainSocket::Read(socket_fd_, return_after_bytes, buffer);
}

::ssize_t DomainSocketClient::Read(char eot, std::string* buffer) const {
  return UnixDomainSocket::Read(socket_fd_, eot, buffer);
}

::ssize_t DomainSocketClient::Write(const std::string& bytes, char eot) const {
  return UnixDomainSocket::Write(socket_fd_, bytes, eot);
}

