// Copyright 2023 LittleCube
//

#pragma once

#include <semaphore.h>

#include <cstddef>

/*
 * Semaphore wrapper class.
 * Allows the user to easily create a semaphore.
 * 
 */
class Semaphore {
 public:
  /*
   * Semaphore constructor.
   * Create a semaphore with an initial value of 0.
   * 
   * No parameters.
   * 
   */
  Semaphore();

  /*
   * Semaphore constructor.
   * Create a semaphore with the given initial value.
   * 
   * @param init_value: the value to start the semaphore's internal
   *                    counter at.
   * 
   */
  explicit Semaphore(size_t init_value);

  /*
   * Increment the semaphore's internal counter. If the internal counter
   * is positive as a result, any threads waiting on
   * this semaphore will be unblocked.
   * 
   * Also, even if the internal counter is negative or zero after incrementing,
   * let the next wait through. [citation needed]
   * 
   */
  void Post();

  /*
   * Decrement the semaphore's internal counter. If the internal counter
   * is zero as a result, any threads waiting on
   * this semaphore will be blocked.
   * 
   */
  void Wait();

  /*
   * Semaphore destructor.
   * Call the POSIX semaphore destroy function, freeing up the resources
   * used by the semaphore.
   * 
   */
  ~Semaphore();

 private:
  ::sem_t semaphore_;  // member semaphore
};
