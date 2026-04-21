#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  // Create multiple child processes
  for(int i = 0; i < 4; i++){
    int pid = fork();

    if(pid < 0){
      printf("fork failed\n");
      exit(1);
    }

    if(pid == 0){
      // child process
      int mypid = getpid();

      // run a few iterations only
      for(int j = 0; j < 5; j++){
        for(volatile int k = 0; k < 200000000; k++); // slow down

        printf("PID %d running (iter %d)\n", mypid, j);
      }

      exit(0);
    }
  }

  // parent waits for all children
  for(int i = 0; i < 4; i++){
    wait(0);
  }

  exit(0);
}
