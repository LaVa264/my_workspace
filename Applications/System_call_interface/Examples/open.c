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
	int fd = open(DUMMY_FILE, O_CREAT | O_RDWR | O_TRUNC);
	if(fd < 0){
		printf("open file error fd: %d and errno: %ld\n", fd, (long)errno);	
	}

	// set error number to 0
	errno = 0;
	if(close(fd) == -1){
		printf("close file error :%ld", (long)errno);
	}
}	
