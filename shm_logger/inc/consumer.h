// Copyright 2022 CSCE 311
//
#ifndef SHM_LOGGER_INC_CONSUMER_H_
#define SHM_LOGGER_INC_CONSUMER_H_


#include <shm_logger/inc/named_semaphore.h>
#include <shm_logger/inc/shm_store.h>

#include <sys/mman.h>  // POSIX shared memory map
#include <sys/unistd.h>  // Unix standard header (ftruncate)

#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstddef>  // size_t

#include <fstream>
#include <iostream>
#include <string>


namespace logger {

class Consumer {
 public:
  explicit Consumer(const char* kSHM_name, const char* shm_log_signal_name);


  // Unlinks shared memory for future use
  //
  ~Consumer();

  // Do forever: while there are logs in log, write to file
  //
  void Consume(const char log_name[]);

 private:
  const char* shm_name_;

  SharedMemoryStore *store_;

  wrappers::NamedSemaphore shm_log_signal_;
};

}  // namespace logger

#endif  // SHM_LOGGER_INC_CONSUMER_H_
