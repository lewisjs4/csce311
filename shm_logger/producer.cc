// Copyright 2024 CSCE 311
//

#include <shm_logger/producer.h>

namespace logger {

Producer::Producer(const char shm_name[], const char mutex_name[])
    : shm_name_(shm_name),
      shm_log_signal_(mutex_name) {
  // get shared memory log signal named semaphore
  //   created by and locked by consumer
  shm_log_signal_.Open();
}


int Producer::Produce(const std::string& msg) {
  // open shared memory with read/write rights and get file descriptor
  int shm_fd = ::shm_open(shm_name_.c_str(), O_RDWR, 0);
  if (shm_fd < 0) {
    std::cerr << "Producer::Produce ::shm_open: " << ::strerror(errno)
      << std::endl;
    return errno;
  }

  // get copy of mapped mem
  SharedMemoryStore<kSharedMemSize>* store
    = static_cast<SharedMemoryStore<kSharedMemSize>*>(
      ::mmap(nullptr,
             sizeof(SharedMemoryStore<kSharedMemSize>),
             PROT_READ | PROT_WRITE,
             MAP_SHARED,
             shm_fd,
             0));
  if (store == MAP_FAILED) {
    std::cerr << "Producer::Produce ::mmap: " << ::strerror(errno)
      << std::endl;
    return errno;
  }

  // copy string msg into shared memory via strncpy ('n' for bounded copy)
  strncpy(store->buffer, msg.c_str(), kSharedMemSize);
  store->buffer_size = msg.length();

  // signal consumer
  shm_log_signal_.Up();

  // release copy of mapped mem
  if (::munmap(store, sizeof(SharedMemoryStore<kSharedMemSize>)) < 0) {
    std::cerr << "Producer::Produce ::munmap: " <<  ::strerror(errno)
      << std::endl;
    return errno;
  }

  return 0;
}

}  // namespace logger

