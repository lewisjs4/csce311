// Copyright 2022 CSCE 311
//

#include <sys/stat.h>  // Unix header for stat, using mkfifo
#include <sys/types.h>  // Unix header for types, using mode_t
#include <fcntl.h>  // Unix file control header, using open, O_RDONLY, O_WRONLY
#include <unistd.h>  // Unix standard header, using close, fork, pipe

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // abort, atoi
#include <cstring>  // using strcmp, strcpy, strerror, strlen

#include <algorithm>
#include <filesystem>
#include <iostream>


// Class provides C++ pipe management
//
class FifoManager {
 public:
  // Saves path to FIFO
  //
  FifoManager(const char *fifo_path) : fifo_path_(nullptr),
                                       file_desc_(0),
                                       file_mode_(0) {
    fifo_path_ = new char[::strlen(fifo_path)];
    ::strcpy(fifo_path_, fifo_path);
  }


  //  Closes pipe if open, emits error message if close fails
  //
  ~FifoManager() {
    if (file_desc_) {
      int success = close(file_desc_);
      if (fifo_path_)
        delete [] fifo_path_;

      if (success < 0) {
        std::cerr << ::strerror(errno) << std::endl;

        abort();
      }
    }
  }


  // Builds FIFO if one does not exist
  //
  // - Precondition: error string is "large enough" to hold error message if
  //                 not nullptr
  // - Notes: idempotent
  bool Init(char* error = nullptr) {
    int success = 0;
    if (!std::filesystem::is_fifo(fifo_path_))  // ensure FIFO does not already
                                                //   exist
      success = mkfifo(fifo_path_, 0600);  // give read and write permission to
                                           //   creating user

    if (success == 0)
      return true;
    
    if (error)
      ::strcpy(error, ::strerror(errno));

    return false;
  }


  // Ensures FIFO exists and opens for read-only
  //
  // Note: if file descriptor is open, file will be closed and opened
  //
  bool OpenRead(char* error = nullptr) {
    file_mode_ = O_RDONLY;

    return Open(error);
  }


  bool Read(char *buffer, size_t size) {
    assert(file_mode_ == O_RDONLY);

    // read from file_desc into buffer
    size_t bytes_read = ::read(file_desc_, buffer, size);
    buffer[bytes_read] = '\0';

    return bytes_read > 0;
  }


  // Ensures FIFO exists and opens for write-only
  //
  // Note: if file descriptor is open, file will be closed and opened
  //
  bool OpenWrite(char* error = nullptr) {
    file_mode_ = O_WRONLY;

    return Open(error);
  }


  char* Write(char* buffer, size_t buffer_size) {
    assert(file_mode_ == O_WRONLY);

    if (::write(file_desc_, buffer, buffer_size)
        == static_cast<int>(buffer_size))
      return nullptr;
    else
      return ::strerror(errno);
  }


 private:
  // Ensures FIFO exsits
  bool Open(char* error = nullptr) {
    if (Init(error)) {
      if (file_desc_)
        ::close(file_desc_);

      file_desc_ = ::open(fifo_path_, file_mode_);

      if (file_desc_ < 0) {
        if (error)
          ::strcpy(error, ::strerror(errno));

        return false;
      }
    }

    return true;
  }


  char* fifo_path_;  // stores path to pipe file

  int file_desc_;  // stores pipe file descriptor
  int file_mode_;  // stores pipe file mode (O_RDONLY, O_WRONLY)
};


int main(int argc, char* argv[]) {
  int a;
  (void) a;
  if (argc < 2)
    return -1;

  FifoManager manager("/tmp/fifo-manager");

  char *err_msg = new char[16];  // for errs returned by manager

  int mode = atoi(argv[1]);
  if (mode == 0) {
    // reader
    if (!manager.OpenRead(err_msg)) {
      std::cerr << err_msg << std::endl;
      delete []err_msg;

      return 10;
    }

    const size_t kRead_buffer_size = 32;
    char read_buffer[kRead_buffer_size];
    while (true) {
      if (manager.Read(read_buffer, kRead_buffer_size))
        std::cout << read_buffer << std::endl;
    }
  } else if (mode == 1) {
    // writer
    if (!manager.OpenWrite(err_msg)) {
      std::cerr << err_msg << std::endl;
      delete []err_msg;

      return 20;
    }

    const size_t kWrite_buffer_size = 32;
    char write_buffer[kWrite_buffer_size];  // one byte with null terminator
    while (true) {
      std::cin.getline(write_buffer, kWrite_buffer_size);

      err_msg = manager.Write(write_buffer, kWrite_buffer_size);
      if (err_msg) {
        std::cerr << err_msg << std::endl;
        delete []err_msg;

        return 21;
      }
    }
  }

  delete []err_msg;
  return 0;
}
