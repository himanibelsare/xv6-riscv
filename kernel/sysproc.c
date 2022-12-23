#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_trace(void)
{
  int traceVar;
  argint(0, &traceVar); // retrieve first argument from kernel function call
  trace(traceVar);
  return 0;
}

uint64
sys_sigalarm(void)
{
  int interval;
  uint64 handler1;

  if (argint(0, &interval) < 0)
    return -1;
  if(interval <= 0)
    return -1;  
  if (argaddr(1, &handler1) < 0)
    return -1;
  myproc()->ticks = interval;
  myproc()->handler = (void*)handler1;
  myproc()->currTicks = 0;
  return 0;
}

uint64
sys_sigreturn(void)
{
  myproc()->currTicks = 0;
  *myproc()->trapframe = myproc()->alarmCopy;
  myproc()->alarmOn = 0;

  return myproc()->trapframe->a0;
}

uint64
sys_waitx(void)
{
  uint64 addr, addr1, addr2;
  uint wtime, rtime;
  argaddr(0, &addr);
  argaddr(1, &addr1); // user virtual memory
  argaddr(2, &addr2);
  int ret = waitx(addr, &wtime, &rtime);
  struct proc* p = myproc();
  if (copyout(p->pagetable, addr1,(char*)&wtime, sizeof(int)) < 0)
    return -1;
  if (copyout(p->pagetable, addr2,(char*)&rtime, sizeof(int)) < 0)
    return -1;
  return ret;
}

uint64
sys_set_priority(void)
{
  int pid, priority;
  if(argint(0, &pid) < 0)
    return -1;
  if(argint(0, &priority) < 0)
    return -1;

  return set_priority(pid, priority);    
}

uint64
sys_settickets(void)
{
  int tickets;
  if(argint(0, &tickets) < 0)
    return -1;

  return settickets(tickets);
}
