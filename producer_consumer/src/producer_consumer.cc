// Copyright 2022 CSCE 311
//
// This is an example of semaphore-synchronized multi-slot produce/consumer
// problem.
//


#include <producer_consumer/inc/semaphore_manager.h>

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <semaphore.h>  // POSIX semaphore support
#include <unistd.h>  // UNIX standard header (sleep)

#include <cstddef>  // size_t
#include <cstdlib>  // sleep srand rand
#include <ctime>  // time

#include <iostream>
#include <utility>
#include <vector>


// Provides scoped names for the different semaphores and mutexes managed by
//     ThreadSemaphoreManager
//
//   Naming is from Google style guidelines. Don't blame me.
enum class SemaphoreId : size_t {
  kBufferAvailable = 0,
  kItemsAvailable = 1,
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
  Consumer(pthread_t, size_t, std::vector<size_t>*, size_t*);

  pthread_t& id();


  // Requests an item using SemaphoreId::kItemsAvailable
  // Requests access to buffer using SemaphoreId::kBufferLock
  // "Consumes" one job from buffer
  // Updates shared buffer read location
  // Returns "slot" in buffer using SemaphoreId::kBufferAvailable
  // Returns access to buffer using SemaphoreId::kBufferLock
  static void* Consume(void* ptr);

 private:
  pthread_t id_;  // consumers store their thread ids
  size_t index_;  // know their indices in the collection
  std::vector<size_t>* buffer_;  // have a reference to the shared buffer
  size_t* buffer_index_;  // and a reference to the shared current read point
                          //     in the buffer
};


void PrintThreaded(const std::string& msg);


int main(/* int argc, char* argv[] */) {
  ::srand(::time(nullptr));

  const size_t kProductionTime = 1;
  const size_t kMinConsumptionTime = 8;
  const size_t kMaxConsumptionTime = 16;
  const size_t kNoConsumers = 8;

  const size_t kBufferSize = 8;

  // create semaphores and mutexes
  SemaphoreManager::Create(kBufferSize, SemaphoreId::kBufferAvailable);
  SemaphoreManager::Create(0, SemaphoreId::kItemsAvailable);
  SemaphoreManager::Create(1, SemaphoreId::kBufferLock);
  SemaphoreManager::Create(1, SemaphoreId::kPrintLock);

  // the buffer is where all "jobs" are stored, shared with threads
  std::vector<size_t> buffer(kBufferSize);
  size_t buffer_read_index = 0;  // stored here, shared with threads

  // consumers contain thread id, reference to buffer, and reference to
  //     buffer's read index
  std::vector<::Consumer> consumers;
  for (size_t i = 0; i < kNoConsumers; ++i)
    consumers.push_back(
      ::Consumer(pthread_t(), i, &buffer, &buffer_read_index));

  for (auto& consumer : consumers)
    pthread_create(&consumer.id(),
                   nullptr,  // restrictions (read on your own)
                   ::Consumer::Consume,  // pointer to function
                   static_cast<void *>(&consumer));  // thread index

  size_t buffer_write_index = 0;
  while (true) {
    PrintThreaded("Producer: producing at buffer["
      + std::to_string(buffer_write_index) + "]\n");

    ::sleep(kProductionTime);

    ::SemaphoreManager::Down(SemaphoreId::kBufferAvailable);

    ::SemaphoreManager::Down(SemaphoreId::kBufferLock);

    buffer[buffer_write_index] = kMinConsumptionTime
      + ::rand() % (kMaxConsumptionTime - kMinConsumptionTime + 1);

    buffer_write_index = ((buffer_write_index + 1) % kBufferSize);
    ::SemaphoreManager::Up(SemaphoreId::kBufferLock);

    ::SemaphoreManager::Up(SemaphoreId::kItemsAvailable);


    PrintThreaded("Producer: completed\n");
  }

  for (auto& consumer : consumers)
    pthread_join(consumer.id(), nullptr);  // wait for children to finish

  ::SemaphoreManager::Destroy();

  return 0;
}


void* ::Consumer::Consume(void *ptr) {
  auto consumer = static_cast<::Consumer *>(ptr);

  while (true) {
    ::SemaphoreManager::Down(::SemaphoreId::kItemsAvailable);

    ::SemaphoreManager::Down(::SemaphoreId::kBufferLock);

    size_t consumption_time = consumer->buffer_->at(*consumer->buffer_index_);
    *consumer->buffer_index_ = (*consumer->buffer_index_ + 1)
      % consumer->buffer_->size();

    ::PrintThreaded("\tConsumer " + std::to_string(consumer->index_)
      + ": consuming from buffer[" + std::to_string(*consumer->buffer_index_)
      + "], " + std::to_string(consumption_time) + "s to complete\n");

    ::SemaphoreManager::Up(::SemaphoreId::kBufferAvailable);

    ::SemaphoreManager::Up(::SemaphoreId::kBufferLock);

    ::sleep(consumption_time);

    ::PrintThreaded("\tConsumer " + std::to_string(consumer->index_)
      + ": completed after " + std::to_string(consumption_time) + "s\n");
  }

  return ptr;
}


::Consumer::Consumer(pthread_t id,
                   size_t index,
                   std::vector<size_t>* buffer,
                   size_t* buffer_index) : id_(id),
                                           index_(index),
                                           buffer_(buffer),
                                           buffer_index_(buffer_index) {
  // empty
}


::pthread_t& ::Consumer::id() {
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

void PrintThreaded(const std::string& msg) {
  ::SemaphoreManager::Down(::SemaphoreId::kPrintLock);

  std::cout << msg;

  ::SemaphoreManager::Up(::SemaphoreId::kPrintLock);
}

