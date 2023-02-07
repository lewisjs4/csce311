// Copyright 2023 CSCE 311
//

#include <sem_eg/thread_sem_manager.h>

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <semaphore.h>  // POSIX semaphore support
#include <unistd.h>  // UNIX standard header (sleep)

#include <cstddef>  // size_t
#include <cstdlib>  // atoi sleep

#include <iostream>
#include <utility>
#include <vector>


// settings for customers and capacity
const size_t kCustomerCount = 32;
const size_t kRestaurantCapacity = 16;


// two semaphores in semaphore manager
enum Semaphore { kSemCapacity = 0, kSemSTDOUT = 1 };


// generic-ish class for managing data to, from, and between threads
class Thread {
 public:
  
  Thread(::pthread_t thread_id, ::size_t internal_id, ::size_t* capacity)
      : thread_id_(thread_id), internal_id_(internal_id), capacity_(capacity) {
    // empty
  }

  static void *Enter(void *arg) {
    ::Thread* thread = static_cast<::Thread*>(arg);
    // request entry
    ThreadSemaphoreManager::Down(kSemCapacity);
  

    // request access to STDOUT
    ThreadSemaphoreManager::Down((kSemSTDOUT));
    --*(thread->capacity_);
    std::cout << "Thread " << thread->internal_id_ << " enters the restaurant"
      << std::endl;
    std::cout << "Capacity: " << *(thread->capacity_) << std::endl;
    ThreadSemaphoreManager::Up((kSemSTDOUT));
  
    // spend time
    sleep((thread->internal_id_ >> 1) + 1);  // wait 1 second + thread id
   
    // request access to STDOUT
    ThreadSemaphoreManager::Down((kSemSTDOUT));
    std::cout << "Thread " << thread->internal_id_ << " leaves the restaurant"
      << std::endl;
    ++*(thread->capacity_);
    std::cout << "Capacity: " << *(thread->capacity_) << std::endl;
    ThreadSemaphoreManager::Up((kSemSTDOUT));
  
    // withdraw
    ThreadSemaphoreManager::Up(kSemCapacity);

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
  ::size_t* capacity_;
};



void *EnterRestaurantFunction( void *ptr );

int main(int argc, char* argv[]) {
  ::ThreadSemaphoreManager::Create(kRestaurantCapacity);  // counting semaphore
  ::ThreadSemaphoreManager::Create(1);  // binary semaphore

  // size of a "restaurant" customer threads wish to visit
  ::size_t capacity = kRestaurantCapacity;

  // independent threads vying for entry to "restaurant"
  std::vector<::Thread> customers;

  // create customer thread data and give reference to restaurant
  for (::size_t i = 0; i < kCustomerCount; ++i)
    customers.push_back(::Thread(::pthread_t(), i, &capacity));

  // create threads
  for (::Thread& customer : customers)
    if (pthread_create(&(customer.thread_id()),
                       nullptr,
                       ::Thread::Enter,
                       static_cast<void *>(&customer))) {
      std::cerr << "Thread creation failed. Aborting." << std::endl;
      ThreadSemaphoreManager::Destroy();

      return -1;
    }

  // wait for threads
  for (::Thread& customer: customers)
    pthread_join(customer.thread_id(), nullptr);

  // clean up semaphores
  ThreadSemaphoreManager::Destroy();

  return 0;
}
