#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h> /* Defines O_* constants */
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <ctype.h>

const char *sock_path = "/larva_server";
const uint16_t maximum_size_of_buffer = 100;

int main(void)
{
    errno = 0;
    int sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock_fd == -1)
    {
        printf("Failed to create a new socket with error: %d\n", errno);
        goto exit_failure;
    }
    /* 2. Remove old socket path if it exists.*/
    errno = 0;
    if (remove(sock_path) == -1 && errno != ENOENT)
    {
        printf("Failed to remove a old socket path with error: %d \n", errno);
        goto exit_failure;
    }

    /* 3. Construct server socket address. */
    struct sockaddr_un server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, sock_path, sizeof(server_addr.sun_path) - 1);

    /* 4. Bind socket to server socket address. */
    errno = 0;
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1)
    {
        printf("Failed to bind socket to server socket address with error: %d\n", errno);
        goto exit_failure;
    }

    /* 5. Server waits for messages. */
    while (1)
    {
        char buffer_to_read[maximum_size_of_buffer];
        struct sockaddr_un client_addr;
        socklen_t len = sizeof(struct sockaddr_un);

        errno = 0;
        int num = 0;

        if ((num = recvfrom(sock_fd, (void *)buffer_to_read, maximum_size_of_buffer, 0, (struct sockaddr *)&client_addr, &len)) < 0)
        {
            printf("Failed to receive a message from client with error: %d\n", errno);
            goto exit_failure;
        }
        printf("[%d] Server received %d bytes from %s with data: %s \n", getpid(), num, client_addr.sun_path, buffer_to_read);

        /* Response to client. */
        if (sendto(sock_fd, (void *){"Server received message successfully"}, maximum_size_of_buffer, 0, (struct sockaddr *)&client_addr, len) < 0)
        {
            printf("Failed to response to client with error: %d\n", errno);
            goto exit_failure;
        }

        printf("[%d] Server send feedback successfully \n", getpid());
    }

    exit(EXIT_SUCCESS);
exit_failure:
    exit(EXIT_FAILURE);
}