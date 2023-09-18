// Copyright 2023 CSCE 311
//
#ifndef CONSUMER_PRODUCER_THREAD_H_
#define CONSUMER_PRODUCER_THREAD_H_

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)


template <class T>
class Thread {
 public:
  inline Thread(const ::pthread_t& thread_id, ::size_t internal_id)
      : thread_id_(thread_id), internal_id_(internal_id) {  /* empty */ }

  inline static void *Execute(void *arg) {
    return T::Execute(arg);
  }

  inline ::pthread_t& thread_id() {
    return thread_id_;
  }

  inline ::size_t id() {
    return internal_id_;
  }

 private:
  ::pthread_t thread_id_;
  ::size_t internal_id_;
};


#endif  // CONSUMER_PRODUCER_THREAD_H_
