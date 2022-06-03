#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h> /* Defines O_* constants */
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>

const char *sock_name = "larvar_server";
const char *sock_path = "/larva";
const uint16_t maximum_size_of_buffer = 100;

int main(void)
{
    errno = 0;
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
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
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_path, sizeof(addr.sun_path) - 1);

    /* 4. Bind socket to server socket address. */
    errno = 0;
    if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        printf("Failed to bind socket to server socket address with error: %d\n", errno);
        goto exit_failure;
    }

    /* 5. Make socket as a passive socket, that is, as a socket that will be used to accept incoming connection requests. */
    errno = 0;
    if (listen(sock_fd, 5) == -1)
    {
        printf("Failed to make it as a listening socket with error: %d\n", errno);
        goto exit_failure;
    }

    /* 6. Server waits for incomming connection requests. */
    while (1)
    {
        errno = 0;
        int client_fd = accept(sock_fd, NULL, NULL); /* If there are no pending connections when accept() is called, the call blocks until a connection request arrives. */
        printf("[%d] There is a call !!\n", getpid());
        if (client_fd == -1)
        {
            printf("Failed to accept a incoming connection request with error: %d \n", errno);
            goto exit_failure;
        }
        char buffer[maximum_size_of_buffer];
        errno = 0;
        if (read(client_fd, (void *)buffer, maximum_size_of_buffer) < 0)
        {
            printf("Failed to get data client with error: %d \n", errno);
            goto exit_failure;
        }
        printf("[%d] Data get from client: %s \n", getpid(), buffer);

        /* Response to client.*/
        int num_write = 0;
        errno = 0;
        if ((num_write = write(client_fd, (void *){"Server received data successfully"}, maximum_size_of_buffer)) < 0)
        {
            printf("Failed to response to client with error: %d \n", errno);
            goto exit_failure;
        }
        close(client_fd);
    }

    exit(EXIT_SUCCESS);
exit_failure:
    exit(EXIT_FAILURE);
}