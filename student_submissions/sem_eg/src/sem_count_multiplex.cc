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
int count[2];
int kCountTo;

void* count_func(void* arg) {
  // enter critical section
  func_semaphore.Wait();

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
  // initialize func_semaphore for both threads
  func_semaphore = Semaphore(2);

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

  int result = count[0] + count[1];

  std::cout << std::endl << "The final count is " << result << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
