#include <unistd.h>
#include <stdio.h>


pid_t getppid(void);


///do some thing

int main(){
	printf("Process id is %d \n", getppid());

	while(1);
}
