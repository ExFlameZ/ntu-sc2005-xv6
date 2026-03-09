#include "../kernel/types.h"
#include "user.h"

#define MSG_SIZE 32

int
main(void)
{
	int pipe1[2], pipe2[2];
	char buf[MSG_SIZE];
	int pid;

	if (pipe(pipe1) < 0 || pipe(pipe2) <0){
		printf("Pipe creation failed\n");
		exit(1);
	}
	
	pid = fork();
	
	if (pid < 0){
		printf("Fork failed\n");
		exit(1);
	}

	if (pid == 0){
		close(pipe1[1]);
		close(pipe2[0]);

		int n = read(pipe1[0], buf, MSG_SIZE);
		if (n>0){
			buf[n] = 0;
			printf("Sibling B (PID %d) received: %s\n", getpid(), buf);
		}
		
		char *reply = "pong from B";
		printf("Sibling B (PID %d) sending: %s\n", getpid(), reply);
		write(pipe2[1], reply, strlen(reply));

		close(pipe1[0]);
		close(pipe2[1]);
		exit(0);
	}else{
		close(pipe1[0]);
		close(pipe2[1]);

		char *msg = "ping from A";
		printf("Sibling A (PID %d) sending: %s\n", getpid(), msg);
		write(pipe1[1], msg, strlen(msg));
		
		int n = read(pipe2[0], buf, MSG_SIZE);
		if (n> 0){
			buf[n] = 0;
			printf("Sibling A (PID %d) received: %s\n", getpid(), buf);
		}

		close(pipe1[1]);
		close(pipe2[0]);
		wait(0);
		exit(0);
	}
}


