#include <wrappers/inc/named_semaphore.h>

#include <iostream>

int main(int argc, char* argv[]) {
  wrappers::NamedSemaphore sem(argv[1]);
  sem.Create(1);
  sem.Open();
  sem.Up();
  int tmp;
  std::cin >> tmp;
  sem.Down();
  sem.Destroy();
  return 0;
}
