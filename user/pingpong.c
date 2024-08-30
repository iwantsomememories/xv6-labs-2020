#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;
  char buf;
  char byte = 'a';
  int pipe1[2], pipe2[2];
  pipe(pipe1);
  pipe(pipe2);

  if(fork() == 0){
    pid = getpid();
    read(pipe1[0], &buf, 1);
    printf("%d: received ping\n", pid);
    write(pipe2[1], &byte, 1);

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    exit(0);
  } else {
    pid = getpid();
    write(pipe1[1], &byte, 1);
    read(pipe2[0], &buf, 1);
    printf("%d: received pong\n", pid);

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    exit(0);
  }
}

