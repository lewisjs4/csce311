// Copyright 2023 CSCE 311
//

#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <vector>


const int kCount_max = 5000;


void Lock(::size_t i,
          std::vector<bool>* entering,
          std::vector<::size_t>* number) {
  entering->at(i) = true;

  number->at(i) = 1 + *std::max_element(number->begin(), number->end());

  entering->at(i) = false;

  for (::size_t j = 0; j < entering->size(); ++j) {
    // wait for thread j to enter the queue
    while (entering->at(j)) {}  // do nothing

    // process all threads at higher priority (or lower index) in queue
    while (number->at(j) != 0
           && (number->at(j) < number->at(i)
               || (number->at(j) == number->at(i) && j < i))) {}
             // do nothing
  }
}


void Unlock(::size_t i, std::vector<::size_t>* number) {
  number->at(i) = 0;
}


struct ThreadData {
  ThreadData(::pthread_t _id,
             ::size_t _index,
             std::vector<bool>* _entering,
             std::vector<::size_t>* _number,
             int* _sum) : id(_id),
                          index(_index),
                          entering(_entering),
                          number(_number),
                          sum(_sum) {  /* empty */ }

  ::pthread_t id;
  ::size_t index;
  std::vector<bool> *entering;
  std::vector<::size_t> *number;
  int *sum;
};


void* WorkerThread(void* arg) {
  ThreadData* argv = static_cast<ThreadData*>(arg);

  Lock(argv->index, argv->entering, argv->number);
  for (int i = 0; i < kCount_max; ++i)
    ++(*(argv->sum));
  Unlock(argv->index, argv->number);

  return nullptr;
}


int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <number_of_threads>" << std::endl;
    exit(-1);
  }

  std::vector<bool> entering;
  std::vector<::size_t> number;

  int sum = 0;

  std::cout << "Creating threads\n";
  std::vector<::ThreadData> threads;
  for (int i = 0; i < ::atoi(argv[1]); ++i) {
    entering.push_back(false);
    number.push_back(0);
    threads.push_back(::ThreadData(::pthread_t(),
                                   static_cast<::size_t>(i),
                                   &entering,
                                   &number,
                                   &sum));
  }
  std::cout << "Threads created" << std::endl;

  std::cout << "Current sum: " << sum << '\n';

  std::cout << "Starting threads\n";
  for (int i = 0; i < ::atoi(argv[1]); ++i)
    ::pthread_create(&threads[i].id, nullptr, WorkerThread, &threads[i]);
  std::cout << "Threads started" << '\n';

  for (::ThreadData& thread : threads)
    ::pthread_join(thread.id, nullptr);
  std::cout << "Threads completed" << '\n';

  std::cout << "Final sum, Expected:" << (kCount_max * ::atoi(argv[1]))
    << ", Actual: " << sum << std::endl;

  return 0;
}
