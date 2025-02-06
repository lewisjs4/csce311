// Copyright 2025 CSCE 311
//
#include <sem_eg/thread.h>

#include <unistd.h>
#include <iostream>
#include <string>

// shared data and synchronization primitives
static const std::string kGoal = "QT";
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// getter class
class Passenger : public Thread<Passenger> {
 public:
  Passenger(::pthread_t thread_id,
          ::size_t internal_id,
          const std::string* location)
      : Thread<Passenger>(thread_id, internal_id),
        times_woke_(0), location_(location) {
    // empty
  }

  static void* Execute(Passenger* passenger) {
  pthread_mutex_lock(&mutex);  // enter critical section

  while (*(passenger->location_) != kGoal) {
    if (passenger->times_woke_ == 0)
      std::cout << "Passenger: Wake me up when we get to QT..." << std::endl;
    else
      std::cout << "Passenger: I said QT!" << std::endl;

    pthread_cond_wait(&cond, &mutex);  // Wait for condition signal
    ++passenger->times_woke_;
  }

  // Once the goal is met, execute logic
  std::cout << "Great! I will be right back. Want anything?";

  pthread_mutex_unlock(&mutex);  // exit critical section
  return nullptr;
}
 private:
   std::size_t times_woke_;
   const std::string* location_;
};


// Setter Class
class Driver : public Thread<Driver> {
 public:
  Driver(::pthread_t thread_id,
         ::size_t internal_id,
         std::string* location)
      : Thread<Driver>(thread_id, internal_id), location_(location) {
    // empty
  }

  static void* Execute(Driver* driver) {
  while (true) {
    std::string input;
    
    std::cout << "What is my new location? ";
    std::getline(std::cin, input);

    pthread_mutex_lock(&mutex);  // Lock mutex
    *driver->location_ = input;        // Update shared data

    pthread_cond_signal(&cond);  // Notify passenger thread
    pthread_mutex_unlock(&mutex);  // Unlock mutex
  }
  return nullptr;
}
 private:
   std::string* location_;

};

int main() {
  std::string location;
  Driver setter(::pthread_t(), 0, &location);
  Passenger getter(::pthread_t(), 1, &location);

  // Create threads
  pthread_create(&getter.thread_id(),
                 nullptr,
                 Thread<Passenger>::Execute, &getter);
  pthread_create(&setter.thread_id(),
                 nullptr,
                 Thread<Driver>::Execute, &setter);

  // Join threads
  pthread_join(getter.thread_id(), nullptr);
  pthread_join(setter.thread_id(), nullptr);

  // Cleanup
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}
