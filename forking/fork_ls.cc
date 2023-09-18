// Copyright 2023 CSCE 311
//

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>


int main(int argc, char* argv[]) {
  std::cout << "Proc " << ::getpid() << " forking" << std::endl;

  switch (::fork()) {
    case 0:  // executing as child proc
      std::cout << "Proc " << ::getpid() << " executing ls" << std::endl;

      // replace copy of parent proc (./fork_ls) with /bin/ls proc
      ::execl("/bin/ls", "ls", "-all", nullptr);

      // no break needed, proc is replaced in memory by execl

    case -1:
      std::cout << "ERROR: fork FAILED (returned -1)" << std::endl;

      return -1;  // pass error state up

    default:  // executing as parent proc
      wait(nullptr);  // reap child proc

      std::cout << "Proc " << ::getpid() << " terminating" << std::endl;
  }

  return 0;
}
