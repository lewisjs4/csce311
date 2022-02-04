// Copyright 2021 CSCE 240
//

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <semaphore.h>  // POSIX semaphore support
#include <unistd.h>  // UNIX standard header (sleep)

#include <cstddef>  // size_t
#include <cstdlib>  // atoi sleep

#include <iostream>
#include <utility>
#include <vector>


class ThreadSemaphoreManager {
 public:
  static size_t Create(size_t count = 0) {
    mutexes.push_back(sem_t());
    sem_init(&(mutexes.back()), 0, count);

    return mutexes.size() - 1;
  }

  static void Destroy() {
    for (sem_t& mutex : mutexes)
      sem_destroy(&mutex);
  }

  static void Down(size_t index=0) {
    sem_wait(&(mutexes.at(index)));
  }

  static void Up(size_t index=0) {
    sem_post(&(mutexes.at(index)));
  }
 private:
  static std::vector<sem_t> mutexes;
};
std::vector<sem_t> ThreadSemaphoreManager::mutexes = std::vector<sem_t>();


void *EnterRestaurantFunction( void *ptr );

int main(int argc, char* argv[]) {
  const size_t kThread_count = 32;
  const size_t kRestaurant_capacity = 16;
  ThreadSemaphoreManager::Create(kRestaurant_capacity);
  ThreadSemaphoreManager::Create(1);  // binary semaphore

  std::vector<pthread_t> threads(kThread_count);
  std::vector<std::pair<size_t *, size_t>> arg_pairs;  // holds index (id) of each thread, necessary for
                            //   sharing info with threads

  size_t capacity = kRestaurant_capacity;
  for (size_t i = 0; i < threads.size(); ++i)
    arg_pairs.push_back(std::make_pair(&capacity, i));

  for (size_t i = 0; i < threads.size(); ++i) {
    pthread_create(&threads[i],
                   NULL,  // restrictions (read on your own)
                   EnterRestaurantFunction,  // pointer to function
                   static_cast<void *>(&arg_pairs[i]));  // thread index
  }

  for (pthread_t& thread : threads)
    pthread_join(thread, NULL);  // wait for children to finish

  ThreadSemaphoreManager::Destroy();
  return 0;
}

void* EnterRestaurantFunction(void *ptr) {
  ThreadSemaphoreManager::Down();
  auto arg = static_cast<std::pair<size_t *, size_t> *>(ptr);

  ThreadSemaphoreManager::Down(1);
  --*(arg->first);
  std::cout << "Thread " << arg->second << " enters the restaurant" << std::endl;
  std::cout << "Capacity: " << *(arg->first) << std::endl;
  ThreadSemaphoreManager::Up(1);

  sleep((arg->second >> 1) + 1);  // wait 1 second + thread id
 
  ThreadSemaphoreManager::Down(1);
  std::cout << "Thread " << arg->second << " leaves the restaurant" << std::endl;
  ++*(arg->first);
  std::cout << "Capacity: " << *(arg->first) << std::endl;
  ThreadSemaphoreManager::Up(1);

  ThreadSemaphoreManager::Up();
  return ptr;
}
