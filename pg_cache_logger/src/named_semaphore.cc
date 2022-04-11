// Copyright 2022 CSCE 311
//


#include <pg_cache_logger/inc/named_semaphore.h>

namespace wrappers {


void NamedSemaphore::Create(int count) {
  // create new semaphore with name
  sem_ptr_ = ::sem_open(name_.c_str(),
                        O_CREAT,
                        S_IRGRP | S_IRUSR | S_IWGRP | S_IWUSR,
                        count);

  if (sem_ptr_ == SEM_FAILED) {
    std::cerr << ::strerror(errno) << std::endl;
    ::exit(EXIT_FAILURE);
  }

  if (sem_close(sem_ptr_) < 0) {
    std::cerr << ::strerror(errno) << std::endl;
    ::exit(EXIT_FAILURE);
  }

  sem_ptr_ = nullptr;
}


void NamedSemaphore::Open() {
  // create new semaphore with name
  sem_ptr_ = ::sem_open(name_.c_str(), 0, 0, 0);

  if (sem_ptr_ == SEM_FAILED) {
    std::cerr << ::strerror(errno) << std::endl;
    ::exit(EXIT_FAILURE);
  }
}


}  // namespace wrappers
