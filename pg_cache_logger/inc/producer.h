// Copyright 2022 CSCE 311
//
#ifndef PG_CACHE_LOGGER_INC_PRODUCER_H_
#define PG_CACHE_LOGGER_INC_PRODUCER_H_

#include <pg_cache_logger/inc/named_semaphore.h>

#include <fcntl.h>  // POSIX file control api
#include <sys/mman.h>  // POSIX shared memory map
#include <sys/unistd.h>  // UNIX standard header (sleep)

#include <cassert>
#include <cerrno>
#include <cstddef>  // size_t

#include <iostream>
#include <string>
#include <tuple>


namespace logger {

class Producer {
 public:
  Producer();


  int Produce(const std::string& msg);

 private:
  static constexpr char log_sig_name_[] = "sem_log_sig";
  static constexpr char buf_file_name_[] = "transfer_file";

  wrappers::NamedSemaphore log_sig_;  // shared memory log mutex
};

}  // namespace logger


#endif  // PG_CACHE_LOGGER_INC_PRODUCER_H_
