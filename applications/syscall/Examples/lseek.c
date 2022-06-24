#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define 	DUMMY_FILE 	"dummy.txt"
int main(void){
	// set error number to 0
	errno = 0;
	int fd = open(DUMMY_FILE, O_CREAT | O_RDWR | O_APPEND);
	if(fd < 0){
		printf("open file error fd: %d and errno: %ld\n", fd, (long)errno);	
	return -1;
	}
	char * buffer = "do some thing 3";
	
	
	
	// read data in file 	
	size_t size = write(fd, (void*)buffer, 20);
	printf("write data to file: %s with size of data: %ld\n", buffer, size);
	// set current cursor to begin of file
	off_t offset = lseek(fd, 0, SEEK_SET);
	printf(" set cursor to %ld\n", offset);
	// write data to standard output (terminal)
	// STDOUT_FILENO = 1
	size = write(STDOUT_FILENO, (void*)buffer, 20);
        printf("write: %s with size of data: %ld to terminal\n", buffer, size);




	// set error number to 0
	errno = 0;
	if(close(fd) == -1){
		printf("close file error :%ld", (long)errno);
	}
}	


