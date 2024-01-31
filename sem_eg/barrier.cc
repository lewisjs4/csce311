// Copyright 2024 CSCE 311
//

#include <sem_eg/thread.h>
#include <sem_eg/thread_sem_manager.h>

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
  kBarrier = 0,
  kBarrierLock = 1,
  kPrintLock = 2
};


// Wrapper class. Updates method API to use SemaphoreIds, rather than the
//     size_t indices.
class SemaphoreIdManager : public ThreadSemaphoreManager {
 public:
  inline static size_t Create(int count, ::SemaphoreId id);

  inline static void Down(::SemaphoreId id);

  inline static void Up(::SemaphoreId id);
};


// Extends parent class Thread with a static ``start_routine'' function for
// ::pthread_create, along with methods to support the class specialization.
class ThreadWithBarrier : public Thread<ThreadWithBarrier> {
 public:
  using ::Thread<ThreadWithBarrier>::Thread;

  // Calls Barrier
  // After Barrier releases, it "does work" and exits
  static void* Execute(void* ptr);

  // Should be invoked before instancing class object
  static void SetBarrierNumber(size_t count);

  // Acquires the print mutex and prints the given message
  void SendMessage(const char *) const;

 private:
  static void Barrier();  // Execute() will call before beginning

  static size_t barrier_complete_count_;  // number of threads completed

  static size_t barrier_complete_total_;  // number of threads needed to
                                          //   complete before barrier is
                                          //   removed must be set before
                                          //   Execute is invoked
};


// The main function initializes random seed, creates all semaphores, creates,
// executes,and joins all threads
//
int main(/* int argc, char* argv[] */) {
  ::srand(::time(nullptr));

  const size_t kNoWorkers = 7;

  ::ThreadWithBarrier::SetBarrierNumber(kNoWorkers);  // all threads must finish
                                                      //   before barrier is
                                                      //   unlocked

  // create semaphores and mutexes
  ::SemaphoreIdManager::Create(0, ::SemaphoreId::kBarrier);  // start locked
  ::SemaphoreIdManager::Create(1, ::SemaphoreId::kBarrierLock);  // mutex
  ::SemaphoreIdManager::Create(1, ::SemaphoreId::kPrintLock);  // mutex

  // threads contain thread id and internal id
  std::vector<::ThreadWithBarrier> threads;
  // initialize all threads
  for (size_t i = 0; i < kNoWorkers; ++i)
    threads.push_back(::ThreadWithBarrier(::pthread_t(), i));

  // create and run each thread in threads
  for (auto& thread : threads) {
    ::pthread_create(&thread.thread_id(),  // address of thread's thread id
                     nullptr,  // restrictions (read on your own)
                     ThreadWithBarrier::Execute,  // pointer to function
                     static_cast<void *>(&thread));  // thread instance ptr
  }

  // wait and reclaim threads
  for (auto& thread : threads)
    ::pthread_join(thread.thread_id(), nullptr);  // block while children finish

  SemaphoreIdManager::Destroy();

  return 0;
}


// init statics
//
size_t ::ThreadWithBarrier::barrier_complete_count_ = 0;
size_t ::ThreadWithBarrier::barrier_complete_total_ = 0;


// Barrier method
//   Called after thread initialization to indicate a ready thread
//
// - Precondition(s):
//     ThreadWithBarrier::SetBarrierNumber() invoked to set barrier completion
//     number
//
void ::ThreadWithBarrier::Barrier() {
  // binary semaphore
  ::SemaphoreIdManager::Down(::SemaphoreId::kBarrierLock);

  ++barrier_complete_total_;  // one more ready thread

  // check whether ``this'' thread is the last one
  if (barrier_complete_count_ == barrier_complete_total_) {
    // last thread calls up enough time to unlock all threads + 1 for itself
    for (size_t i = 0; i < barrier_complete_count_; ++i)
      ::SemaphoreIdManager::Up(::SemaphoreId::kBarrier);
  }

  // release hold on barrier lock
  ::SemaphoreIdManager::Up(::SemaphoreId::kBarrierLock);

  // semaphore begins at 0, locked; last thread will call Up enough times to
  // allow all threads to complete
  ::SemaphoreIdManager::Down(::SemaphoreId::kBarrier);
}


// Thread function
//
// -Precondition: ptr is a <ThreadWithBarrier *>
//
void* ThreadWithBarrier::Execute(void *ptr) {
  const size_t kMinExecuteTime = 5;
  const size_t kMaxExecuteTime = 10;

  size_t completion_time = kMinExecuteTime;
  // randomly generate thread's total time to work and initialize
  completion_time += ::rand() % (kMaxExecuteTime - kMinExecuteTime + 1);

  // 3/4 of time is spent initializing
  size_t init_time = round(0.75 * completion_time);
  // rest is spent working
  size_t work_time = completion_time - init_time;


  // get ``this'' reference of executing thread
  auto thread = reinterpret_cast<ThreadWithBarrier *>(ptr);
  // use instance method (not static)
  thread->SendMessage(
    std::string("initializing for " + std::to_string(init_time) + "s").c_str());

  // initialize
  ::sleep(init_time);

  // finished initializing, update barrier statically (not instance method)
  Barrier();

  // use instance method (not static)
  thread->SendMessage(
    std::string("working for " + std::to_string(work_time) + "s").c_str());

  // work
  ::sleep(work_time);

  // use instance method (not static)
  thread->SendMessage("completed");

  return 0;
}


void ThreadWithBarrier::SendMessage(const char* msg) const {
  SemaphoreIdManager::Down(::SemaphoreId::kPrintLock);
  std::cout << "Worker " << id() << ": " << msg << std::endl;
  SemaphoreIdManager::Up(::SemaphoreId::kPrintLock);
}


void ThreadWithBarrier::SetBarrierNumber(size_t count) {
  barrier_complete_count_ = count;
}


size_t SemaphoreIdManager::Create(int count, ::SemaphoreId id) {
  return ThreadSemaphoreManager::Create(count, static_cast<size_t>(id));
}


void SemaphoreIdManager::Down(::SemaphoreId id) {
  ThreadSemaphoreManager::Down(static_cast<size_t>(id));
}


void SemaphoreIdManager::Up(::SemaphoreId id) {
  ThreadSemaphoreManager::Up(static_cast<size_t>(id));
}

