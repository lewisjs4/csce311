// Copyright 2023 LittleCube
//

/*
 * Sometimes, semaphores are not necessary. This problem can be
 * solved simply by separating the variables, removing the dependence
 * the functions previously had on each other.
 * 
 * This is not always possible. Sometimes dependencies cannot be removed.
 * 
 */

#include <pthread.h>
#include <iostream>

int kCountTo;

void* count_func(void* arg) {
  int* count = static_cast<int*>(arg);

  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++*count;
  }

  std::cout << "Finished! Counted to " << *count << "." << std::endl;

  return nullptr;
}

int main(/* int argc, char* argv[] */) {
  // initialize counts
  int a_count = 0;
  int b_count = 0;

  // assign our goal
  kCountTo = 2000000;

  // start thread a
  ::pthread_t a_thread;
  ::pthread_create(&a_thread,
                   0,
                   &count_func,
                   static_cast<void*>(&a_count));

  // start thread b
  ::pthread_t b_thread;
  ::pthread_create(&b_thread,
                   0,
                   &count_func,
                   static_cast<void*>(&b_count));

  // wait for threads to finish before moving on
  ::pthread_join(a_thread, nullptr);
  ::pthread_join(b_thread, nullptr);

  // add counts to get final result
  int count = a_count + b_count;

  std::cout << std::endl << "The final count is " << count << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
