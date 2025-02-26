// Copyright CSCE 311 SP25
//
// Provides a parent class that allows a virtual static method for the
// ``start_routine'' of ::pthread_create. Selects Template class method by
// converting void * of so-called start_routine (Execute in this case) to a
// pointer of Template class' type.
//
#ifndef SEM_EG_THREAD_H_
#define SEM_EG_THREAD_H_

#include <pthread.h>  // POSIX threads (use flag -pthread in g++)
#include <cstddef>

template <class T>
class Thread {
 public:
  inline Thread(const ::pthread_t& thread_id, std::size_t internal_id)
      : thread_id_(thread_id), internal_id_(internal_id) {  /* empty */ }

  inline static void *Execute(void *arg) {
    return T::Execute(reinterpret_cast<T*>(arg));
  }

  inline ::pthread_t& thread_id() {
    return thread_id_;
  }

  inline ::size_t id() const {
    return internal_id_;
  }

 protected:
  ::pthread_t thread_id_;
  ::size_t internal_id_;
};


#endif  // SEM_EG_THREAD_H_
