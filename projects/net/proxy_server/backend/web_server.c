#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>

#define DEFAULT_PAGE "templates/index.html"
#define log(fmt, ...) vlog(fmt, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
void vlog(const char *const fmt, const char *const caller_name, const char *const file_name, int line, ...);

const uint16_t buf_size = 1024;

typedef enum http_request_type
{
    GET = 0,
    HEAD = 1,
    POST = 2,
    UNSUPPORTED = 10
} http_method;

typedef struct http_message
{
    http_method m_method;
    char m_path[50];
    char m_version[10];
    char m_data[50];
    char m_mime_type[10];
} http_message;

// * Helper functions.
bool write_str_to_client(int sckt, const char *str);

bool write_data_to_client(int sckt, const void *data, size_t len);

bool compress_data(FILE *fp, void *data);

long get_file_size();

void http_request_parser(const char *req, http_message *mess);

http_method get_method(const char *method);

char *get_real_path(const char *http_path);

char *get_mime_type(const char *path);

int main(void)
{
    int server_socket, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen;

    char *buffer;
    buffer = (char *)malloc(buf_size);
    if (!buffer)
    {
        perror("The receive buffer was not allocated");
        exit(EXIT_FAILURE);
    }
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("The socket was not created.");
        exit(EXIT_FAILURE);
    }

    log("The socket was created\n");

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); //  INADDR_ANY;
    address.sin_port = htons(84);

    int option_value;
    option_value = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("The socket was not bound");
        exit(EXIT_FAILURE);
    }

    log("The socket is bound.\n");

    if (listen(server_socket, 10) == -1)
    {
        perror("The socket was not opened for listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        log("The socket is listening...\n");

        addrlen = sizeof(address);
        // * 1. Waiting for connection from client.
        client_socket = accept(server_socket, (struct sockaddr *)&address, &addrlen);

        if (client_socket == -1)
        {
            perror("A client was not accepted");
            goto exit_failure;
        }

        log("A client is connected from %s:%hu...\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        // * 2. Get message.
        int numRead = recv(client_socket, buffer, buf_size, 0);
        if (numRead < 1)
        {
            if (numRead == 0)
            {
                log("The client was not read from: disconnected\n");
            }
            else
            {
                perror("The client was not read from");
            }
            goto exit_failure;
        }

        log("Request: \n%.*s\n", numRead, buffer);
        
        // * 3. Parse the message.
        http_message message;
        http_request_parser(buffer, &message);

        log("Parser method: %d\n", message.m_method);
        log("Parser path: %s\n", message.m_path);
        log("Parser version: %s\n", message.m_version);
        log("Parser mime type: %s\n", message.m_mime_type);

        // * 4. Response to the client.
        if (message.m_method == GET || message.m_method == HEAD)
        {
            FILE *fp;
            fp = fopen(message.m_path, "rb");
            if (!fp)
            {
                perror("Fail to open file");
                goto response_error;
            }

            long file_size;
            file_size = get_file_size(fp);
            if (file_size <= 0)
            {
                perror("Fail to get file size");
                goto response_error;
            }

            if (!write_str_to_client(client_socket, "HTTP/1.1 200 OK\r\n"))
            {
                goto response_error;
            }

            char clen[40];
            sprintf(clen, "Content-length: %ld\r\n", file_size);
            if (!write_str_to_client(client_socket, clen))
            {
                goto response_error;
            }

            char content_type[40];
            sprintf(content_type, "Content-Type: %s\r\n", message.m_mime_type);
            if (!write_str_to_client(client_socket, content_type))
            {
                goto response_error;
            }

            if (!write_str_to_client(client_socket, "Connection: close\r\n\r\n") == -1)
            {
                goto response_error;
            }

            char *msg;
            msg = (char *)malloc(file_size);
            if (!msg)
            {
                perror("Fail allocate message buffer");
                goto response_error;
            }

            if (!compress_data(fp, (void *)msg))
            {
                perror("Fail to compress data");
                goto free_buffer;
            }

            if (!write_data_to_client(client_socket, msg, file_size))
            {
                goto free_buffer;
            }

            log("The file was sent successfully\n");

            goto exit_success;
            /* Clean up*/

        free_buffer:
            free((void *)msg);
        response_error:
            log("Sending page not found message.\n");
            write_str_to_client(client_socket, "HTTP/1.1 404 Not Found\r\n");
            write_str_to_client(client_socket, "Connection: close\r\n\r\n");
        exit_failure:
            close(client_socket);
            continue;
        exit_success:
            free((void *)msg);
            close(client_socket);
        }
        else if (message.m_method == POST)
        {
            if (strcmp(message.m_data, "\0"))
            {
                log("Get data from client: %s \n", message.m_data);
                log("Response accepted message. \n");
                write_str_to_client(client_socket, "HTTP/1.1 202 Accepted\r\n");
            }
            else
            {
                log("Fail to get data!, response error message.\n");
                write_str_to_client(client_socket, "HTTP/1.1 503 Service Unavailable\r\n");
            }

            write_str_to_client(client_socket, "Connection: close\r\n\r\n");
            close(client_socket);
        }
        else
        {
            log("Not handle request, response error. \n");
            write_str_to_client(client_socket, "HTTP/1.1 400 Bad Request\r\n");
            write_str_to_client(client_socket, "Connection: close\r\n\r\n");
            close(client_socket);
        }
    }

    close(server_socket);
    return EXIT_SUCCESS;
}

bool write_data_to_client(int sckt, const void *data, size_t len)
{
    const char *pdata = (const char *)data;

    while (len > 0)
    {
        int num_sent = send(sckt, pdata, len, 0);
        if (num_sent <= 0)
        {
            if (num_sent == 0)
            {
                log("The client was not written to: disconnected\n");
            }
            else
            {
                perror("The client was not written to");
            }
            return false;
        }
        pdata += num_sent;
        len -= num_sent;
    }

    return true;
}

bool write_str_to_client(int sckt, const char *str)
{
    return write_data_to_client(sckt, str, strlen(str));
}

bool compress_data(FILE *fp, void *data)
{
    if (!fp)
    {
        perror("The file was not opened");
        return false;
    }

    log("The file was opened.\n");

    if (!data)
    {
        perror("The file buffer was not allocated");
        return false;
    }
    long file_size;
    file_size = get_file_size(fp);
    if (file_size <= 0)
    {
        perror("Fail to get file size");
        return false;
    }

    if (fread(data, file_size, 1, fp) != 1)
    {
        perror("The file was not read");
        free(data);
        return false;
    }
    fclose(fp);
    log("The file is compressed successfully. \n");

    return true;
}

long get_file_size(FILE *fp)
{
    long file_size;
    if (!fp)
    {
        perror("The file was not opened");
        return -1;
    }
    if (fseek(fp, 0, SEEK_END) == -1)
    {
        perror("The file was not seeked");
        return -1;
    }

    file_size = ftell(fp);
    if (file_size == -1)
    {
        perror("The file size was not retrieved");
        return -1;
    }

    rewind(fp);
    return file_size;
}

void http_request_parser(const char *req, http_message *mess)
{

    if (!mess)
    {
        log("HTTP message handler is NULL.\n");
        goto exit_open_message_failure;
    }

    {
        char arr_req[strlen(req)];
        strcpy(arr_req, req);

        char *first_line = strtok(arr_req, "\n");
        if (!first_line)
        {
            perror("Fail to parse the request");
            goto exit_failure;
        }
        log("Request is: %s\n", first_line);

        char *tokens[3];
        tokens[0] = strtok(first_line, " ");
        for (int i = 1; i < 3; i++)
        {
            tokens[i] = strtok(NULL, " ");
        }
        if (!tokens[0])
        {
            goto exit_failure;
        }
        mess->m_method = get_method(tokens[0]);
        if (!tokens[1])
        {
            goto exit_failure;
        }
        strcpy(mess->m_path, get_real_path(tokens[1]));
        strcpy(mess->m_mime_type, get_mime_type(tokens[1]));

        if (!tokens[2])
        {
            goto exit_failure;
        }
        strcpy(mess->m_version, tokens[2]);

        if (mess->m_method == POST)
        {
            char *ret = strchr(req, '{');
            if (ret == NULL)
            {
                strcpy(mess->m_data, "\0");
            }
            else
            {
                strcpy(mess->m_data, ret);
            }
        }

        log("Parse successfully.\n");
        return;

    exit_failure:
        mess->m_method = UNSUPPORTED;
    }
exit_open_message_failure:
    return;
}

http_method get_method(const char *method)
{
    if (!strcmp(method, "GET"))
    {
        return GET;
    }
    else if (!strcmp(method, "HEAD"))
    {
        return HEAD;
    }
    else if (!strcmp(method, "POST"))
    {
        return POST;
    }
    else
    {
        return UNSUPPORTED;
    }
}

char *get_real_path(const char *http_path)
{

    if (!strcmp(http_path, "/"))
    {
        return DEFAULT_PAGE;
    }

    char *path = strchr(http_path, '/');
    path++;

    if (!path)
    {
        return "";
    }

    if (strstr(path, ".css"))
    {
        return path;
    }
    else if (strstr(path, ".js"))
    {
        return path;
    }
    else if (strstr(path, ".txt"))
    {
        return path;
    }
    else
    {
        return "";
    }
}

char *get_mime_type(const char *path)
{
    if (strstr(path, ".html"))
    {
        return "text/html";
    }
    else if (strstr(path, ".css"))
    {
        return "text/css";
    }
    else if (strstr(path, ".js"))
    {
        return "text/javascript";
    }
    else if (strstr(path, ".txt"))
    {
        return "text/plain";
    }
    else
    {
        return "text/html";
    }
}

void vlog(const char *const fmt, const char *const caller_name, const char *const file_name, int line, ...)
{
    char tm[50];
    strftime(tm, sizeof tm, "%T", localtime(&(time_t){time(NULL)}));
    printf("[pid:%d][%s][%s:%d][%s] ", getpid(), tm, file_name, line, caller_name);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
