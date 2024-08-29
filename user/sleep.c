#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int count;

  if(argc <= 1){
    fprintf(2, "Usage: sleep seconds\n");
    exit(1);
  }

  count = atoi(argv[1]);
  if(count == 0){
    fprintf(2, "Invalid parameters\n");
    exit(1);
  }

  sleep(count);

  exit(0);
}
