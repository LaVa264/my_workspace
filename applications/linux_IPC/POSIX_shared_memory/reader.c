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

    /* 1. Open existing object. */
    errno = 0;
    int fd = shm_open(shared_object_name, O_RDONLY, 0);
    if (fd == -1)
    {
        printf("Read shared object fail with error: %d \n ", errno);
        exit(EXIT_FAILURE);
    }

    /* 2. Get information of the shared file from stat structure. */
    errno = 0;
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        printf("get information of shared file fail with error: %d \nd", errno);
        exit(EXIT_FAILURE);
    }
    /* 3. Use shared memory object size as length argument for mmap() and as number of bytes to write() */

    errno = 0;
    char *addr = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        printf("Map shared memory object fail with error: %d \n", errno);
        exit(EXIT_FAILURE);
    }

    printf("Data get from Reader is: %s \n", addr);
    close(fd);
    exit(EXIT_SUCCESS);
}