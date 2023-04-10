// Copyright 2023 LittleCube
//

#include <shm_sup.h>

struct shmbuf* shmp;

int main(int argc, char** argv) {
    // make sure shared memory does not already exist
    shm_unlink(SHMPATH);

    // open existing semaphores located on server
    sem_t *sem1 = sem_open(SEM_SERVER, 0);
    sem_t *sem2 = sem_open(SEM_CLIENT, 0);

    // create previously uninstantiated shared memory
    int shmfd = shm_open(SHMPATH, O_CREAT | O_EXCL | O_RDWR,
                                  S_IRUSR | S_IWUSR);

    // Truncate the memory (VERY IMPORTANT, will get a Bus Error otherwise)
    ftruncate(shmfd, sizeof(struct shmbuf));

    // map shared memory
    shmp = mmap(NULL,
                sizeof(*shmp),
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                shmfd,
                0);

    if (shmp == MAP_FAILED) {
        fprintf(stderr, "error mapping memory\n");
        return -1;
    }

    while (sem1 == 0) {}

    // notify server that shared memory is created
    sem_post(sem2);

    // prepare string to send
    char sup_string[] = "sup\n";

    // wait for server to be ready to read
    sem_wait(sem1);

    // load the string into shared memory
    snprintf(shmp->buf, BUFFER_SIZE, "%s", sup_string);

    // notify server that string is ready to read
    sem_post(sem2);
}
