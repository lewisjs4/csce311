// Copyright 2026 CSCE 311
//

#include <readers_writers/inc/mutex_manager.h>

namespace csce311 {

// init the mutexes_
std::vector<::pthread_mutex_t> ThreadMutexManager::mutexes_
  = std::vector<::pthread_mutex_t>();


size_t ThreadMutexManager::Create(size_t index) {
  assert(index <= mutexes_.size());

  // create and initialize mutex at new index
  if (index == mutexes_.size()) {
    mutexes_.push_back(::pthread_mutex_t());
    ::pthread_mutex_init(&(mutexes_.back()), nullptr);
    return mutexes_.size() - 1;
  }

  // reuse location for new mutex at indicated index
  ::pthread_mutex_destroy(&mutexes_[index]);
  mutexes_[index] = ::pthread_mutex_t();
  ::pthread_mutex_init(&mutexes_[index], nullptr);
  return index;
}

}  // namespace csce311

