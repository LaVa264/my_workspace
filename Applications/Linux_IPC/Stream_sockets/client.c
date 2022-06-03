#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h> /* Defines O_* constants */
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

const char *sock_name = "larvar_server";
const char *sock_path = "/larva";
const uint16_t maximum_size_of_buffer = 100;

int main(int argc, char *argv[])
{
    errno = 0;
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        printf("Failed to create a new socket with error: %d\n", errno);
        goto exit_failure;
    }

    /* 3. Construct server socket address. */
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_path, sizeof(addr.sun_path) - 1);

    /* 4. Connect to socket server. */
    errno = 0;
    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        printf("Failed to connect to the socket server with error: %d \n", errno);
        goto exit_failure;
    }

    /* 5. Send data to the socket server. */
    char *buffer_to_write = argv[1];
    if (buffer_to_write == NULL)
    {
        buffer_to_write = "Empty data";
    }
    sleep(3);

    errno = 0;
    int num_write = 0;
    if ((num_write = write(sock_fd, (void *)buffer_to_write, strlen(buffer_to_write))) < 0)
    {
        printf("Failed to send data to socket server with error: %d \n", errno);
        goto exit_failure;
    }
    printf("[%d] Send data to server sucessfully with lenght of data: %d \n", getpid(), num_write);

    /* 6. Get feedback from server*/
    {
        char buffer_to_read[maximum_size_of_buffer];
        errno = 0;
        if (read(sock_fd, (void *)buffer_to_read, maximum_size_of_buffer) < 0)
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