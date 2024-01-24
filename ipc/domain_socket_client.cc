// Copyright 2024 CSCE 311
//

#include <ipc/domain_socket_client.h>


///
/// Domain Socket Client C++ Interface Class
///
void DomainSocketClient::Run() {
  // (1) open nameless Unix socket
  std::cout << "DomainSocketClient initializing..." << std::endl;
  if(!Init())
    exit(1);

  // (2) connect to an existing socket
  std::cout << "DomainSocketClient connecting..." << std::endl;
  if (!Connect())
    exit(2);

  // (3) write to socket
  while (true) {
    std::string input;
    std::getline(std::cin, input);
    ::ssize_t bytes_wrote = Write(input);
    if (bytes_wrote < 0) {
      std::cerr << "DomainSocketClient terminating..." << std::endl;

      exit(3);
    } else if(bytes_wrote == 0) {
      std::cerr << "Server disconnected" << std::endl;
      exit(4);
    }
  }
}


int main(int argc, char *argv[]) {
  DomainSocketClient dsc(kSocket_path);
  dsc.Run();

  return 0;
}


