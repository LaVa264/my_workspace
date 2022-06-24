#include <sys/socket.h>
#include <sys/un.h> /* definations for UNIX domain sockets */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SOCKET_PATH	"intermediary.txt"

inline void handle_error(const char* str){
	perror(str);
	exit(EXIT_FAILURE);
}


int main(){
	//1. create a socket.
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(fd < 0)
		handle_error("socket");
	//2. binding the socket to well-know address.
	//2.1. assign a address to it (this operation is called as assigning a name to a socket).
	struct sockaddr_un address = {.sun_family = AF_UNIX};
	memset(&address, 0, sizeof(address));
	strncopy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

	bind(fd, )
	
	
}
