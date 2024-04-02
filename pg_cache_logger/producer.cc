// Copyright 2024 CSCE 311
//

#include <pg_cache_logger/producer.h>

namespace logger {

Producer::Producer() : log_sig_(log_sig_name_) {
  // get shared memory log signal named semaphore
  //   created by and locked by consumer
  log_sig_.Open();
}


int Producer::Produce(const std::string& msg) {
  // get buffer file descriptor and stats (size)
  int buf_fd;
  ::off_t buf_size;
  std::tie(buf_fd, buf_size) = OpenFile(buf_file_name_, O_RDWR);

  // assure file empty and get map to page cache
  if (buf_size > 0) {
    std::cerr << "Transfer file is not empty" << std::endl;
    ::exit(1);
  }

  // make file big enough
  if (static_cast<std::size_t>(buf_size) < msg.length() + 1)
    if (::fallocate(buf_fd, 0, 0, msg.length() + 1) < 0)  // need to add \n
      HandleError("fallocate");
  char *buf_file_addr = static_cast<char *>(::mmap(nullptr,
                                                   msg.length() + 1,
                                                   PROT_READ | PROT_WRITE,
                                                   MAP_SHARED,
                                                   buf_fd,
                                                   0));
  if (buf_file_addr == MAP_FAILED)
    HandleError("Buffer file map");
  if (::close(buf_fd) < 0)
    HandleError("Buffer file close");

  // copy string msg into memory mapped file in page cache
  for (size_t i = 0; i < msg.length(); ++i)
    buf_file_addr[i] = msg[i];
  buf_file_addr[msg.length()] = '\n';

  // update transfer file
  if (::msync(buf_file_addr, buf_size, MS_SYNC) < 0)
    HandleError("Synchronizing transfer file map");

  // signal consumer
  log_sig_.Up();

  // release copy of mapped mem
  if (::munmap(buf_file_addr, msg.length() + 1))
    HandleError("Buffer file unmap");

  return 0;
}

}  // namespace logger

