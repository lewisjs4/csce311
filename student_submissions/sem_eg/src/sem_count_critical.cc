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

struct args_struct {
  args_struct();
  Semaphore* semaphore_;
  int* count_;
  int kCountTo_;
};

args_struct::args_struct() {
  // empty
}

void* count_func(void* args_param) {
  struct args_struct* args = reinterpret_cast<struct args_struct*>(args_param);
  Semaphore* semaphore = args->semaphore_;
  int* count = args->count_;
  int kCountTo = args->kCountTo_;

  // enter critical section
  semaphore->Wait();

  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++*count;
  }

  std::cout << "Finished! Counted to " << *count << "." << std::endl;

  // leave critical section
  semaphore->Post();

  return nullptr;
}

int main(/* int argc, char* argv[] */) {
  // initialize b_count_func's semaphore
  Semaphore func_semaphore = Semaphore(1);

  // assign our goal
  int kCountTo = 2000000;

  // initialize count
  int count = 0;

  struct args_struct args;
  args.semaphore_ = &func_semaphore;
  args.count_ = &count;
  args.kCountTo_ = kCountTo;

  void* args_void = reinterpret_cast<void*>(&args);

  // start thread a
  ::pthread_t a_thread;
  ::pthread_create(&a_thread,
                   0,
                   &count_func,
                   args_void);

  // start thread b
  ::pthread_t b_thread;
  ::pthread_create(&b_thread,
                   0,
                   &count_func,
                   args_void);

  // wait for threads to finish before moving on
  ::pthread_join(a_thread, nullptr);
  ::pthread_join(b_thread, nullptr);

  std::cout << std::endl << "The final count is " << count << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
