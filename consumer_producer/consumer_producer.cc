// Copyright 2024 CSCE 311
//
// This is an example of semaphore-synchronized multi-slot produce/consumer
// problem.
//
//
#include <consumer_producer/thread.h>
#include <consumer_producer/thread_sem_manager.h>

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <semaphore.h>  // POSIX semaphore support
#include <unistd.h>  // UNIX standard header (sleep)

#include <cstddef>  // size_t
#include <cstdlib>  // sleep srand rand
#include <ctime>  // time

#include <iostream>
#include <string>
#include <utility>
#include <vector>


const size_t kProductionTime = 1;  // time to produce a unit of work
const size_t kMinConsumptionTime = 8;  // min time to consume a unit of work 
const size_t kMaxConsumptionTime = 16;  // max time to consume a unit of work
const size_t kNoConsumers = 4;  // number of consumers

const size_t kBufferSize = 12;  // size of circular buffer


// Provides scoped names for the different semaphores and mutexes managed by
//     ThreadSemaphoreManager
//
//   Naming is from Google style guidelines. Don't blame me.
enum class SemaphoreId : ::size_t {
  kBufferAvailable = 0,
  kItemsAvailable = 1,
  kBufferLock = 2,
  kPrintLock = 3
};


// Wrapper class. Updates method API to use SemaphoreIds, rather than the
//     size_t indices.
class SemaphoreManager : public ::ThreadSemaphoreManager {
 public:
  inline static size_t Create(::size_t count, ::SemaphoreId id);

  inline static void Down(::SemaphoreId id);  // lock if binary semaphore

  inline static void Up(::SemaphoreId id);  // unlock if binary sempahore
};


class Consumer : public Thread<Consumer> {
 public:
  Consumer(::pthread_t thread_id,
           std::size_t index,
           std::vector<::size_t>* buffer_reference,
           std::size_t* read_index_reference);


  // Uses SemaphoreId::kItemsAvailable signal and SemaphoreId::kBufferLock to
  // coordinate with other threads to consume ``jobs'' from the buffer.
  static void* StartRoutine(void* ptr);

 private:
  std::vector<std::size_t>* buffer_;  //  reference to the shared buffer;
                                      //  heap address stored in stack
  std::size_t* buffer_index_;  //  a reference to the shared current read point
                               //  in the buffer
};


class Producer : public Thread<Producer> {
 public:
  Producer(::pthread_t thread_id, std::vector<::size_t>* buffer_reference);

  // Uses SemaphoreId::kBufferAvailable signal and SemaphoreId::kBufferLock to
  // coordinate with other threads to produce ``jobs'' in the buffer.
  static void* StartRoutine(void* ptr);

 private:
  std::vector<std::size_t>* buffer_;  // reference to shared buffer; heap
                                      // address stored in stack
  std::size_t buffer_index_;  // write point in shared buffer
};


void PrintThreaded(const std::string& msg);


int main(/* int argc, char* argv[] */) {
  ::srand(::time(nullptr));  // seed the random gen with current time

  // create semaphores and mutexes
  // consumption signal
  SemaphoreManager::Create(kBufferSize, SemaphoreId::kBufferAvailable);
  // production signal
  SemaphoreManager::Create(0, SemaphoreId::kItemsAvailable);
  // buffer access control
  SemaphoreManager::Create(1, SemaphoreId::kBufferLock);
  // STDOUT access control
  SemaphoreManager::Create(1, SemaphoreId::kPrintLock);

  // the buffer is where all "jobs" are stored, shared with threads
  // "jobs" are represented by the time to complete
  std::vector<std::size_t> buffer(kBufferSize);
  std::size_t buffer_read_index = 0;  // stored here, shared with threads

  ::Producer producer(::pthread_t(), &buffer);
  ::pthread_create(&producer.thread_id(),
                   nullptr,  // restrictions (read on your own)
                   ::Producer::StartRoutine,  // pointer to function
                   static_cast<void *>(&producer));  // thread index

  // consumers contain thread id, reference to buffer, and reference to
  //     buffer's read index
  std::vector<::Consumer> consumers;
  for (size_t i = 0; i < kNoConsumers; ++i)
    consumers.push_back(
      ::Consumer(::pthread_t(), i, &buffer, &buffer_read_index));

  for (::Consumer& consumer : consumers)
    pthread_create(&consumer.thread_id(),
                   nullptr,  // restrictions (read on your own)
                   ::Consumer::StartRoutine,  // pointer to function
                   static_cast<void *>(&consumer));  // thread index
  while(true);
  ::pthread_join(producer.id(), nullptr);  // wait for parent to finish
  for (auto& consumer : consumers)
    ::pthread_join(consumer.id(), nullptr);  // wait for children to finish

  ::SemaphoreManager::Destroy();

  return 0;
}


