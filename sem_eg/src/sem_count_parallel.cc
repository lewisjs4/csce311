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

#include <iostream>
#include <thread>  // NOLINT

void a_count_func(int* count, const int kCountTo) {
  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++*count;
  }

  std::cout << "a is finished! Counted to " << *count << "." << std::endl;
}

void b_count_func(int* count, const int kCountTo) {
  // count to the other half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++*count;
  }

  std::cout << "b is finished! Counted to " << *count << "." << std::endl;
}

int main(/* int argc, char* argv[] */) {
  // assign our goal
  int kCountTo = 2000000;

  // initialize counts
  int a_count = 0;
  int b_count = 0;

  // start thread a
  std::thread a_thread = std::thread(a_count_func,
                                     &a_count,
                                     kCountTo);

  // start thread b
  std::thread b_thread = std::thread(b_count_func,
                                     &b_count,
                                     kCountTo);

  // wait for threads to finish before moving on
  a_thread.join();
  b_thread.join();

  // add counts to get final result
  int count = a_count + b_count;

  std::cout << std::endl << "The final count is " << count << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
