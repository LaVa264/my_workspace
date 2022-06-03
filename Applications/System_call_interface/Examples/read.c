#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

//       int open(const char *pathname, int flags);
//       int open(const char *pathname, int flags, mode_t mode);

#define 	DUMMY_FILE 	"dummy.txt"
int main(void){
	// set error number to 0
	errno = 0;
	int fd = open(DUMMY_FILE, O_CREAT | O_RDWR | O_APPEND);
	if(fd < 0){
		printf("open file error fd: %d and errno: %ld\n", fd, (long)errno);	
	return -1;
	}
	char * buffer;
	// read data in file 	
	size_t size = read(fd, (void*)buffer, 20);
	printf("data in the file is: %s with size of data: %ld\n", buffer, size);


	// read data from standard input (terminal)
	// STDIN_FILENO = 0
	size = read(STDIN_FILENO, (void*)buffer, 20);
        printf("data from std input is: %s with size of data: %ld\n", buffer, size);




	// set error number to 0
	errno = 0;
	if(close(fd) == -1){
		printf("close file error :%ld", (long)errno);
	}
}	


