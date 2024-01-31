// Copyright 2024 CSCE 311
//

#include <sem_eg/thread_sem_manager.h>

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <unistd.h>  // UNIX standard header (sleep)

#include <cstddef>  // size_t
#include <cstdlib>  // atoi

#include <iostream>
#include <vector>


class Thread {
 public:
  Thread(::pthread_t thread_id,
         ::size_t internal_id,
         const std::string& message)
      : thread_id_(thread_id), internal_id_(internal_id), message_(message) {
    // empty
  }

  static void *Run(void *arg) {
    Thread *thread_i = static_cast<Thread*>(arg);  // information about
                                                   // executing thread

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

  ::pthread_t& thread_id() {
    return thread_id_;
  }

  ::size_t id() {
    return internal_id_;
  }

 private:
  ::pthread_t thread_id_;
  ::size_t internal_id_;
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
  std::vector<::Thread> threads;

  // create thread info in main's stack/proc heap
  for (::size_t i = 0; i < kThreadCount; ++i)
    threads.push_back(::Thread(::pthread_t(),
                      i,
                      "Thread " + std::to_string(i)));

  // create threads
  for (::Thread& thread : threads)
    if (pthread_create(&(thread.thread_id()),
                       nullptr,  // restrictions (read manual for details)
                       ::Thread::Run,
                       static_cast<void*>(&thread)) != 0) {
      std::cerr << "Thread creation failed. Aborting." << std::endl;
      ThreadSemaphoreManager::Destroy();
      return -1;
    } else {
      std::cout << "Thread " << thread.id() << " created.\n";
    }

  // wait for threads
  for (::Thread& thread : threads) {
    pthread_join(thread.thread_id(), nullptr);
    std::cout << "Thread " << thread.id() << " completed.\n";
  }

  // clean up semaphores
  ThreadSemaphoreManager::Destroy();

  return 0;
}


void *print_message_function(void *ptr) {
  std::string *message = static_cast<std::string *>(ptr);

  ThreadSemaphoreManager::Down();
  for (size_t i = 0; i < 10; ++i) {
    std::cout << *message << ' ';
    usleep(1);  // wait 1ms
  }

  std::cout << std::endl;
  ThreadSemaphoreManager::Up();

  return nullptr;
}
