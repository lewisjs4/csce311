// Copyright 2026 CSCE 311
//
#ifndef READERS_WRITERS_INC_SEMAPHORE_MANAGER_H_
#define READERS_WRITERS_INC_SEMAPHORE_MANAGER_H_

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <semaphore.h>  // POSIX semaphore support

#include <cassert>  // assert
#include <cstddef>  // size_t

#include <vector>


namespace csce311 {

class ThreadSemaphoreManager {
 public:
  // When index equals number of stored semaphores, a new semaphore is
  // created. Otherwise, a previous location gets reused, the original
  // semaphore at that location destroyed.
  static size_t Create(int count, size_t index = 0);


  inline static void Destroy() {
    for (::sem_t& mutex : semaphores_)
      ::sem_destroy(&mutex);
  }


  inline static void Down(size_t index=0) {
    ::sem_wait(&(semaphores_.at(index)));
  }


  inline static void Up(size_t index=0) {
    ::sem_post(&(semaphores_.at(index)));
  }


 private:
  static std::vector<::sem_t> semaphores_;

};

}  // namespace csce311

#endif  // READERS_WRITERS_INC_SEMAPHORE_MANAGER_H_
