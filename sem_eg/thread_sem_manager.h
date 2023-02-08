// Copyright 2023 CSCE 311
//
#ifndef PRODUCER_CONSUMER_INC_SEMAPHORE_MANAGER_H_
#define PRODUCER_CONSUMER_INC_SEMAPHORE_MANAGER_H_


#include <semaphore.h>  // POSIX semaphore support

#include <cassert>  // assert
#include <cstddef>  // ::size_t

#include <vector>


class ThreadSemaphoreManager {
 public:
  static ::size_t Create(::size_t count, ::size_t index = 0);

  inline static void Destroy() {
    for (::sem_t& mutex : semaphores_)
      ::sem_destroy(&mutex);
  }


  inline static void Down(::size_t index = 0) {
    ::sem_wait(&(semaphores_.at(index)));
  }


  inline static void Up(::size_t index = 0) {
    ::sem_post(&(semaphores_.at(index)));
  }


 private:
  static std::vector<::sem_t> semaphores_;
};

#endif  // PRODUCER_CONSUMER_INC_SEMAPHORE_MANAGER_H_
