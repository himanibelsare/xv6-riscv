#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int tickets;
  if (argc != 2)
  {
    exit(1);
  }

  tickets = atoi(argv[1]);

  printf("tickets: %d\n",settickets(tickets));
  exit(0);  
}