// Copyright 2024 CSCE 311
//
//  An implementation of the Lamport's Bakery Algorithm using C++'s atomic
//  library for thread syncronization. This solves the problem demonstrated in
//  race_condition.cc
//
//  Notice this approach uses global memory.  This is usually poor planning,
//  but in this case, the solution was complex enough to detract from Lamport's
//  discussion.
//

#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <iostream>
#include <vector>


const std::size_t kThread_no = 8;  // number of threads

const int kThread_increments = 5000;  // number of increments per thread


// Begin Mutable Global memory
//   Not a great idea, but to avoid it would complicate the code too much for
//   casual discussion.
std::array<std::atomic_bool, kThread_no> entering;
std::array<std::atomic_size_t, kThread_no> number;
// End Mutable Global memory


void Lock(std::size_t i) {
  entering[i] = true;

  number[i] = 1 + *std::max_element(number.begin(), number.end());

  entering[i] = false;

  for (std::size_t j = 0; j < kThread_no; ++j) {
    // wait for thread j to enter the queue
    while (entering[j])
      continue;  // do nothing

    // process all threads at higher priority (or lower index) in queue
    while (number[j] != 0
           && (number[j] < number[i]
               || (number[j] == number[i] && j < i) ))
      continue;  // do nothing
  }
}


void Unlock(::size_t i) {
  number[i] = 0;
}


struct ThreadData {
  ThreadData(::pthread_t _id,
             std::size_t _index,
             int* _sum) : id(_id),
                          index(_index),
                          sum(_sum) {  /* empty */ }
  ::pthread_t id;
  ::size_t index;
  int *sum;
};


void* WorkerThread(void* arg) {
  ThreadData *argv = static_cast<ThreadData *>(arg);

  Lock(argv->index);
  for (int i = 0; i < kThread_increments; ++i)
    ++(*(argv->sum));
  Unlock(argv->index);

  return nullptr;
}


int main(int argc, char* argv[]) {
  int sum = 0;

  std::cout << "Creating threads\n";
  std::vector<::ThreadData> threads;
  for (int i = 0; i < kThread_no; ++i) {
    entering[i] = false;
    number[i] = 0;
    threads.push_back(::ThreadData(::pthread_t(),
                                   static_cast<std::size_t>(i),
                                   &sum));
  }
  std::cout << "Threads created" << std::endl;

  std::cout << "Current sum: " << sum << '\n';

  std::cout << "Starting threads\n";
  for (int i = 0; i < kThread_no; ++i)
    ::pthread_create(&threads[i].id, nullptr, WorkerThread, &threads[i]);
  std::cout << "Threads started" << '\n';

  for (::ThreadData& thread : threads)
    ::pthread_join(thread.id, nullptr);
  std::cout << "Threads completed" << '\n';

  std::cout << "Final sum, Expected:" << (kThread_increments * kThread_no)
    << ", Actual: " << sum << std::endl;

  return 0;
}
