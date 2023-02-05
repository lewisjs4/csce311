// Copyright 2023 LittleCube
//

#include <cubiphore.h>

Cubiphore::Cubiphore(size_t init_value) {
  ::sem_init(&semaphore_, 0, init_value);
}

void Cubiphore::Post() {
  ::sem_post(&semaphore_);
}

void Cubiphore::Wait() {
  ::sem_wait(&semaphore_);
}

Cubiphore::~Cubiphore() {
  ::sem_destroy(&semaphore_);
}
