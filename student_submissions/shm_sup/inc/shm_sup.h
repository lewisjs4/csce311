// Copyright 2023 LittleCube
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define BUFFER_SIZE 0x400
#define SHMPATH "csvshmem"

#define SEM_SERVER "/sem_server"
#define SEM_CLIENT "/sem_client"

struct shmbuf {
    char buf[BUFFER_SIZE];
};

void quit();
