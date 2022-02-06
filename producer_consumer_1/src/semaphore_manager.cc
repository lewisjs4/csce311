// Copyright 2021 CSCE 240
//


#include <prod_cons/inc/semaphore_manager.h>


// init the semaphores_
std::vector<::sem_t> ThreadSemaphoreManager::semaphores_ 
  = std::vector<::sem_t>();


size_t ThreadSemaphoreManager::Create(size_t count, int index) {
// TODO: the automatic selection and addition of new semaphores is wrong
  assert(index <= static_cast<int>(semaphores_.size()));

  // create and initialize semaphore at new index
  if (index == static_cast<int>(semaphores_.size())) {
    semaphores_.push_back(::sem_t());
    ::sem_init(&(semaphores_.back()), 0, count);

    return semaphores_.size() - 1;
  }

  // recreate semaphore at indicated index
  ::sem_destroy(&semaphores_[index]);
  semaphores_[index] = ::sem_t();
  ::sem_init(&semaphores_[index], 0, count);
  return index;
}

