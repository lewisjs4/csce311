// Copyright 2022 CSCE 311
//
#ifndef SHM_LOGGER_SHM_STORE_H_
#define SHM_LOGGER_SHM_STORE_H_


#include <cstddef>  // size_t


template <typename BaseType, typename BufferType, std::size_t BufferSize>
struct FlexType : public BaseType {
  std::size_t buffer_size = BufferSize;
  BufferType buffer[BufferSize];
};


template <std::size_t BufferSize>
struct SharedMemoryStore {
  std::size_t buffer_size;
  char buffer[BufferSize];  // this will be extended by external code
};


constexpr std::size_t SharedMemoryStoreSizeInPages(
    std::size_t buffer_size, std::size_t page_size = 4096) {
  return page_size * (1 + (buffer_size + sizeof(std::size_t)) / page_size);
}


#endif  // SHM_LOGGER_SHM_STORE_H_
