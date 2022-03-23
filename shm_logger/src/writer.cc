// Copyright 2022 CSCE 311
//

#include <shm_logger/inc/consumer.h>

#include <cstddef>
#include <iostream>


// Deletes kLogger memory (calls destructor)
void LoggerSigTermHandler(int sig);


logger::Consumer* log_writer;


int main(int argc, char* argv[]) {
  assert(argc == 4
    && "usage: consumer <shared_memory_name> <log_signal_mux_name> <log_file_name>");

  // set SIGTERM signal handler to unlink shm
  ::signal(SIGTERM, LoggerSigTermHandler);
  ::signal(SIGINT, LoggerSigTermHandler);

  // build Consumer and start writing to file
  log_writer = new logger::Consumer(argv[1], argv[2]);
  log_writer->Consume(argv[3]);

  return 0;
}


void LoggerSigTermHandler(int sig) {
  if (sig == SIGTERM || sig == SIGINT)
    delete log_writer;
}

