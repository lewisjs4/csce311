// Copyright 2025 CSCE 311
//
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string>

// Shared data and synchronization primitives
static std::string shared_value;
static const std::string kGoal = "run";
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// **Checker Thread Function**
void* CheckerThread(void* arg) {
  pthread_mutex_lock(&mutex);  // Lock mutex

  while (shared_value != kGoal) {  
    std::cout << "Checker waiting for input...\n";
    pthread_cond_wait(&cond, &mutex);  // Wait for condition signal
  }

  // Once the goal is met, execute logic
  std::cout << "Checker thread detected '" << kGoal << "'. Executing task!\n";

  pthread_mutex_unlock(&mutex);  // Unlock mutex
  return nullptr;
}

// **Setter Thread Function**
void* SetterThread(void* arg) {
  while (true) {
    std::string input;
    
    std::cout << "Enter a command: ";
    std::cin >> input;

    pthread_mutex_lock(&mutex);  // Lock mutex
    shared_value = input;        // Update shared data

    pthread_cond_signal(&cond);  // Notify checker thread
    pthread_mutex_unlock(&mutex);  // Unlock mutex

    if (input == kGoal) break;  // Exit when "run" is entered
  }
  return nullptr;
}

int main() {
  pthread_t setter, checker;

  // Create threads
  pthread_create(&checker, nullptr, CheckerThread, nullptr);
  pthread_create(&setter, nullptr, SetterThread, nullptr);

  // Join threads
  pthread_join(setter, nullptr);
  pthread_join(checker, nullptr);

  // Cleanup
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}
