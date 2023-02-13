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

// sorry doc, it's way easier to read
int count;
int kCountTo;

void* a_count_func(void*) {
  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++count;
  }

  std::cout << "a is finished! Counted to " << count << "." << std::endl;

  return nullptr;
}

void* b_count_func(void*) {
  // count to the other half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++count;
  }

  std::cout << "b is finished! Counted to " << count << "." << std::endl;

  return nullptr;
}

int main(/* int argc, char* argv[] */) {
  // initialize count
  count = 0;

  // assign our goal
  kCountTo = 2000000;

  // start thread a
  ::pthread_t a_thread;
  ::pthread_create(&a_thread,
                   0,
                   &a_count_func,
                   nullptr);

  // start thread b
  ::pthread_t b_thread;
  ::pthread_create(&b_thread,
                   0,
                   &b_count_func,
                   nullptr);

  // wait for threads to finish before moving on
  ::pthread_join(a_thread, nullptr);
  ::pthread_join(b_thread, nullptr);

  std::cout << std::endl << "The final count is " << count << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
