// Copyright 2026 CSCE 311 Spring
//
// System libs
#include <sys/sysinfo.h>
#include <pthread.h>
// C libs
#include <cstddef>
#include <cstdlib>
// C++ libs
#include <iostream>
#include <vector>


void* StartRoutine(void* arg);


int main(int argc, char* argv[]) {
  // use nprocs to get "n" if user does not supply
  int n = argc == 2 ? ::atoi(argv[1]) : get_nprocs();
  std::vector<::pthread_t> thread_pool(n);

  std::cout << "Creating " << n << " threads\n";
  int thread_index = 0;
  for (::pthread_t& thread : thread_pool) {
    ::pthread_create(&thread, nullptr, StartRoutine, &thread_index);
    ++thread_index;
  }

  // blocking calls
  for (::pthread_t thread : thread_pool)
    ::pthread_join(thread, nullptr);

  std::cout << "Threads complete" << std::endl;
  std::cout << "Why were indices wrong?" << std::endl;

  return 0;
}


void* StartRoutine(void* arg) {
  int *index = static_cast<int *>(arg);
  std::cout << "StartRoutine received index " << *index << std::endl;

  return nullptr;
}
