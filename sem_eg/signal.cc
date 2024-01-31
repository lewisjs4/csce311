// Copyright 2024 CSCE 311
//

#include <sem_eg/thread_sem_manager.h>

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


void* SignaledFunction(void* ptr);

int main(int argc, char* argv[]) {
  // create semaphore already locked
  ::ThreadSemaphoreManager::Create(0);

  ::pthread_t sig_thread;

  ::pthread_create(&sig_thread,
                 nullptr,  // restrictions (read on your own)
                 SignaledFunction,  // pointer to function
                 nullptr);

  std::cout << "main: Thread created..." << std::endl;
  // ::sleep(2);  // sleep for 2s
  int buf;
  std::cin >> buf;

  ::ThreadSemaphoreManager::Up();
  std::cout << "main: Signal sent!" << std::endl;

  ::pthread_join(sig_thread, nullptr);
  std::cout << "main: thread returned, terminating..." << std::endl;

  // clean up semaphores
  ::ThreadSemaphoreManager::Destroy();
  return 0;
}

void* SignaledFunction(void* ptr) {
  std::cout << "thread: Waiting for Signal..." << std::endl;
  ::ThreadSemaphoreManager::Down();

  std::cout << "thread: Signal Received! Starting work..." << std::endl;
  ::sleep(1);  // sleep for 1s

  return ptr;
}
