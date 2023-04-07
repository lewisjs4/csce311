# csce311

## forking
  - fork.cc
  - fork\_ls.cc
  - makefile

## ipc
  - fork\_pipe.cc
  - fifo\_manager.cc
  - socket\_cli\_srv.cc
  - makefile

## busy\_waiting
  - thread.cc
  - thread\_generator.cc
  - race\_condition.cc
  - lamports\_algo.cc
  - makefile

## sem_eg
  - mutex.cc
  - multiplex.cc
  - signal.cc
  - makefile

## shm_logger
  - makefile
    - logger : the server proc which writes to the log file
    - ex-client : an example client proc; accepts keyboard input, then sends to
      the server to log
    - clean : deletes .o files
  - named_semaphore.cc/h : a wrapper class around the C semaphore; simplifies
    semaphore usage between processes
  - shm_store.h : a set of structs to map into the shared memory; provides
    example of constexpr and value template types
  - consumer.cc/h : class providing the logger server
  - producer.cc/h : class providing the logger client
  - logger.cc : main entry point for logger server
  - ex_client.cc : example client; provides main entry point for Producer class

## producer_consumer
  - thread.h
  - thread_sem_manager.h
  - thread_sem_manager.cc
  - producer_consumer.cc
  - makefile

## deadlock
  - bankers.py
  - bankers.txt

## student_submissions
  - sem_eg
    - src
      - semaphore_wrapper.cc
      - sem_count_broken.cc
      - sem_count.cc
      - sem_count_critical.cc
      - sem_count_multiplex.cc
      - sem_count_parallel.cc
    - inc
      - semaphore_wrapper.h
    - makefile
  - shm_sup
    - src
      - shm_sup_server.c
      - shm_sup_client.c
    - inc
      - shm_sup.h
    - makefile

