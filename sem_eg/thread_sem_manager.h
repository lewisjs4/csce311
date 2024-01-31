// Copyright 2024 CSCE 311
//
// This wrapper class allows the managment of one or more semaphores for
// threads. It is not intended for use between processes.
// 
// The class provides static class members and methods to avoid global mutable
// variables.
//
#ifndef SEM_EG_THREAD_SEM_MANAGER_H_
#define SEM_EG_THREAD_SEM_MANAGER_H_


#include <semaphore.h>  // POSIX semaphore C interface

#include <cassert>  // assert
#include <cstddef>  // std::size_t

#include <vector>


class ThreadSemaphoreManager {
 public:
  static std::size_t Create(std::size_t count, std::size_t index = 0);

  inline static void Destroy() {
    // iterate through and destroy all semaphores created by this class
    for (::sem_t& mutex : semaphores_)
      ::sem_destroy(&mutex);
  }


  inline static void Down(std::size_t index = 0) {
    // get lock for specified semaphore
    ::sem_wait(&(semaphores_.at(index)));
  }


  inline static void Up(std::size_t index = 0) {
    // remove lock for specified semaphore
    ::sem_post(&(semaphores_.at(index)));
  }


 private:
  // ::sem_t is a wrapper for a file descriptor, an index into a
  //         kernel-maintained array of open files
  static std::vector<::sem_t> semaphores_;
};

#endif  // SEM_EG_THREAD_SEM_MANAGER_H_
