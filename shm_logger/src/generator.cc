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

  log_writer.Produce(std::string(buffer));

  return 0;
}
