// Copyright 2021 CSCE 240
//

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <semaphore.h>  // POSIX semaphore support
#include <unistd.h>  // UNIX standard header (sleep)

#include <cstddef>  // size_t
#include <cstdlib>  // atoi

#include <iostream>
#include <vector>


class ThreadSemaphoreManager {
 public:
  static size_t Create(size_t count = 0) {
    mutexes.push_back(sem_t());
    sem_init(&(mutexes.back()), 0, count);

    return mutexes.size() - 1;
  }

  static void Destroy() {
    for (sem_t& mutex : mutexes)
      sem_destroy(&mutex);
  }

  static void Down(size_t index=0) {
    sem_wait(&(mutexes.at(index)));
  }

  static void Up(size_t index=0) {
    sem_post(&(mutexes.at(index)));
  }
 private:
  static std::vector<sem_t> mutexes;
};
std::vector<sem_t> ThreadSemaphoreManager::mutexes = std::vector<sem_t>();


void *print_message_function(void *ptr);


int main(int argc, char* argv[]) {
  ThreadSemaphoreManager::Create(1);  // binary semaphore
  pthread_t thread1, thread2;
  int  iret1, iret2;

  std::string message1 = "Thread 1";
  std::string message2 = "Thread 2";

  // independent threads compete for print space

  iret1 = pthread_create(&thread1,
                         nullptr,  // restrictions (read on your own)
                         print_message_function,  // pointer to function
                         static_cast<void *> (&message1));
  iret2 = pthread_create(&thread2,
                         nullptr,
                         print_message_function,
                         static_cast<void *> (&message2));

  pthread_join(thread1, nullptr);
  pthread_join(thread2, nullptr); 

  std::cout << "Thread 1 returns: " << iret1 << std::endl;
  std::cout << "Thread 2 returns: " << iret2 << std::endl;

  ThreadSemaphoreManager::Destroy();
  return 0; 
}


void *print_message_function(void *ptr) {
  auto *message = static_cast<std::string *>(ptr);

  ThreadSemaphoreManager::Down();
  for (size_t i = 0; i < 10; ++i) {
    std::cout << *message << ' ';
    usleep(1);  // wait 1ms
  }

  std::cout << std::endl;
  ThreadSemaphoreManager::Up();

  return nullptr;
}
