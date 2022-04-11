// Copyright 2022 CSCE 311
//
#ifndef PG_CACHE_LOGGER_INC_PG_CACHE_STORE_H_
#define PG_CACHE_LOGGER_INC_PG_CACHE_STORE_H_


#include <cstddef>  // size_t


struct SharedMemoryStore {
  size_t buffer_size;
  char buffer[1];  // this will be extended by external code
};


#endif  // PG_CACHE_LOGGER_INC_PG_CACHE_STORE_H_
