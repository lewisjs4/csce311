// Copyright 2024 CSCE 311
//
#ifndef SHM_LOGGER_PRODUCER_H_
#define SHM_LOGGER_PRODUCER_H_

#include <shm_logger/named_semaphore.h>
#include <shm_logger/shm_store.h>

#include <sys/mman.h>  // POSIX shared memory map
#include <sys/unistd.h>  // UNIX standard header (sleep)

#include <cassert>
#include <cerrno>
#include <cstddef>  // size_t

#include <iostream>
#include <string>


namespace logger {

class Producer {
 public:
  Producer(const char shm_name[], const char mutex_name[]);

  ~Producer() {
    // empty
  }


  int Produce(const std::string& msg);

 private:
  static const std::size_t kBufferSize = 1024;
  static const std::size_t kSharedMemSize
    = sizeof(SharedMemoryStore<kBufferSize>);

  std::string shm_name_;  // shared memory name
  wrappers::NamedSemaphore shm_log_signal_;  // shared memory log mutex
};

}  // namespace logger


#endif  // SHM_LOGGER_PRODUCER_H_
