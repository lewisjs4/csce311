// Copyright 2023 LittleCube
//

#include <shm_sup.h>

#include <signal.h>

struct shmbuf* shmp;

int main(int argc, char** argv) {
    // happy signal time (properly cleanup on terminate)
    signal(SIGTERM, quit);
    signal(SIGINT, quit);

    // make sure semaphores do not already exist
    sem_unlink(SEM_SERVER);
    sem_unlink(SEM_CLIENT);

    // create previously nonexistent semaphores
    // sem_open returns -1 on fail, perhaps worth checking
    sem_t *sem1 = sem_open(SEM_SERVER, O_CREAT, 0660, 0);
    sem_t *sem2 = sem_open(SEM_CLIENT, O_CREAT, 0660, 0);

    // wait for client to open shared memory
    sem_wait(sem2);

    // so does shm_open
    int shmfd = shm_open(SHMPATH, O_RDWR, 0);

    // map shared memory
    // BUFFER_SIZE is defined in shm_sup.h
    shmp = mmap(NULL,
                sizeof(*shmp),
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                shmfd,
                0);

    char read_buffer[BUFFER_SIZE];

    // ready to read from client
    sem_post(sem1);

    // wait for client to finish writing
    sem_wait(sem2);

    // read string from shared memory
    snprintf(read_buffer, BUFFER_SIZE, "%s", shmp->buf);

    // print client string from read_buffer
    printf("%s", read_buffer);
}

// I'm a happy signal boy
void quit() {
    shm_unlink(SHMPATH);
    sem_unlink(SEM_SERVER);
    sem_unlink(SEM_CLIENT);
    exit(0);
}
