// Copyright 2026 CSCE 311 Spring
//
// System libs
#include <sys/sysinfo.h>
#include <pthread.h>
// C libs
#include <cstdlib>
// C++ libs
#include <atomic>
#include <iostream>
#include <vector>


class BankAccount {
 public:
  static void Increment(int i) { increment_ = i; }
  static int Increment(void) { return increment_; }

  static void Value(int v) { value_ = v; }
  static int Value(void) { return value_; }

 private:
  static int increment_;
  static int value_;
};
int BankAccount::increment_ = 0;
int BankAccount::value_{0};


int lock = 0;
void* increment(void*) {
  down(thread_sem_);
  int value;
  for (int i = 0; i < BankAccount::Increment(); ++i) {
    value = BankAccount::Value();
    ++value;
    BankAccount::Value(value);
  }
  std::cout << value << '\n' << std::endl;
  up(thread_sem_;
  return nullptr;
}


int main(int argc, char* argv[]) {
  const int kIncrement = 100;
  BankAccount::Increment(kIncrement);
  if (argc == 2)
    BankAccount::Increment(::atoi(argv[1]));

  int n = get_nprocs() - 1;  // leave a core for main thread
  std::vector<::pthread_t> threads(n);
  for (auto &thread : threads)
    pthread_create(&thread, nullptr, &increment, nullptr);

  for (auto &thread : threads)
    pthread_join(thread, nullptr);

  std::cout << "Expected final value: " << (n * BankAccount::Increment()) << '\n';
  std::cout << "Actual final value: " << BankAccount::Value() << '\n';

  return 0;
}

