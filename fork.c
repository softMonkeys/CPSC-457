/*
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	pid_t childPID;

	childPID = fork();
	if(childPID >= 0){	// fork is successful
		if(childPID == 0){	// child process
			printf("child process\n");
		} else{		// parent process
			printf("parent process\n");
		}
	} else{	// fork fail
		printf("fork fail\n");
		return 1;
	}
	return 0;
}

***********************************************

#include <stdio.h>
#include <unistd.h>
int main() {
	printf("A\n");//Only first process exists

	fork();
	printf("B\n");//First process+first child

	fork();//Called by both the parent and child
	printf("C\n");//4 processes prints it

	return 0;
}

#include <stdio.h>
#include <unistd.h>
int main() {
	for(int i=0; i<4; i++){
		fork();
	}
	printf("X\n");
	return 0;
}
*/


#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	pid_t pid;

	pid = fork(); /* create & run child process - a duplicate of parent */
	/* both parent and child will execute the next line */
	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork failed");
		exit(-1);
	}
	else if (pid == 0) { /* child process */
		execlp("/bin/ls", "ls", "-l", NULL); /* replace process with 'ls -l' */
	}
	else { /* parent process will wait for the child to complete */
		printf("Waiting for child process %d\n", pid);
		wait(NULL);
		exit(0);
	}
}


