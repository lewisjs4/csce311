// Copyright 2023 LittleCube
//

/*
 * This is the broken version of the count to 2000000 program.
 * 
 * This code does not handle the race condition.
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
  int count = 0;

  // start thread a
  std::thread a_thread = std::thread(a_count_func,
                                     &count,
                                     kCountTo);

  // start thread b
  std::thread b_thread = std::thread(b_count_func,
                                     &count,
                                     kCountTo);

  // wait for threads to finish before moving on
  a_thread.join();
  b_thread.join();

  std::cout << std::endl << "The final count is " << count << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
