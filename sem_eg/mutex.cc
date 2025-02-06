// Copyright 2025 CSCE 311
//

#include <sem_eg/thread.h>
#include <sem_eg/thread_sem_manager.h>

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <unistd.h>  // UNIX standard header (sleep)

#include <cstddef>  // size_t
#include <cstdlib>  // atoi

#include <iostream>
#include <vector>


class MutexThread : public Thread<MutexThread> {
 public:
  MutexThread(::pthread_t thread_id,
              ::size_t internal_id,
              const std::string& message)
      : Thread<MutexThread>(thread_id, internal_id), message_(message) {
    // empty
  }

  static void *Execute(MutexThread *thread_i) {
    // begin critical section
    ::ThreadSemaphoreManager::Down();

    for (size_t i = 0; i < 5; ++i) {
      std::cout << thread_i->message_ << ' ';
      ::usleep(1);  // wait 1us
    }

    std::cout << std::endl;

    // end critical section
    ::ThreadSemaphoreManager::Up();

    return nullptr;
  }

 private:
  std::string message_;
};


// entry point to process
//
// arguments ignored
//
int main(int argc, char* argv[]) {
  std::size_t kThreadCount = 4;
  ThreadSemaphoreManager::Create(1);  // binary semaphore, aka MUTEX

  // independent threads competing for print space
  std::vector<MutexThread> threads;

  // create thread info in main's stack/proc heap
  for (::size_t i = 0; i < kThreadCount; ++i)
    threads.push_back(MutexThread(::pthread_t(), i, "Thread " + std::to_string(i)));

  // create threads
  for (::MutexThread& thread : threads)
    if (pthread_create(&(thread.thread_id()),
                       nullptr,  // restrictions (read manual for details)
                       ::Thread<MutexThread>::Execute,  // function thread executes
                       static_cast<void*>(&thread)) != 0) {
      ::ThreadSemaphoreManager::Down();
      std::cerr << "Thread creation failed. Aborting." << std::endl;
      ::ThreadSemaphoreManager::Up();
      ThreadSemaphoreManager::Destroy();
      return -1;
    } else {
      ::ThreadSemaphoreManager::Down();
      std::cout << "Thread " << thread.id() << " created.\n";
      ::ThreadSemaphoreManager::Up();
    }

  // wait for threads
  for (::MutexThread& thread : threads) {
    pthread_join(thread.thread_id(), nullptr);
    ::ThreadSemaphoreManager::Down();
    std::cout << "Thread " << thread.id() << " completed.\n";
    ::ThreadSemaphoreManager::Up();
  }

  // clean up semaphores
  ThreadSemaphoreManager::Destroy();

  return 0;
}

