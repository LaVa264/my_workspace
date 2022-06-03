#include <stdio.h>
#include <fcntl.h>    /* Defines O_* constants */
#include <sys/stat.h> /* Defines mode constants */
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

const char *shared_object_name = "Larva_Server";
const uint16_t size_of_shared_file = 4096;

int main(void)
{

    /* 1. Create shared memory object and set its size. */
    errno = 0;
    int fd = shm_open(shared_object_name, O_CREAT | O_RDWR, 0777);
    if (fd == -1)
    {
        printf("Create shared object fail with error: %d \n ", errno);
        exit(EXIT_FAILURE);
    }

    errno = 0;

    if (ftruncate(fd, size_of_shared_file) == -1)
    {
        printf("Truncate length of shared file to %d fail with error: %d \n ", size_of_shared_file, errno);
        exit(EXIT_FAILURE);
    }

    /* 2. Map shared memory object. */
    errno = 0;
    void *addr = mmap(NULL, size_of_shared_file, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        printf("Map shared memory object fail with error: %d \n", errno);
        exit(EXIT_FAILURE);
    }

    /* 3. Copy data to shared memory. */
    char buffer[] = "Hello Writer !!!";
    memcpy(addr, (void *)buffer, strlen(buffer) + 1);
    sleep(5);
    shm_unlink(shared_object_name);
    close(fd);
    exit(EXIT_SUCCESS);
}