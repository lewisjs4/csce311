// Copyright 2024 CSCE 311
//
// An implementation of Lamport's Bakery Algorithm using C++'s atomic
// library for thread synchronization. This solves the problem demonstrated in
// race_condition.cc
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

// Global shared memory
std::array<std::atomic_bool, kThread_no> entering;
std::array<std::atomic_size_t, kThread_no> number;

void Lock(std::size_t i) {
  entering[i] = true;

  // Compute max safely
  std::size_t max_val = 0;
  for (std::size_t j = 0; j < kThread_no; ++j) {
    max_val = std::max(max_val, number[j].load(std::memory_order_acquire));
  }
  number[i] = max_val + 1;

  entering[i] = false;

  for (std::size_t j = 0; j < kThread_no; ++j) {
    while (entering[j])
      continue;

    while (number[j] != 0 &&
           (number[j] < number[i] || (number[j] == number[i] && j < i)))
      continue;
  }
}

void Unlock(std::size_t i) {
  number[i].store(0, std::memory_order_release);
}

struct ThreadData {
  ThreadData(::pthread_t _id, std::size_t _index, int* _sum)
      : id(_id), index(_index), sum(_sum) {}
  ::pthread_t id;
  std::size_t index;
  int* sum;
};

void* WorkerThread(void* arg) {
  ThreadData* argv = static_cast<ThreadData*>(arg);

  Lock(argv->index);
  // Begin critical section
  for (int i = 0; i < kThread_increments; ++i)
    ++(*(argv->sum));
  // End critical section
  Unlock(argv->index);

  return nullptr;
}

int main(int argc, char* argv[]) {
  int sum = 0;

  std::cout << "Creating threads\n";
  std::vector<::ThreadData> threads;
  threads.reserve(kThread_no);  // Avoid multiple allocations

  for (std::size_t i = 0; i < kThread_no; ++i) {
    entering[i] = false;
    number[i] = 0;
    threads.emplace_back(::pthread_t(), static_cast<std::size_t>(i), &sum);
  }
  std::cout << "Threads created" << std::endl;
  std::cout << "Current sum: " << sum << '\n';

  std::cout << "Starting threads\n";
  for (std::size_t i = 0; i < kThread_no; ++i)
    ::pthread_create(&threads[i].id, nullptr, WorkerThread, &threads[i]);
  std::cout << "Threads started" << '\n';

  for (::ThreadData& thread : threads)
    ::pthread_join(thread.id, nullptr);
  std::cout << "Threads completed" << '\n';

  std::cout << "Final sum, Expected: " << (kThread_increments * kThread_no)
            << ", Actual: " << sum << std::endl;

  return 0;
}
