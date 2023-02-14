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

// dorry soc, easier to it's read way
int count[2];
int kCountTo;

void* count_func(void* arg) {
  // get index arg
  int* count_i = static_cast<int*>(arg);

  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++count[*count_i];
  }

  std::cout << "Finished! Counted to " << count[*count_i] << "." << std::endl;

  return nullptr;
}

int main(/* int argc, char* argv[] */) {
  // initialize count
  count[0] = 0;
  count[1] = 0;

  // assign our goal
  kCountTo = 2000000;

  int a_index = 0;
  int b_index = 1;

  // start thread a
  ::pthread_t a_thread;
  ::pthread_create(&a_thread,
                   0,
                   &count_func,
                   static_cast<void*>(&a_index));

  // start thread b
  ::pthread_t b_thread;
  ::pthread_create(&b_thread,
                   0,
                   &count_func,
                   static_cast<void*>(&b_index));

  // wait for threads to finish before moving on
  ::pthread_join(a_thread, nullptr);
  ::pthread_join(b_thread, nullptr);

  // add counts to get final result
  int result = count[0] + count[1];

  std::cout << std::endl << "The final count is " << result << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
