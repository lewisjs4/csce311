// Copyright 2022 CSCE 311
//

#include <pg_cache_logger/inc/consumer.h>


namespace logger {

Consumer::Consumer() : log_sig_(log_sig_name_) {
  // create signal mux (unlocked by producer(s))
  log_sig_.Create(0);
  log_sig_.Open();
}


Consumer::~Consumer() {
  // delete named semaphore
  log_sig_.Destroy();

  ::exit(0);
}


std::tuple<int, off_t> Consumer::OpenFile(const char file_name[], int flags) {
  // open and capture file descriptor
  int fd = ::open(file_name, flags);
  if (fd < 0) {
    std::cerr << ::strerror(errno) << std::endl;
    log_sig_.Destroy();
    ::exit(errno);
  }

  // get size of file
  struct ::stat f_stats;
  if (::fstat(fd, &f_stats) < 0)
    HandleError("OpenFile");

  return {fd, f_stats.st_size};
}


void Consumer::HandleError(const char msg[]) {
  std::cerr << "Source: " << msg << std::endl;
  std::cerr << '\t' << ::strerror(errno) << std::endl;
  log_sig_.Destroy();
  ::exit(errno);
}


void Consumer::Consume(const char log_file_name[]) {
  while (true) {
    log_sig_.Down();  // block until client ready

    // open transfer file and get size
    int buf_fd;
    long int buf_size;  // off_t is a long int
    std::tie(buf_fd, buf_size) = OpenFile(buf_file_name_, O_RDWR);

    // map to transfer file pages in page cache
    char *buf_file_addr = static_cast<char *>(
      ::mmap(nullptr, buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, buf_fd, 0));
    if (buf_file_addr == MAP_FAILED)
      HandleError("Transfer file map");
    if (::close(buf_fd) < 0)
      HandleError("Transfer file map close");

    // open log file and get size
    int log_fd;
    long int log_size;  // off_t is a long int
    std::tie(log_fd, log_size) = OpenFile(log_file_name, O_RDWR);

    // add necessary bytes to end of log file
    if (::fallocate(log_fd, 0, log_size, buf_size) < 0)
      HandleError("Allocating additional log file size");

    // map to log file pages in page cache and allocate additional pages
    char *log_file_addr = static_cast<char *>(::mmap(nullptr,
                                                     log_size + buf_size,
                                                     PROT_READ | PROT_WRITE,
                                                     MAP_SHARED,
                                                     log_fd,
                                                     0));
    if (log_file_addr == MAP_FAILED)
      HandleError("Log file map");
    if (::close(log_fd) < 0)
      HandleError("Log file map close");
    
    // copy from transfer file to log file (should use strncat)
    for (long int i = 0; i < buf_size; ++i) {
      log_file_addr[log_size + i] = buf_file_addr[i];
    }

    // update log file
    if (msync(log_file_addr, log_size + buf_size, MS_SYNC) < 0)
      HandleError("Synchronizing log file map");

    // empty transfer file
    if (::truncate(buf_file_name_, 0) < 0)
      HandleError("Emptying transfer file");

    // release copy of mapped mem
    if (::munmap(buf_file_addr, buf_size))
      HandleError("Buffer file unmap");
    if (::munmap(log_file_addr, log_size + buf_size))
      HandleError("Log file unmap");
  }
}


}  // namespace logger
