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

#include <pthread.h>
#include <iostream>

// sorry doc, it's way easier to read
Semaphore b_semaphore;
int count;
int kCountTo;

void* a_count_func(void*) {
  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++count;
  }

  std::cout << "a is finished! Counted to " << count << "." << std::endl;

  // allow b_count to count the other half
  b_semaphore.Post();

  return nullptr;
}

void* b_count_func(void*) {
  // make sure we don't start counting before a finishes
  b_semaphore.Wait();

  // a has finished and we can begin counting
  for (int i = 0; i < kCountTo/2; ++i) {
    ++count;
  }

  std::cout << "b is finished! Counted to " << count << "." << std::endl;

  return nullptr;
}

int main(/* int argc, char* argv[] */) {
  // initialize b_count_func's semaphore
  b_semaphore = Semaphore(0);

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
