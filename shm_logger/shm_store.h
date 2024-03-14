// Copyright 2024 CSCE 311
//
#ifndef SHM_LOGGER_SHM_STORE_H_
#define SHM_LOGGER_SHM_STORE_H_


#include <cstddef>  // size_t


template <std::size_t BufferSize>
struct SharedMemoryStore {
  std::size_t buffer_size;  // used to denote size of array in shared mem
  char buffer[BufferSize];  // this will be extended by external code
};


#endif  // SHM_LOGGER_SHM_STORE_H_
