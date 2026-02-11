// Copyright 2026 CSCE 311
//
#ifndef READERS_WRITERS_INC_MUTEX_MANAGER_H_
#define READERS_WRITERS_INC_MUTEX_MANAGER_H_

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <cassert>    // assert
#include <cstddef>    // size_t
#include <vector>

namespace csce311 {

class ThreadMutexManager {
 public:
  // When index equals number of stored mutexes, a new mutex is
  // created. Otherwise, a previous location gets reused, the original
  // mutex at that location destroyed.
  static size_t Create(size_t index = 0);

  inline static void Destroy() {
    for (::pthread_mutex_t& mutex : mutexes_)
      ::pthread_mutex_destroy(&mutex);
    mutexes_.clear();
  }

  inline static void Lock(size_t index = 0) {
    ::pthread_mutex_lock(&(mutexes_.at(index)));
  }

  inline static void Unlock(size_t index = 0) {
    ::pthread_mutex_unlock(&(mutexes_.at(index)));
  }

 private:
  static std::vector<::pthread_mutex_t> mutexes_;
};

}  // namespace csce311

#endif  // READERS_WRITERS_INC_MUTEX_MANAGER_H_

