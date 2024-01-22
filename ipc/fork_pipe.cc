// Copyright 2023 CSCE 311
//


#include <unistd.h>  // (Unix standard header) ::close, ::fork, ::pipe

#include <cassert>  // assert macro
#include <cstddef>  // std::size_t
#include <cstdlib>  // std::abort
#include <cstring>  // std::strerror

#include <algorithm>  // std::copy
#include <iostream>


// Class provides C++ interface to a pipe used for fork-ed proc IPC.
//
class ForkPipe {
 public:
  // Initializes pipe
  //
  // - Precondition: must be called before fork which will use pipe
  //
  ForkPipe() {
    int success = ::pipe(file_desc_);  // pipe function uses pass-by-ref to
                                       //   return file descriptor parameter;
                                       //   it is an output parameter)
    if (success < 0) {
      std::cerr << std::strerror(errno) << std::endl;

      std::abort();
    }
  }


  // Closes the file descriptor used
  //
  ~ForkPipe() {
    int success = ::close(file_desc_[file_desc_t_]);  // destructors never
                                                      //   forget to close
                                                      //   unused pipes

    if (success < 0)
      std::cerr << std::strerror(errno) << std::endl;
  }


  // Reads pipe n bytes at a time
  //
  const char* Read() const {
    const std::size_t kMsg_init_size = 8;  // two bytes
    const std::size_t kBuff_size = 8;  // read eight bytes each time

    assert(file_desc_t_ == 0);

    // entire message (may need multiple reads)
    std::size_t msg_size = kMsg_init_size, msg_end = 0;
    char *message = new char[msg_size];

    // buffer for each read
    char buffer[kBuff_size];

    // read from file_desc_ into buffer
    std::size_t bytes_read = ::read(file_desc_[file_desc_t_], buffer, kBuff_size);
    while (bytes_read > 0) {
      std::cout << "ForkPipe::Read:\t\t" << bytes_read << " bytes read.\n";
      if (msg_end + bytes_read > msg_size - 1) {
        // too small, request more memory
        char *tmp = new char[msg_size << 1];

        // deep copy into new memory
        std::copy(message, message + msg_size, tmp);

        // update message
        delete [] message;
        message = tmp;
        msg_size <<= 1;
      }

      // move buffer into message
      std::copy(buffer, buffer + bytes_read, message + msg_end);
      msg_end += bytes_read;

      // check for more bytes
      bytes_read = ::read(file_desc_[file_desc_t_], buffer, kBuff_size);
    }

    return const_cast<const char*>(message);
  }


  // Writes entire message to pipe at once
  //
  // message is a cstring (null terminated char array)
  //
  const char* Write(const char* message) const {
    assert(file_desc_t_ == 1);
    int msg_len = ::strlen(message) + 1;  // include '\0'
    std::cout << "ForkPipe::Write:\t" << msg_len << " bytes sent.\n";
    if (::write(file_desc_[file_desc_t_], message, msg_len) == msg_len)
      return nullptr;
    else
      return std::strerror(errno);
  }


  const char* SetWriter() {
    file_desc_t_ = 1;
    return CloseFileDescriptor(0);
  }


  const char* SetReader() {
    file_desc_t_ = 0;
    return CloseFileDescriptor(1);
  }

 private:
  const char* CloseFileDescriptor(int descriptor) {
    if (::close(file_desc_[descriptor]) == 0)
      return nullptr;
    else
      return std::strerror(errno);
  }


  // file decriptors, 0 : read end of queue, 1 : write end
  int file_desc_[2];  // always size two for two ends of queue

  int file_desc_t_;  // stores the type of descriptor (0 or 1)
};


void PrintCString(std::ostream* cout, const char* c_string) {
  *cout << "{ ";
  for (std::size_t i = 0; c_string[i]; ++i)
    *cout << c_string[i] << ", ";
  *cout << "\\0 }";
}


int main(int argc, char* argv[]) {
  ForkPipe pipe;

  int child_pid = ::fork();

  if (child_pid > 0) {  // parent
    std::cout << "Parent:\t\t\tSetting pipe and listening for child.\n";
    pipe.SetReader();
    const char *message = pipe.Read();

    std::cout << "Parent:\t\t\tChild sent: \"" << message << "\"\n";
    delete [] message;
    
    std::cout << "Parent:\t\t\tMessage received, exiting." << std::endl;


  } else if (child_pid == 0) {  // child
    pipe.SetWriter();
    std::cout << "Sending:\n\t";
    PrintCString(&std::cout, "Hello from your child!");
    std::cout << '\n';
    pipe.Write("Hello from your child!");
    std::cout << "Child:\t\t\tMessage sent, exiting." << std::endl;
  } else {
      std::cerr << "Fork failed with error, \"" << std::strerror(errno) << "\"\n";
      return -1;
  }

  return 0;
}
