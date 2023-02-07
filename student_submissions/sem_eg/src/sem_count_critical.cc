// Copyright 2023 LittleCube
//

/*
 * Count to 2000000 with two threads
 * and synchronize them with a semaphore.
 * 
 * This is one of the simplest semaphore examples out there.
 * 
 */

#include <semaphore_wrapper.h>

#include <iostream>
#include <thread>  // NOLINT

void count_func(Semaphore* semaphore, int* count, const int kCountTo) {
  // enter critical section
  semaphore->Wait();

  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++*count;
  }

  std::cout << "Finished! Counted to " << *count << "." << std::endl;

  // leave critical section
  semaphore->Post();
}

int main(/* int argc, char* argv[] */) {
  // initialize b_count_func's semaphore
  Semaphore func_semaphore = Semaphore(1);

  // assign our goal
  int kCountTo = 2000000;

  // initialize count
  int count = 0;

  // start thread a
  std::thread a_thread = std::thread(count_func,
                                     &func_semaphore,
                                     &count,
                                     kCountTo);

  // start thread b
  std::thread b_thread = std::thread(count_func,
                                     &func_semaphore,
                                     &count,
                                     kCountTo);

  // wait for threads to finish before moving on
  a_thread.join();
  b_thread.join();

  std::cout << std::endl << "The final count is " << count << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
