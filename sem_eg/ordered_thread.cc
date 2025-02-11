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

constexpr std::size_t kSemPrint = 0;
constexpr std::size_t kSemLock = 1;


class OrderedThread : public Thread<OrderedThread> {
 public:
  OrderedThread(::pthread_t thread_id,
                std::size_t internal_id,
                std::size_t* current,
                const std::size_t* order)
      : Thread<OrderedThread>(thread_id, internal_id),
      current_(current), order_(order) {
    // empty
  }

  static void *Execute(OrderedThread *thread_i) {
    while (true) {
      // enter thread-critical section
      ::ThreadSemaphoreManager::Down(kSemLock);
      if (thread_i->order_[*(thread_i->current_)] == thread_i->id()) {
        ::ThreadSemaphoreManager::Down(kSemPrint);
        std::cout << "Thread " << thread_i->id() << " executed!" << std::endl;
        ::ThreadSemaphoreManager::Up(kSemPrint);
        *(thread_i->current_) += 1;
        ::ThreadSemaphoreManager::Up(kSemLock);
        break;
      }
      ::ThreadSemaphoreManager::Up(kSemLock);
      // end critical section
    }

    return thread_i;
  }

 private:
  std::size_t* current_;
  const std::size_t* order_; // array of indices 
};


// entry point to process
//
// arguments ignored
//
int main(int argc, char* argv[]) {
  const std::size_t kThreadCount = 8;
  ThreadSemaphoreManager::Create(1, kSemPrint);  // binary semaphore, aka MUTEX
  ThreadSemaphoreManager::Create(0, kSemLock);  // start locked

  // independent threads competing for print space
  std::vector<OrderedThread> threads;

  // create thread info in main's stack/proc heap
  std::size_t current_thread = 0;
  const std::size_t order[] = { 8, 1, 2, 6, 3, 7, 5, 4 };
  for (::size_t i = 0; i < kThreadCount; ++i)
    threads.push_back(OrderedThread(::pthread_t(), i + 1, &current_thread, order));

  // create threads
  for (::OrderedThread& thread : threads)
    if (pthread_create(&(thread.thread_id()),
                       nullptr,  // restrictions (read manual for details)
                       ::Thread<OrderedThread>::Execute,  // function thread executes
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
  ::ThreadSemaphoreManager::Up(kSemLock);

  // wait for threads
  for (::OrderedThread& thread : threads) {
    pthread_join(thread.thread_id(), nullptr);
    ::ThreadSemaphoreManager::Down();
    std::cout << "Thread " << thread.id() << " completed.\n";
    ::ThreadSemaphoreManager::Up();
  }

  // clean up semaphores
  ThreadSemaphoreManager::Destroy();

  return 0;
}

