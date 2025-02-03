// Copyright 2023 CSCE 311
//

#include <sem_eg/thread_sem_manager.h>



// init the semaphores_
std::vector<::sem_t> ThreadSemaphoreManager::semaphores_
  = std::vector<::sem_t>();


size_t ::ThreadSemaphoreManager::Create(std::size_t count, std::size_t index) {
  assert(index <= semaphores_.size());

  if (index == semaphores_.size()) {
    // create and initialize semaphore at new index
    semaphores_.push_back(::sem_t());

    // int pshared hard-coded 0 because this is only between threads
    // https://pubs.opengroup.org/onlinepubs/7908799/xsh/sem_init.html
    ::sem_init(&(semaphores_.back()), 0, count);

    return semaphores_.size() - 1;
  }

  // ensure there are no threads blocked on semaphore
  int current_count;
  ::sem_getvalue(&semaphores_[index], &current_count);
  assert(current_count > 0);

  // recreate existing semaphore at indicated index
  ::sem_destroy(&semaphores_[index]);
  semaphores_[index] = ::sem_t();
  ::sem_init(&semaphores_[index], 0, count);

  return index;
}

