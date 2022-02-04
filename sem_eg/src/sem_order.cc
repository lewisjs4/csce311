// Copyright 2021 CSCE 240
//

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <semaphore.h>  // POSIX semaphore support
#include <unistd.h>  // UNIX standard header (sleep)

#include <cstddef>  // size_t
#include <cstdlib>  // atoi sleep
#include <cstdio>

#include <functional>
#include <initializer_list>
#include <iostream>
#include <utility>
#include <vector>


class ThreadSemaphoreManager {
 public:

  static size_t Create(size_t count=0) {
    mutexes.push_back(sem_t());
    sem_init(&(mutexes.back()), 0, count);

    return mutexes.size() - 1;
  }

  static void Destroy() {
    for (sem_t& mutex : mutexes)
      sem_destroy(&mutex);
  }

  static void Down(size_t index=0) {
    sem_wait(&(mutexes.at(index)));
  }

  static void Up(size_t index=0) {
    sem_post(&(mutexes.at(index)));
  }
 private:
  static std::vector<sem_t> mutexes;
};
std::vector<sem_t> ThreadSemaphoreManager::mutexes = std::vector<sem_t>();


void *SignaledFunction( void *ptr );

int main(int argc, char* argv[]) {
  ThreadSemaphoreManager::Create();

  pthread_t sig_thread;

  pthread_create(&sig_thread,
                 nullptr,  // restrictions (read on your own)
                 SignaledFunction,  // pointer to function
                 nullptr);  // thread index

  std::cout << "Signal sent! Waiting for thread" << std::endl;
  sleep(1);  // sleep for 1 second
  ThreadSemaphoreManager::Up();

  pthread_join(sig_thread, nullptr);

  ThreadSemaphoreManager::Destroy();
  return 0;
}

void* SignaledFunction(void *ptr) {
  ThreadSemaphoreManager::Down();
  std::cout << "Signal Received! Starting work." << std::endl;

  return ptr;
}
