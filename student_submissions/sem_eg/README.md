# student_submissions/sem_eg

This folder contains all student-submitted semaphore examples for CSCE 311. Each one is an attempt to count to 2,000,000, but each does so in a different way. Listed below are each file and their descriptions.

### sem_count_broken

An approach using threads without a semaphore that does not work, it contains a race condition.

### sem_count

An approach using threads and a semaphore that works, it contains no race conditions. It works by counting to 1,000,000 using one thread first, while the other waits using the semaphore. After the first thread finishes, it signals to the second thread by posting with the semaphore, and the second thread counts to the next 1,000,000, resulting in 2,000,000 being counted.

### sem_count_critical

An approach using threads and a semaphore that works, it contains no race conditions, but is different from sem_count because it uses the semaphore more as a mutex instead of just a signal.

### sem_count_multiplex

An approach using threads and a semaphore that works, it contains no race conditions, but is different from sem_count because it uses the semaphore as a multiplex instead of a mutex signal. It initializes both threads at once, but gives them different indices of an int array to count in parallel, and adds their results later.

### sem_count_parallel

An approach using threads without a semaphore that works, it contains no race conditions. It works by counting to 1,000,000 using two different int variables and later adding them together once both threads have joined.