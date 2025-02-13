// Copyright 2024 CSCE 311
//

#include <domain_socket.h>
#include <cstring>

// DomainSocket constructor
DomainSocket::DomainSocket(const char* socket_path, bool abstract) {
  socket_fd_ = 0;
  socket_path_ = std::string(socket_path);
  sock_addr_ = {};  // equivalent to memset(0)
  sock_addr_.sun_family = AF_UNIX;
  if (abstract) {
      strncpy(sock_addr_.sun_path + 1, socket_path, sizeof(sock_addr_.sun_path) - 1);
  } else {
      strncpy(sock_addr_.sun_path, socket_path, sizeof(sock_addr_.sun_path));
  }
}

DomainSocket::~DomainSocket() {
  Close();
}

bool DomainSocket::Init(int domain, int type, int protocol) {
  socket_fd_ = ::socket(domain, type, protocol);
  return socket_fd_ > 0;
}

void DomainSocket::Close(int socket_fd) const {
  if (!socket_fd)
    socket_fd = socket_fd_;

  if (socket_fd > 0)
      ::close(socket_fd);
}

::ssize_t DomainSocket::Read(std::string* output,
                             int socket_fd,
                             std::size_t byte_count,
                             char eot) const {
  if (!socket_fd)
      socket_fd = socket_fd_;

  const size_t kBufferSize = 64;
  char buffer[kBufferSize];

  ::ssize_t bytes_read = Read(socket_fd, buffer, byte_count ? byte_count : kBufferSize);
  if (bytes_read <= 0)
      return bytes_read;

  ::ssize_t total_bytes_read = bytes_read;

  while (buffer[bytes_read - 1] != eot
         || (byte_count && static_cast<std::size_t>(bytes_read) < byte_count)) {
      output->insert(output->size(), buffer, bytes_read);

      bytes_read = Read(socket_fd, buffer,
          byte_count ? byte_count - total_bytes_read : kBufferSize);
      if (bytes_read <= 0)
          return total_bytes_read;
      total_bytes_read += bytes_read;
  }

  // If we are reading to EOT, omit the final EOT char
  output->insert(output->size(), buffer, byte_count ? bytes_read : bytes_read - 1);

  return total_bytes_read;
}

::ssize_t DomainSocket::Read(int socket_fd, char buffer[], std::size_t buffer_size) const {
  ::ssize_t bytes_read = ::read(socket_fd, buffer, buffer_size);

  if (bytes_read == 0) {
      std::cout << "Writer disconnected" << std::endl;
  } else if (bytes_read < 0) {
      std::cerr << "Read Error: " << ::strerror(errno) << std::endl;
  }

  return bytes_read;
}


::ssize_t DomainSocket::Write(const std::string& bytes, int socket_fd, char eot) const {
  if (!socket_fd) socket_fd = socket_fd_;
  if (socket_fd <= 0) return -1;
  ::ssize_t bytes_written = ::write(socket_fd, bytes.c_str(), bytes.size());
  ::write(socket_fd, &eot, 1);
  return bytes_written + 1;
}

// Server methods
bool DomainSocketServer::Bind() {
  unlink(socket_path_.c_str());
  return ::bind(socket_fd_, reinterpret_cast<const sockaddr*>(&sock_addr_), sizeof(sock_addr_))
    == 0;
}

bool DomainSocketServer::Listen(std::size_t max_connections) {
  return ::listen(socket_fd_, max_connections) == 0;
}

bool DomainSocketServer::Accept(int* client_request_socket_fd,
      std::function<void(int)> on_connection) {
  *client_request_socket_fd = ::accept(socket_fd_, nullptr, nullptr);
  if (*client_request_socket_fd > 0 && on_connection) {
      on_connection(*client_request_socket_fd);
  }
  return *client_request_socket_fd > 0;
}

// Client methods
bool DomainSocketClient::Connect() {
  return ::connect(socket_fd_, reinterpret_cast<const sockaddr*>(&sock_addr_),
      sizeof(sock_addr_)) == 0;
}

