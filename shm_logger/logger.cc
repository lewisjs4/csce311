// Copyright 2024 CSCE 311
//

#include <shm_logger/consumer.h>

#include <cstddef>
#include <iostream>


// Deletes kLogger memory (calls destructor)
void LoggerSigTermHandler(int sig);


logger::Consumer* log_writer;


int main(int argc, char* argv[]) {
  const int kArgCount = 4,
            kMemName = 1,
            kSigName = 2,
            kLogName = 3;

  if (argc != kArgCount) {
    std::cerr
    << "usage: "
    << argv[0]
    << " <shared_memory_name> <log_mutex_name> <log_file_name>"
    << std::endl;
    return 1;
  }

  // set SIGTERM signal handler to unlink shm
  ::signal(SIGTERM, LoggerSigTermHandler);
  ::signal(SIGINT, LoggerSigTermHandler);

  // build Consumer and start writing to file
  log_writer = new logger::Consumer(argv[kMemName], argv[kSigName]);
  log_writer->Consume(argv[kLogName]);

  return 0;
}


void LoggerSigTermHandler(int sig) {
  if (sig == SIGTERM || sig == SIGINT)
    delete log_writer;
}
