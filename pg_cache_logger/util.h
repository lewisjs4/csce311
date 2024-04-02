#ifndef PG_CACHE_LOGGER_UTIL_H_
#define PG_CACHE_LOGGER_UTIL_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <cstring>
#include <iostream>
#include <tuple>
#include <utility>


void HandleError(const char err_src[], std::ostream* out = &std::cerr);

std::tuple<int, ::off_t> OpenFile(const char file_name[],
                                  int flags,
                                  std::ostream* out = &std::cerr);

#endif  // PG_CACHE_LOGGER_UTIL_H_
