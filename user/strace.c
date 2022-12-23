#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int traceMask;

    if (argc < 3)
    {
        exit(1);
    }

    traceMask = atoi(argv[1]); // store 2nd argument in traceMask

    trace(traceMask);

    char *commands[argc - 1];
    int j = 2;
    for (int i = 0; i < argc - 2; i++) // store commands given in the strace call, first 2 are skipped, i.e "strace" and the traceMask value
    {
        commands[i] = argv[j];
        j++;
    }

    commands[argc - 2] = 0;
    int res = exec(commands[0], commands);
    if (res < 0)
    {
        fprintf(2, "%s failed to execute", commands[0]);
    }
    exit(0);
}