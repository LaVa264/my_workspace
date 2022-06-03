#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
//int socket(int domain, int type, int protocol);

int main(void){
	int result = socket(AF_UNIX, SOCK_STREAM, 0);
	if(result < 0){
		printf("create a socket failed, with error: %ld \n",(long)errno);
		return -1;
	}	
	printf("create the socket successfully \n");
}

