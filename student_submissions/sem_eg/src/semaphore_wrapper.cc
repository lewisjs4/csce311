// Copyright 2023 LittleCube
//

#include <semaphore_wrapper.h>

Semaphore::Semaphore() {
  ::sem_init(&semaphore_, 0, 0);
}

Semaphore::Semaphore(size_t init_value) {
  ::sem_init(&semaphore_, 0, init_value);
}

void Semaphore::Post() {
  ::sem_post(&semaphore_);
}

void Semaphore::Wait() {
  ::sem_wait(&semaphore_);
}

Semaphore::~Semaphore() {
  ::sem_destroy(&semaphore_);
}
