// Copyright 2022 CSCE 311
//

#include <shm_logger/inc/producer.h>

namespace logger {

Producer::Producer(const char shm_name[], const char mutex_name[])
    : shm_name_(shm_name),
      shm_log_signal_(mutex_name) {
  // get shared memory log signal named semaphore
  //   created by and locked by consumer
  shm_log_signal_.Open();
}


int Producer::Produce(const std::string& msg) {
  // open shared memory and get file descriptor
  int shm_fd = ::shm_open(shm_name_.c_str(), O_RDWR, 0);
  if (shm_fd < 0) {
    std::cerr << ::strerror(errno) << std::endl;

    return errno;
  }

  // get copy of mapped mem
  SharedMemoryStore* store = static_cast<SharedMemoryStore*>(
    ::mmap(nullptr,
           sizeof(SharedMemoryStore),
           PROT_READ | PROT_WRITE,
           MAP_SHARED,
           shm_fd,
           0));
  if (store == MAP_FAILED) {
    std::cerr << ::strerror(errno) << std::endl;

    return errno;
  }

  // copy string msg into shared memory via strncpy ('n' for bounded copy)
  strncpy(store->buffer, msg.c_str(), store->buffer_size);

  // signal consumer
  shm_log_signal_.Up();

  // release copy of mapped mem
  int result = ::munmap(store, sizeof(SharedMemoryStore));
  if (result < 0) {
    std::cerr << ::strerror(errno) << std::endl;

    return errno;
  }

  return 0;
}

}  // namespace logger


int main(int argc, char* argv[]) {
  assert(argc == 3 && "producer <shared_mem_name> <log_mutex_name>");

  logger::Producer log_writer(argv[1], argv[2]);

  std::cout << "Sending: >";

  const size_t kBuffer_size = 64;
  char buffer[kBuffer_size];
  std::cin.getline(buffer, kBuffer_size);
  buffer[std::cin.gcount() + 1] = '\n';

  log_writer.Produce(std::string(buffer));

  return 0;
}
