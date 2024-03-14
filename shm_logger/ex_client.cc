// Copyright 2024 CSCE 311
//

#include <shm_logger/producer.h>

#include <cstddef>
#include <iostream>

int main(int argc, char* argv[]) {
  const std::size_t kArgC = 3,
                    kSharedMemIndex = 1,
                    kMutexIndex = 2;

  if (argc != kArgC) {
    std::cerr << argv[0] << " <shared_mem_name> <log_mutex_name>" << std::endl;
    return 1;
  }

  logger::Producer log_writer(argv[kSharedMemIndex], argv[kMutexIndex]);

  std::cout << "Sending: > ";

  const int64_t kBufferSize = 64;
  char buffer[kBufferSize];
  std::string msg;

  std::cin.getline(buffer, kBufferSize);
  std::cout << "Bytes read: " << std::cin.gcount() << std::endl;
  while (std::cin.gcount() + 1 == kBufferSize) {
    msg += std::string(buffer, std::cin.gcount());

    std::cin.clear();
    std::cin.getline(buffer, kBufferSize);
    std::cout << "Bytes read: " << std::cin.gcount() << std::endl;
  }
  msg += std::string(buffer, std::cin.gcount());

  std::cout << "Sent: " << msg << std::endl;
  log_writer.Produce(std::string(msg));

  return 0;
}
