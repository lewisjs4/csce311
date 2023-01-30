// Copyright 2023 CSCE 311
//

#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

#include <climits>

#include <iostream>
#include <vector>


const int kCount_max = 5000;


void* WorkerThread(void* arg) {
  int* argv = static_cast<int*>(arg);

  for (int i = 0; i < kCount_max; ++i)
    ++(*argv);

  return nullptr;
}


int main(int argc, char* argv[]) {
  std::vector<::pthread_t> threads;
  int sum = 0;

  std::cout << "Current sum: " << sum << '\n';
  std::cout << "Creating threads\n";
  for (int i = 0; i < ::atoi(argv[1]); ++i) {
    threads.push_back(pthread_t());

    ::pthread_create(&threads[i], nullptr, WorkerThread, &sum);
  }

  std::cout << "Threads created" << std::endl;

  for (::pthread_t& thread : threads)
    ::pthread_join(thread, nullptr);

  std::cout << "Threads complete" << '\n';
  std::cout << "Final sum, Expected:" << (kCount_max * ::atoi(argv[1]))
    << ", Actual: " << sum << std::endl;

	return 0;
}
