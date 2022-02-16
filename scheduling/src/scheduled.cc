// Copyright 2022 CSCE 311
//


#include <scheduling/inc/semaphore_manager.h>

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <semaphore.h>  // POSIX semaphore support
#include <unistd.h>  // UNIX standard header (sleep)

#include <cmath>  // round
#include <cstddef>  // size_t
#include <cstdlib>  // sleep srand rand
#include <ctime>  // time

#include <iostream>
#include <string>
#include <utility>
#include <vector>


// Provides scoped names for the different semaphores and mutexes managed by
//     ThreadSemaphoreManager
//
//   Naming is from Google style guidelines. Don't blame me.
enum class SemaphoreId : size_t {
  kPrintLock = 0
};


// Wrapper class. Updates method API to use SemaphoreIds, rather than the
//     size_t indices.
class SemaphoreManager : public ThreadSemaphoreManager {
 public:
  inline static size_t Create(int count, SemaphoreId id);

  inline static void Down(SemaphoreId id);

  inline static void Up(SemaphoreId id);
};


class Thread {
 public:
  // Must begin with thread handle and index in external container 
  Thread(pthread_t, size_t);


  // Returns the thread id handle
  pthread_t& thread_id();


  static void* Run(void* ptr);


  // Should be invoked before instancing class object
  static void SetBarrierNumber(size_t count);


 private:
  pthread_t id_;  // stored thread handle

  size_t index_;  // stored index in external container
};


// Acquires the print mutex and prints the given message
void PrintThreaded(const char *);


int main(/* int argc, char* argv[] */) {
  ::srand(::time(nullptr));

  // create print mutex
  ::SemaphoreManager::Create(1, SemaphoreId::kPrintLock);

  const size_t kThread_count = 5;

  ::pthread_attr_t attributes;
  int thread_scope;  // system or process scope

  // get current scope setting and update to process scope if system
  ::pthread_attr_init(&attributes);
  if (pthread_attr_getscope(&attributes, &thread_scope) == 0) { 
    if (thread_scope == PTHREAD_SCOPE_PROCESS) {
      std::clog << "PTHREAD_SCOPE_PROCESS" << std::endl;
    } else if (thread_scope == PTHREAD_SCOPE_SYSTEM) {
      // contention is among all processes' threads, set to only this proc
      std::cout << "PTHREAD_SCOPE_SYSTEM" << std::endl;
      pthread_attr_setscope(&attributes, PTHREAD_SCOPE_SYSTEM); 
    } else {
      std::cerr << "Illegal scope value.\n" << std::endl;
    }
  } else {
    std::cerr << "Unable to get scheduling scope" << std::endl;
  }

  // create list of Thread instances containing thread id and index in vector
  std::vector<::Thread> threads;
  for (size_t i = 0; i < kThread_count; ++i)
    threads.push_back(::Thread(pthread_t(), i));

  // create and invoke threads for threads
  for (auto& thread : threads) {
    ::pthread_create(&thread.thread_id(),  // address of thread's thread id
                     &attributes,  // restrictions (read on your own)
                     Thread::Run,  // pointer to function
                     static_cast<void *>(&thread));  // thread instance ptr
  }

  // wait and reclaim threads
  for (auto& thread : threads)
    ::pthread_join(thread.thread_id(), nullptr);  // block while children finish

  SemaphoreManager::Destroy();

  return 0;
}


// Thread::Run static method 
//
// -Precondition: ptr is a <Thread *>
//
void* Thread::Run(void *ptr) {
  const size_t kMinRunTime = 1;
  const size_t kMaxRunTime = 20;

  auto thread = static_cast<Thread *>(ptr);

  size_t completion_time = kMinRunTime;
  completion_time += ::rand() % (kMaxRunTime - kMinRunTime + 1);

  size_t init_time = round(0.75 * completion_time);
  size_t work_time = round(0.25 * completion_time);

  PrintThreaded(
    std::string("Worker " + std::to_string(thread->index_ + 1)
      + " initializing for " + std::to_string(init_time) + "s").c_str());

  ::sleep(init_time);

  PrintThreaded(
    std::string("Worker " + std::to_string(thread->index_ + 1)
      + " working for " + std::to_string(work_time) + "s").c_str());

  ::sleep(work_time);

  PrintThreaded("completed");

  return ptr;
}


void PrintThreaded(const char* msg) {
  SemaphoreManager::Down(SemaphoreId::kPrintLock);
  std::cout <<  msg << std::endl;
  SemaphoreManager::Up(SemaphoreId::kPrintLock);
}


Thread::Thread(pthread_t id, size_t index) : id_(id), index_(index) {
  // empty
}


pthread_t& Thread::thread_id() {
  return id_;
}


size_t SemaphoreManager::Create(int count, SemaphoreId id) {
  return ThreadSemaphoreManager::Create(count, static_cast<size_t>(id));
}


void SemaphoreManager::Down(SemaphoreId id) {
  ThreadSemaphoreManager::Down(static_cast<size_t>(id));
}


void SemaphoreManager::Up(SemaphoreId id) {
  ThreadSemaphoreManager::Up(static_cast<size_t>(id));
}

