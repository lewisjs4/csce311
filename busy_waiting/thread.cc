// Copyright 2023 CSCE 311
//

#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>


void* WorkerThread(void* arg) {
  std::cout << static_cast<char*>(arg) << " doing work!" << std::endl;

  return nullptr;
}


int main(int argc, char* argv[]) {
  ::pthread_t p0, p1;
  char p_name0[3] = "p0", p_name1[3] = "p1";

  std::cout << "Creating threads\n";

  ::pthread_create(&p0, nullptr, WorkerThread, p_name0);
  ::pthread_create(&p1, nullptr, WorkerThread, p_name1);

  ::pthread_join(p0, nullptr);
  ::pthread_join(p1, nullptr);

  std::cout << "Threads complete" << std::endl;

	return 0;
}
