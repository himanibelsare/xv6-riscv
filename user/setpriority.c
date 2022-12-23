#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int priority;
  int pid;
  if (argc != 3)
  {
    exit(1);
  }

  priority = atoi(argv[1]);
  pid = atoi(argv[2]);

  printf("setpriority: %d\n",set_priority(priority, pid));
  exit(0);  
}