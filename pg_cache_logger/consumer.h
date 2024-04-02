// Copyright 2024 CSCE 311
//
#ifndef SHM_LOGGER_INC_CONSUMER_H_
#define SHM_LOGGER_INC_CONSUMER_H_


#include <pg_cache_logger/named_semaphore.h>
#include <pg_cache_logger/util.h>

#include <fcntl.h>
#include <sys/mman.h>  // POSIX shared memory map
#include <sys/stat.h>  // data returned by stat function
#include <sys/types.h>  // type off_t from stat struct
#include <sys/unistd.h>  // Unix standard header (ftruncate)

#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstddef>  // size_t

#include <fstream>
#include <iostream>
#include <string>
#include <tuple>


namespace logger {

class Consumer {
 public:
  Consumer();


  // Removes named semaphore and exits
  //
  ~Consumer();


  // Do forever: when prompted load file into memory map
  //
  void Consume(const char log_name[]);

 private:
  // constexpr lets us avoid "error: in-class initialization of static data
  static constexpr char log_sig_name_[] = "sem_log_sig";
  static constexpr char buf_file_name_[] = "transfer_file";

  wrappers::NamedSemaphore log_sig_;
};

}  // namespace logger

#endif  // SHM_LOGGER_INC_CONSUMER_H_
