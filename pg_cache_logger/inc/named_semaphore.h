// Copyright 2022 CSCE 311
//
#ifndef PG_CACHE_LOGGER_INC_NAMED_SEMAPHORE_H_
#define PG_CACHE_LOGGER_INC_NAMED_SEMAPHORE_H_


#include <fcntl.h>  // POSIX open flags (O_CREAT, O_RD, O_RDWR, O_WR)
#include <semaphore.h>  // POSIX semaphore support
#include <sys/stat.h>  // POSIX stat func vals (S_I[R | W | X][GRP | USR | OTH])

#include <cerrno>  // errno
#include <cstdlib>  // exit(int) EXIT_FAILURE
#include <cstring>  // strerror(int)

#include <iostream>


namespace wrappers {


class NamedSemaphore {
 public:
  // name: a null-terminated string beginning with a forward slash (/)
  // count: starting value of semaphore
  NamedSemaphore(const char name[]) : name_(std::string(name)),
                                      sem_ptr_(nullptr) {
    // empty
  }
  NamedSemaphore(const std::string& name) : name_(name), sem_ptr_(nullptr) {
    // empty
  }


  void Create(int count);


  inline void Destroy() {
    ::sem_unlink(name_.c_str());
  }


  void Open();


  inline void Down() {
    ::sem_wait(sem_ptr_);
  }


  inline void Up() {
    ::sem_post(sem_ptr_);
  }


 private:
  std::string name_;
  ::sem_t* sem_ptr_;
};

}  // namespace wrappers


#endif  // PG_CACHE_LOGGER_INC_NAMED_SEMAPHORE_H_