void* ::Consumer::StartRoutine(void *ptr) {
  auto consumer = static_cast<::Consumer *>(ptr);

  while (true) {
    ::SemaphoreManager::Down(::SemaphoreId::kItemsAvailable);

    ::SemaphoreManager::Down(::SemaphoreId::kBufferLock);

    ::size_t consumption_time = consumer->buffer_->at(*consumer->buffer_index_);
    std::string print_index = std::to_string(*consumer->buffer_index_);

    *consumer->buffer_index_ = (*consumer->buffer_index_ + 1)
      % consumer->buffer_->size();

    ::PrintThreaded("\tConsumer " + std::to_string(consumer->id())
      + ": consuming from buffer[" + print_index + "], "
      + std::to_string(consumption_time) + "s to complete\n");

    ::SemaphoreManager::Up(::SemaphoreId::kBufferAvailable);

    ::SemaphoreManager::Up(::SemaphoreId::kBufferLock);

    ::sleep(consumption_time);

    ::PrintThreaded("\t  Consumer " + std::to_string(consumer->id())
      + ": completed buffer[" + print_index + "] after "
      + std::to_string(consumption_time) + "s\n");
  }

  return ptr;
}


void* Producer::StartRoutine(void* ptr) {
  auto producer = reinterpret_cast<Producer *>(ptr);

  while (true) {
    std::string str_index = std::to_string(producer->buffer_index_);
    PrintThreaded("Producer: producing at buffer[" + str_index + "]\n");

    ::sleep(kProductionTime);

    ::SemaphoreManager::Down(SemaphoreId::kBufferAvailable);

    ::SemaphoreManager::Down(SemaphoreId::kBufferLock);


    producer->buffer_->at(producer->buffer_index_)
      = kMinConsumptionTime + ::rand() % (kMaxConsumptionTime
        - kMinConsumptionTime + 1);


    PrintThreaded(std::string("  Producer: completed buffer[")
                  + std::to_string(producer->buffer_index_)
                  + "]\n");

    producer->buffer_index_ = ((producer->buffer_index_ + 1) % kBufferSize);

    ::SemaphoreManager::Up(SemaphoreId::kItemsAvailable);
    ::SemaphoreManager::Up(SemaphoreId::kBufferLock);
  }
}


::Consumer::Consumer(::pthread_t id,
                     ::size_t index,
                     std::vector<::size_t>* buffer,
                     ::size_t* buffer_index) : ::Thread<Consumer>(id, index),
                                                 buffer_(buffer),
                                                 buffer_index_(buffer_index) {
  // empty
}


Producer::Producer(::pthread_t thread_id,
                   std::vector<::size_t>* buffer_reference)
      : Thread<Producer>(thread_id, 0),
        buffer_(buffer_reference),
        buffer_index_(0) {
    // empty
}


::size_t SemaphoreManager::Create(::size_t count, SemaphoreId id) {
  return ThreadSemaphoreManager::Create(count, static_cast<::size_t>(id));
}


void SemaphoreManager::Down(SemaphoreId id) {
  ThreadSemaphoreManager::Down(static_cast<::size_t>(id));
}


void SemaphoreManager::Up(SemaphoreId id) {
  ThreadSemaphoreManager::Up(static_cast<::size_t>(id));
}

void PrintThreaded(const std::string& msg) {
  ::SemaphoreManager::Down(::SemaphoreId::kPrintLock);

  std::cout << msg;

  ::SemaphoreManager::Up(::SemaphoreId::kPrintLock);
}

