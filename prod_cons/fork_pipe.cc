// Copyright 2022 CSCE 311
//


#include <unistd.h>  // Unix standard header, using close, fork, pipe

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // abort
#include <cstring>  // using strerror

#include <algorithm>
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
      std::cerr << ::strerror(errno) << std::endl;

      abort();
    }
  }


  // Closes the file descriptor used
  //
  ~ForkPipe() {
    int success = ::close(file_desc_[file_desc_t_]);  // destructors never
                                                      //   forget to close
                                                      //   unused pipes

    if (success < 0)
      std::cerr << ::strerror(errno) << std::endl;
  }


  // Reads pipe n bytes at a time
  //
  const char* Read() const {
    assert(file_desc_t_ == 0);

    // entire message (may need multiple reads)
    const size_t kMsg_init_size = 16;  // 16 chars
    char *message = nullptr;
    size_t msg_size, msg_end = 0;

    // buffer for each read
    const size_t kBuff_size = 8;  // read eight bytes each time
    char buffer[kBuff_size];

    // read from file_desc_ into buffer
    size_t bytes_read = ::read(file_desc_[file_desc_t_], buffer, kBuff_size);
    while (bytes_read > 0) {
      if (!message) {
        // init message if necessary
        msg_size = kMsg_init_size > bytes_read ? kMsg_init_size : bytes_read;
        message = new char[msg_size];
      } else if (msg_end + bytes_read >= msg_size) {
        // ensure message can contain entire buffer, if exits
        msg_size = msg_end + bytes_read + 1;
        char *tmp = new char[msg_size];

        std::copy(message, message + msg_size, tmp);

        delete message;
        message = tmp;
      }

      // move buffer into message
      std::copy(buffer, buffer + bytes_read, message + msg_end);
      msg_end += bytes_read;

      // check for more bytes
      bytes_read = ::read(file_desc_[file_desc_t_], buffer, kBuff_size);
    }

    return message;
  }


  // Writes entire message to pipe at once
  //
  // message is a cstring (null terminated char array)
  //
  const char* Write(const char* message) const {
    assert(file_desc_t_ == 1);
    const int msg_len = ::strlen(message);
    if (::write(file_desc_[file_desc_t_], message, msg_len) == msg_len)
      return nullptr;
    else
      return ::strerror(errno);
  }


  const char* CloseReader() {
    file_desc_t_ = 1;
    return CloseFileDescriptor(0);
  }


  const char* CloseWriter() {
    file_desc_t_ = 0;
    return CloseFileDescriptor(1);
  }

 private:
  const char* CloseFileDescriptor(int descriptor) {
    int success = ::close(file_desc_[descriptor]);

    if (success == 0)
      return nullptr;
    else
      return ::strerror(errno);
  }


  // file decriptors, 0 : read end of queue, 1 : write end
  int file_desc_[2];  // always size two for two ends of queue

  int file_desc_t_;  // stores the type of descriptor (0 or 1)
};


int main(int argc, char* argv[]) {
  ForkPipe pipe;

  int child_pid = ::fork();

  if (child_pid > 0) {  // parent
    pipe.CloseWriter();
    std::cout << "Parent says:\tChild sent: \"" << pipe.Read() << "\"\n";
    std::cout << "Parent says:\tMessage received, exiting." << std::endl;
  } else if (child_pid == 0) {  // child
    pipe.CloseReader();
    pipe.Write("Hello from your child!");
    std::cout << "Child says:\tMessage sent, exiting." << std::endl;
  } else {
      std::cerr << "Fork failed with error, \"" << ::strerror(errno) << "\"\n";
      return -1;
  }

  return 0;
}
