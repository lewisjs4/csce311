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

struct args_struct {
  args_struct();
  int* count_;
  int kCountTo_;
};

args_struct::args_struct() {
  // empty
}

void* count_func(void* args_param) {
  struct args_struct* args = reinterpret_cast<struct args_struct*>(args_param);
  int* count = args->count_;
  int kCountTo = args->kCountTo_;

  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++*count;
  }

  std::cout << "Finished! Counted to " << *count << "." << std::endl;

  return nullptr;
}

int main(/* int argc, char* argv[] */) {
  // assign our goal
  int kCountTo = 2000000;

  // initialize counts
  int a_count = 0;
  int b_count = 0;

  struct args_struct a_args;
  a_args.count_ = &a_count;
  a_args.kCountTo_ = kCountTo;

  struct args_struct b_args;
  b_args.count_ = &b_count;
  b_args.kCountTo_ = kCountTo;

  void* a_args_void = reinterpret_cast<void*>(&a_args);
  void* b_args_void = reinterpret_cast<void*>(&b_args);

  // start thread a
  ::pthread_t a_thread;
  ::pthread_create(&a_thread,
                   0,
                   &count_func,
                   a_args_void);

  // start thread b
  ::pthread_t b_thread;
  ::pthread_create(&b_thread,
                   0,
                   &count_func,
                   b_args_void);

  // wait for threads to finish before moving on
  ::pthread_join(a_thread, nullptr);
  ::pthread_join(b_thread, nullptr);

  // add counts to get final result
  int count = a_count + b_count;

  std::cout << std::endl << "The final count is " << count << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
