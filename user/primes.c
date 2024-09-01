#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

/**
 * @brief 返回左邻居的第一个数据
 */
int lpipe_first_data(int lpipe[2], int *dst)
{
  if(read(lpipe[RD], dst, INT_LEN) == sizeof(int)){
    printf("prime %d\n", *dst);
    return 0;
  }
  return -1;
}

/**
 * @brief 传递数据
 */
void transmit_data(int lpipe[2], int rpipe[2], int first)
{
  int data;
  // 从左管道读取数据
  while(read(lpipe[RD], &data, INT_LEN) == sizeof(int)){
    if(data % first != 0)
      write(rpipe[WR], &data, INT_LEN);
  }

  close(lpipe[RD]);
  close(rpipe[WR]);

}

void primes(int lpipe[2])
{
  close(lpipe[WR]);
  int first;
  if(lpipe_first_data(lpipe, &first) == 0)
  {
    int p[2];
    pipe(p);
    transmit_data(lpipe, p, first);

    if(fork() == 0)
      primes(p);
    else {
      close(p[RD]);
      wait(0);
    }
  }
  exit(0);
}

int main(int argc, char *argv[]){
  int p[2];
  pipe(p);

  for(int i = 2; i <= 35; i++)
    write(p[WR], &i, INT_LEN);

  if(fork() == 0)
    primes(p);
  else {
    close(p[WR]);
    close(p[RD]);
    wait(0);
  }

  exit(0);
}
