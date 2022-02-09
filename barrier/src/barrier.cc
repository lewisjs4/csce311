// Copyright 2021 CSCE 240
//

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

#include <barrier/inc/semaphore_manager.h>


// Provides scoped names for the different semaphores and mutexes managed by
//     ThreadSemaphoreManager
//
//   Naming is from Google style guidelines. Don't blame me.
enum class SemaphoreId : size_t {
  kBarrier = 0,
  kBarrierLock = 1,
  kPrintLock = 1
};


// Wrapper class. Updates method API to use SemaphoreIds, rather than the
//     size_t indices.
class SemaphoreManager : public ThreadSemaphoreManager {
 public:
  inline static size_t Create(int count, SemaphoreId id);

  inline static void Down(SemaphoreId id);

  inline static void Up(SemaphoreId id);
};


class WorkerWithBarrier {
 public:
  // Must begin with thread handle and index in external container 
  WorkerWithBarrier(pthread_t, size_t);

  // Returns the thread id handle
  pthread_t& thread_id();

  // Calls Barrier
  // After Barrier "does work" and exists
  static void* Work(void* ptr);


  // Should be invoked before instancing class object
  static void SetBarrierNumber(size_t count);


  // Acquires the print mutex and prints the given message
  void SendMessage(const char *) const;

 private:
  static void Barrier();  // Work() will call before beginning

  static size_t barrier_complete_count_;  // number of workers completed

  static size_t barrier_complete_total_;  // number of workers needed to
                                          //   complete before barrier is
                                          //   removed must be set before Work
                                          //   is invoked

  pthread_t id_;  // stored thread handle

  size_t index_;  // stored index in external container
};


int main(/* int argc, char* argv[] */) {
  ::srand(::time(nullptr));

  const size_t kNoWorkers = 7;

  ::WorkerWithBarrier::SetBarrierNumber(kNoWorkers);  // all workers must finish
                                                      //   before barrier is
                                                      //   unlocked

  // create semaphores and mutexes
  ::SemaphoreManager::Create(0, SemaphoreId::kBarrier);  // start locked
  ::SemaphoreManager::Create(1, SemaphoreId::kBarrierLock);  // mutex
  ::SemaphoreManager::Create(1, SemaphoreId::kPrintLock);  // mutex

  // workers contain thread id and internal id
  std::vector<::WorkerWithBarrier> workers;
  for (size_t i = 0; i < kNoWorkers; ++i)
    workers.push_back(::WorkerWithBarrier(pthread_t(), i));

  // create and invoke threads for workers
  for (auto& worker : workers) {
    ::pthread_create(&worker.thread_id(),  // address of worker's thread id
                     nullptr,  // restrictions (read on your own)
                     WorkerWithBarrier::Work,  // pointer to function
                     static_cast<void *>(&worker));  // worker instance ptr
  }

  // wait and reclaim threads
  for (auto& worker : workers)
    ::pthread_join(worker.thread_id(), nullptr);  // block while children finish

  SemaphoreManager::Destroy();

  return 0;
}


// init statics
//
size_t WorkerWithBarrier::barrier_complete_count_ = 0;
size_t WorkerWithBarrier::barrier_complete_total_ = 0;


// Barrier method
//
// - Precondition(s):
//     WorkerWithBarrier::SetBarrierNumber() invoked to set barrier completion
//     number
//
void WorkerWithBarrier::Barrier() {
  SemaphoreManager::Down(SemaphoreId::kBarrierLock);

  ++barrier_complete_total_;

  if (barrier_complete_count_ == barrier_complete_total_)
    for (size_t i = 0; i < barrier_complete_count_; ++i)
      SemaphoreManager::Up(SemaphoreId::kBarrier);

  SemaphoreManager::Up(SemaphoreId::kBarrierLock);

  SemaphoreManager::Down(SemaphoreId::kBarrier);
}


// Thread function
//
// -Precondition: ptr is a <WorkerWithBarrier *>
//
void* WorkerWithBarrier::Work(void *ptr) {
  const size_t kMinWorkTime = 1;
  const size_t kMaxWorkTime = 20;

  auto worker = static_cast<WorkerWithBarrier *>(ptr);

  size_t completion_time = kMinWorkTime;
  completion_time += ::rand() % (kMaxWorkTime - kMinWorkTime + 1);

  size_t init_time = round(0.75 * completion_time);
  size_t work_time = round(0.25 * completion_time);

  worker->SendMessage(
    std::string("initializing for " + std::to_string(init_time) + "s").c_str());

  ::sleep(init_time);

  Barrier();
  
  worker->SendMessage(
    std::string("working for " + std::to_string(work_time) + "s").c_str());

  ::sleep(work_time);

  worker->SendMessage("completed");

  return ptr;
}


void WorkerWithBarrier::SendMessage(const char* msg) const {
  SemaphoreManager::Down(SemaphoreId::kPrintLock);
  std::cout << "Worker " << (index_ + 1) << ": " << msg << std::endl;
  SemaphoreManager::Up(SemaphoreId::kPrintLock);
}


void WorkerWithBarrier::SetBarrierNumber(size_t count) {
  barrier_complete_count_ = count;
}


WorkerWithBarrier::WorkerWithBarrier(pthread_t id,
                                     size_t index) : id_(id),
                                                     index_(index) {
  // empty
}


pthread_t& WorkerWithBarrier::thread_id() {
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

