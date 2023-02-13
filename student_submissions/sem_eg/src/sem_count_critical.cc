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
Semaphore func_semaphore;
int count;
int kCountTo;

void* count_func(void*) {
  // enter critical section
  func_semaphore.Wait();

  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++count;
  }

  std::cout << "Finished! Counted to " << count << "." << std::endl;

  // leave critical section
  func_semaphore.Post();

  return nullptr;
}

int main(/* int argc, char* argv[] */) {
  // initialize b_count_func's func_semaphore
  func_semaphore = Semaphore(1);

  // initialize count
  count = 0;

  // assign our goal
  kCountTo = 2000000;

  // start thread a
  ::pthread_t a_thread;
  ::pthread_create(&a_thread,
                   0,
                   &count_func,
                   nullptr);

  // start thread b
  ::pthread_t b_thread;
  ::pthread_create(&b_thread,
                   0,
                   &count_func,
                   nullptr);

  // wait for threads to finish before moving on
  ::pthread_join(a_thread, nullptr);
  ::pthread_join(b_thread, nullptr);

  std::cout << std::endl << "The final count is " << count << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
