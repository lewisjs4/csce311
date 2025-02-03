// Copyright 2024 CSCE 311
//

#include <unistd.h>
// using ::fork
#include <iostream>


int main(int argc, char* argv[]) {
  std::cout << "parent " << ::getpid() << std::endl;

  switch (::fork()) {  // first call to fork
    case 0:  // I woke up to find I am the child
      std::cout << "child pid " << ::getpid() << std::endl;
      std::cout << "child's parent's pid " << ::getppid() << std::endl;

      switch (::fork()) {
        case 0:  // I woke up to find I am the grandchild
          std::cout << "grandchild pid " << ::getpid() << std::endl;
          std::cout << "grandchild's parent's pid " << ::getppid()
            << std::endl;
          std::cout << "grandchild quitting" << std::endl;
          return 0;

        case -1:
          std::cout << "grandchild failure" << std::endl;
          return -1;

        default:  // I woke up to find I am the child
          std::cout << "grandchild spawned" << std::endl;
      }

      std::cout << "child quitting" << std::endl;
      return 0;

    case -1:
      std::cout << "child failure" << std::endl;
      return -1;

    default:  // I woke up to find I am the parent
      std::cout << "child spawned" << std::endl;
  }

  std::cout << "parent quitting" << std::endl;
  return 0;
}
