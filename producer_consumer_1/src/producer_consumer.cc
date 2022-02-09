// Copyright 2021 CSCE 240
//
// This is an example of semaphore-synchronized single slot producer/consumer
// problem
//

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <semaphore.h>  // POSIX semaphore support
#include <unistd.h>  // UNIX standard header (sleep)

#include <cstddef>  // size_t
#include <cstdlib>  // sleep srand rand
#include <ctime>  // time

#include <iostream>
#include <utility>
#include <vector>

#include <producer_consumer_1/inc/semaphore_manager.h>


// Provides scoped names for the different semaphores and mutexes managed by
//     ThreadSemaphoreManager
//
//   Naming is from Google style guidelines. Don't blame me.
enum class SemaphoreId : int {
  kItemAvailable = 0,
  kBufferAvailable = 1,
  kBufferLock = 2,
  kPrintLock = 3
};


// Wrapper class. Updates method API to use SemaphoreIds, rather than the
//     size_t indices.
class SemaphoreManager : public ThreadSemaphoreManager {
 public:
  inline static size_t Create(size_t count, SemaphoreId id);

  inline static void Down(SemaphoreId id);

  inline static void Up(SemaphoreId id);
};


class Consumer {
 public:
  Consumer(pthread_t, size_t, size_t*);

  pthread_t& id();


  // Requests an item from "slot" using SemaphoreId::kItemAvailable
  // Requests access to buffer using SemaphoreId::kBufferLock
  // "Consumes" job from buffer
  // Returns "slot" in buffer using SemaphoreId::kItemAvailable
  static void* Consume(void* ptr);

 private:
  pthread_t id_;  // consumers store their thread ids
  size_t index_;  // know their indices in the collection
  size_t* buffer_;  // have a reference to the shared buffer
};



int main(/* int argc, char* argv[] */) {
  ::srand(::time(nullptr));

  const size_t kProductionTime = 4;
  const size_t kMinConsumptionTime = 8;
  const size_t kMaxConsumptionTime = 16;
  const size_t kNoConsumers = 32;

  // create semaphores and mutexes
  ::SemaphoreManager::Create(0, ::SemaphoreId::kItemAvailable);
  ::SemaphoreManager::Create(1, ::SemaphoreId::kBufferAvailable);
  ::SemaphoreManager::Create(1, ::SemaphoreId::kBufferLock);
  ::SemaphoreManager::Create(1, ::SemaphoreId::kPrintLock);

  // the buffer is where all "jobs" are stored, shared with threads
  size_t buffer;

  // consumers contain thread id, reference to buffer, and reference to
  //     buffer's read index
  std::vector<::Consumer> consumers;
  for (size_t i = 0; i < kNoConsumers; ++i)
    consumers.push_back(::Consumer(pthread_t(), i, &buffer));

  for (auto& consumer : consumers)
    pthread_create(&consumer.id(),
                   nullptr,  // restrictions (read on your own)
                   ::Consumer::Consume,  // pointer to function
                   static_cast<void *>(&consumer));  // thread index

  while (true) {
    SemaphoreManager::Down(SemaphoreId::kPrintLock);
    std::cout << "Producer: producing for buffer" << std::endl;
    SemaphoreManager::Up(SemaphoreId::kPrintLock);

    ::sleep(kProductionTime);

    SemaphoreManager::Down(SemaphoreId::kBufferAvailable);

    SemaphoreManager::Down(SemaphoreId::kBufferLock);
    buffer = kMinConsumptionTime
      + ::rand() % (kMaxConsumptionTime - kMinConsumptionTime + 1);
    SemaphoreManager::Up(SemaphoreId::kBufferLock);

    SemaphoreManager::Up(SemaphoreId::kItemAvailable);

    SemaphoreManager::Down(SemaphoreId::kPrintLock);
    std::cout << "Producer: completed" << std::endl;
    SemaphoreManager::Up(SemaphoreId::kPrintLock);
  }

  for (auto& consumer : consumers)
    pthread_join(consumer.id(), nullptr);  // wait for children to finish

  SemaphoreManager::Destroy();

  return 0;
}



void* Consumer::Consume(void *ptr) {
  auto consumer = static_cast<Consumer *>(ptr);

  while (true) {
    SemaphoreManager::Down(SemaphoreId::kItemAvailable);

    SemaphoreManager::Down(SemaphoreId::kPrintLock);
    std::cout << "\tConsumer " << consumer->index_
      << ": consuming from buffer";
    SemaphoreManager::Up(SemaphoreId::kPrintLock);

    SemaphoreManager::Down(SemaphoreId::kBufferLock);
    size_t consumption_time = *consumer->buffer_;
    SemaphoreManager::Up(SemaphoreId::kBufferAvailable);
    SemaphoreManager::Up(SemaphoreId::kBufferLock);

    SemaphoreManager::Down(SemaphoreId::kPrintLock);
    std::cout << ", " << consumption_time << " seconds to complete"
      << std::endl;
    SemaphoreManager::Up(SemaphoreId::kPrintLock);

    sleep(consumption_time);

    SemaphoreManager::Down(SemaphoreId::kPrintLock);
    std::cout << "\tConsumer " << consumer->index_ << ": completed after "
      << consumption_time << " seconds" << std::endl;
    SemaphoreManager::Up(SemaphoreId::kPrintLock);
  }

  return ptr;
}


Consumer::Consumer(pthread_t id,
                   size_t index,
                   size_t* buffer) : id_(id), index_(index), buffer_(buffer) {
  // empty
}


pthread_t& Consumer::id() {
  return id_;
}


size_t SemaphoreManager::Create(size_t count, SemaphoreId id) {
  return ThreadSemaphoreManager::Create(count, static_cast<int>(id));
}


void SemaphoreManager::Down(SemaphoreId id) {
  ThreadSemaphoreManager::Down(static_cast<int>(id));
}


void SemaphoreManager::Up(SemaphoreId id) {
  ThreadSemaphoreManager::Up(static_cast<int>(id));
}
