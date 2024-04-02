// Copyright 2024 CSCE 311
//

#include <pg_cache_logger/producer.h>

#include <cstddef>
#include <iostream>

int main(/* int argc, char* argv[] */) {
  logger::Producer log_writer;

  std::cout << "Sending: > ";

  const size_t kBuffer_size = 32;
  char buffer[kBuffer_size];

  std::cin.getline(buffer, kBuffer_size);
  std::string msg(buffer);

  while (std::cin.fail()) {  // did not get to \n in getline
    std::cin.clear();  // reset fail bit

    std::cin.getline(buffer, kBuffer_size);

    msg += std::string(buffer);
  }

  std::cout << "Sent: " << msg << std::endl;
  log_writer.Produce(msg);

  return 0;
}
