// Copyright CSCE 311 SP25
//

#include <sem_eg/thread_sem_manager.h>

#include <pthread.h>
#include <unistd.h>

#include <iostream>


// Monitor class that executes a given function inside a locked section
class Monitor {
public:
  Monitor();
  ~Monitor();
  void execute(void* (*task)(void*));
};

// Shared monitor instance
Monitor monitor;

// Example function executed within the monitor
//   This is the actual job for the task
void* task(void*) {
  std::cout << "Thread " << pthread_self() << " starting work.\n";
  sleep(1);  // Simulate work
  std::cout << "Thread " << pthread_self() << " finishing work.\n";
  return nullptr;
}

// Thread function that passes its task to the monitor
void* thread_function(void*) {
  monitor.execute(task);  // The thread does not deal with locking, just passes the function
  return nullptr;
}

int main() {
  monitor = Monitor();
  pthread_t t1, t2;

  // Create two threads that execute their tasks via the monitor
  pthread_create(&t1, nullptr, thread_function, nullptr);
  pthread_create(&t2, nullptr, thread_function, nullptr);

  // Wait for threads to finish
  pthread_join(t1, nullptr);
  pthread_join(t2, nullptr);

  return 0;
}

// Monitor constructor (initialize mutex)
Monitor::Monitor() {
  ThreadSemaphoreManager::Create(1);  // create an "open" binary semaphore
}

// Monitor destructor (destroy mutex)
Monitor::~Monitor() {
  ThreadSemaphoreManager::Destroy();
}

// Execute function inside monitor
void Monitor::execute(void* (*task)(void*)) {
  ThreadSemaphoreManager::Down();  // Lock the monitor
  task(nullptr);  // Execute the provided function
  ThreadSemaphoreManager::Up();  // Unlock the monitor
}

