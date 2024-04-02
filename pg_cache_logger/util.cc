// Copyright 2024 CSCE 311
//

#include <pg_cache_logger/util.h>



void HandleError(const char err_src[], std::ostream* out) {
  *out << err_src << ": " << std::endl;
  *out << '\t' << ::strerror(errno) << std::endl;
  ::exit(errno);
}


std::tuple<int, ::off_t> OpenFile(const char file_name[],
                                  int flags,
                                  std::ostream* out) {
  // open and capture file descriptor
  int fd = ::open(file_name, flags);
  if (fd < 0) {
    *out << ::strerror(errno) << std::endl;
    ::exit(errno);
  }

  // get size of file
  struct ::stat f_stats;
  if (::fstat(fd, &f_stats) < 0)
    HandleError("Open File");

  return {fd, f_stats.st_size};
}
