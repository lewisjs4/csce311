// Copyright 2021 CSCE 240
//
// This is an example of semaphore-synchronized multi-slot produce/consumer
// problem.
//

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <semaphore.h>  // POSIX semaphore support
#include <unistd.h>  // UNIX standard header (sleep)

#include <cstddef>  // size_t
#include <cstdlib>  // sleep srand rand
#include <ctime>  // time

#include <iostream>
#include <string>
#include <vector>

#include <readers_writers_1/inc/semaphore_manager.h>


// Provides scoped names for the different semaphores and mutexes managed by
//     ThreadSemaphoreManager
//
//   Naming is from Google style guidelines. Don't blame me.
enum class SemaphoreId : size_t {
  kReadLock = 0,
  kWriteLock = 1,
  kPrintLock = 2
};


// Wrapper class. Updates method API to use SemaphoreIds, rather than the
//     size_t indices.
class SemaphoreManager : public ThreadSemaphoreManager {
 public:
  inline static size_t Create(size_t count, SemaphoreId id);

  inline static void Down(SemaphoreId id);

  inline static void Up(SemaphoreId id);
};


class Reader {
 public:
  Reader(pthread_t, size_t);


  // requests read lock on buffer
  // reads buffer
  // simulates long read by sleeping for 1~2s
  // returns read lock on buffer
  // simulates processing by sleeping
  // - Precondition: buffer must be set with SetBuffer before calling
  static void* Read(void*);


  static void SetBuffer(const std::string*);
  

  pthread_t& thread_id();

 private:
  static const std::string* buffer_;  // shared read buffer

  pthread_t id_;  // corresponding thread id
  size_t index_;  // index in internal collection
};


// Acquires print lock to print
void ThreadedPrint(const char*);


int main(/* int argc, char* argv[] */) {
  ::srand(::time(nullptr));

  const size_t kWriteTime= 1;
  const size_t kNoReaders = 4;
  const size_t kMinWaitTime = 1;
  const size_t kMaxWaitTime = 9;

  // create semaphores and mutexes
  SemaphoreManager::Create(1, SemaphoreId::kReadLock); 
  SemaphoreManager::Create(1, SemaphoreId::kWriteLock);
  SemaphoreManager::Create(1, SemaphoreId::kPrintLock);

  // data buffer shared with threads
  std::string buffer = "Message 0";
  ::Reader::SetBuffer(&buffer);

  // readers contain thread id and index in readers vector
  std::vector<::Reader> readers;
  for (size_t i = 0; i < kNoReaders; ++i)
    readers.push_back(::Reader(::pthread_t(), i));

  for (auto& reader : readers)
    ::pthread_create(&reader.thread_id(),
                     nullptr,  // restrictions (read on your own)
                     ::Reader::Read,  // pointer to function
                     static_cast<void *>(&reader));  // thread index

  int message = 0;
  while (true) {
    ::ThreadedPrint("Writer: needs to write");

    ::SemaphoreManager::Down(SemaphoreId::kReadLock);
    ::SemaphoreManager::Down(SemaphoreId::kWriteLock);

    ::ThreadedPrint("Writer: writing");

    ::sleep(kWriteTime);

    buffer = "Message " + std::to_string(++message);

    ThreadedPrint("Writer: done writing");

    ::SemaphoreManager::Up(SemaphoreId::kReadLock);
    ::SemaphoreManager::Up(SemaphoreId::kWriteLock);

    size_t wait = kMinWaitTime + ::rand() % (kMaxWaitTime - kMinWaitTime + 1);
    ::sleep(wait);
  }

  for (auto& reader : readers)
    pthread_join(reader.thread_id(), nullptr);  // wait for children to finish

  SemaphoreManager::Destroy();

  return 0;
}


void* Reader::Read(void *ptr) {
  const size_t kMinReadTime = 2;
  const size_t kMaxReadTime = 4;

  auto reader = static_cast<Reader *>(ptr);

  while (true) {
    std::string msg = "\tReader " + std::to_string(reader->index_)
      + ": waiting";
    ::ThreadedPrint(msg.c_str());

    ::SemaphoreManager::Down(SemaphoreId::kReadLock);

    msg = "\tReader " + std::to_string(reader->index_) + ": reading";
    ::ThreadedPrint(msg.c_str());

    ::sleep(1);

    msg = "\tReader " + std::to_string(reader->index_) + ": "
      + *reader->buffer_;
    ::ThreadedPrint(msg.c_str());

    ::SemaphoreManager::Up(SemaphoreId::kReadLock);

    size_t wait_time = kMinReadTime
      + (::rand() % (kMaxReadTime - kMinReadTime + 1));
    ::sleep(wait_time << 2);
  }

  return ptr;
}


const std::string* Reader::buffer_ = nullptr;


void Reader::SetBuffer(const std::string* buffer) {
  buffer_ = buffer;
}


Reader::Reader(pthread_t id, size_t index) : id_(id),
                                             index_(index) {
  // empty
}


pthread_t& Reader::thread_id() {
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


void ThreadedPrint(const char* message) {
  SemaphoreManager::Down(SemaphoreId::kPrintLock);
  std::cout << message << std::endl;
  SemaphoreManager::Up(SemaphoreId::kPrintLock);
}

