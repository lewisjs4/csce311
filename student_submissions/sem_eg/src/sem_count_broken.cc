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

struct args_struct {
  args_struct();
  int* count_;
  int kCountTo_;
};

args_struct::args_struct() {
  // empty
}

void* a_count_func(void* args_param) {
  struct args_struct* args = reinterpret_cast<struct args_struct*>(args_param);
  int* count = args->count_;
  int kCountTo = args->kCountTo_;

  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++*count;
  }

  std::cout << "a is finished! Counted to " << *count << "." << std::endl;

  return nullptr;
}

void* b_count_func(void* args_param) {
  struct args_struct* args = reinterpret_cast<struct args_struct*>(args_param);
  int* count = args->count_;
  int kCountTo = args->kCountTo_;

  // count to the other half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++*count;
  }

  std::cout << "b is finished! Counted to " << *count << "." << std::endl;

  return nullptr;
}

int main(/* int argc, char* argv[] */) {
  // assign our goal
  int kCountTo = 2000000;

  // initialize count
  int count = 0;

  struct args_struct args;
  args.count_ = &count;
  args.kCountTo_ = kCountTo;

  void* args_void = reinterpret_cast<void*>(&args);

  // start thread a
  ::pthread_t a_thread;
  ::pthread_create(&a_thread,
                   0,
                   &a_count_func,
                   args_void);

  // start thread b
  ::pthread_t b_thread;
  ::pthread_create(&b_thread,
                   0,
                   &b_count_func,
                   args_void);

  // wait for threads to finish before moving on
  ::pthread_join(a_thread, nullptr);
  ::pthread_join(b_thread, nullptr);

  std::cout << std::endl << "The final count is " << count << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
