// Copyright 2023 CSCE 311
//

#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

#include <climits>

#include <iostream>
#include <vector>


void* WorkerThread(void* arg) {
  std::cout << "\t" << static_cast<char*>(arg) << " starting..." << std::endl;

  for (int i = 0; i < INT_MAX >> 3; ++i) {}

  std::cout << "\t" << static_cast<char*>(arg) << " complete!" << std::endl;

  return nullptr;
}


int main(int argc, char* argv[]) {
  std::vector<::pthread_t> threads;

  std::cout << "Creating threads\n";
  for (int i = 1; i < argc; ++i) {
    threads.push_back(pthread_t());

    ::pthread_create(&threads[i - 1], nullptr, WorkerThread, argv[i]);
  }

  std::cout << "Threads created" << std::endl;

  for (::pthread_t& thread : threads)
    ::pthread_join(thread, nullptr);

  std::cout << "Threads complete" << std::endl;

  return 0;
}
