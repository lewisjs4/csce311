// Copyright 2022 CSCE 311
//

#include <shm_logger/inc/producer.h>

#include <cstddef>
#include <iostream>

int main(int argc, char* argv[]) {
  assert(argc == 3 && "producer <shared_mem_name> <log_mutex_name>");

  logger::Producer log_writer(argv[1], argv[2]);

  std::cout << "Sending: > ";

  const size_t kBuffer_size = 64;
  char buffer[kBuffer_size];
  std::cin.getline(buffer, kBuffer_size);
  buffer[std::cin.gcount() + 1] = '\n';
  std::string msg(buffer);
  std::cout << "Bytes read: " << std::cin.gcount() << std::endl;
  while (std::cin.gcount() == kBuffer_size - 1) {
    std::cin.clear();
    std::cin.getline(buffer, kBuffer_size);
    std::cout << "Bytes read: " << std::cin.gcount() << std::endl;
    buffer[std::cin.gcount() + 1] = '\0';
    msg += buffer;
  }
  std::cout << "Sent: " << msg << std::endl;
  log_writer.Produce(std::string(msg));

  return 0;
}
