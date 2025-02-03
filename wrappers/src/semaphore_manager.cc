// Copyright 2022 CSCE 311
//


#include <readers_writers/inc/semaphore_manager.h>


// init the semaphores_
std::vector<::sem_t> ThreadSemaphoreManager::semaphores_ 
  = std::vector<::sem_t>();


size_t ThreadSemaphoreManager::Create(int count, size_t index) {
  assert(index <= semaphores_.size());

  // create and initialize semaphore at new index
  if (index == semaphores_.size()) {
    semaphores_.push_back(::sem_t());
    ::sem_init(&(semaphores_.back()), 0, count);

    return semaphores_.size() - 1;
  }

  // reuse location for new semaphore at indicated index
  ::sem_destroy(&semaphores_[index]);
  semaphores_[index] = ::sem_t();
  ::sem_init(&semaphores_[index], 0, count);
  return index;
}

