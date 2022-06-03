#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h> /* Defines O_* constants */
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>

const char *sock_path = "/larva_server";
const char *client_sock_path = "/larva_client";

const uint16_t maximum_size_of_buffer = 100;

int main(int argc, char *argv[])
{
    errno = 0;
    int sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock_fd == -1)
    {
        printf("Failed to create a new socket with error: %d\n", errno);
        goto exit_failure;
    }

    /* 2. Construct client socket address, and bind it to client address */
    struct sockaddr_un client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_un));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, client_sock_path, sizeof(client_addr.sun_path) - 1);
    errno = 0;

    if (remove(client_sock_path) == -1 && errno != ENOENT)
    {
        printf("Failed to remove a old client socket path with error: %d \n", errno);
        goto exit_failure;
    }

    if (bind(sock_fd, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_un)) == -1)
    {
        printf("Failed to bind socket to client socket address with error: %d\n", errno);
        goto exit_failure;
    }

    /* 3. Construct server socket address. */
    struct sockaddr_un server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, sock_path, sizeof(server_addr.sun_path) - 1);

    /* 4. Send message to the socket server. */
    char *buffer_to_write = argv[1];
    if (buffer_to_write == NULL)
    {
        buffer_to_write = "Empty data";
    }
    errno = 0;
    int num_write = 0;
    if ((num_write = sendto(sock_fd, (void *)buffer_to_write, maximum_size_of_buffer, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un))) < 0)
    {
        printf("Failed to send message to socket server with error: %d \n", errno);
        goto exit_failure;
    }
    printf("[%d] Send data to server successfully with length of data: %d \n", getpid(), num_write);

    /* 6. Get feedback from server*/
    {
        char buffer_to_read[maximum_size_of_buffer];
        socklen_t len = sizeof(struct sockaddr_un);
        errno = 0;
        if (recvfrom(sock_fd, (void *)buffer_to_read, maximum_size_of_buffer, 0, (struct sockaddr *)&server_addr, &len) < 0)
        {
            printf("Failed to get feedback from server with error: %d \n", errno);
            goto exit_failure;
        }

        printf("[%d] Server feedback with data: %s \n", getpid(), buffer_to_read);
    }
    exit(EXIT_SUCCESS);
exit_failure:
    exit(EXIT_FAILURE);
}