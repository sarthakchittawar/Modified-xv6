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
  return 0;  // not reached
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
  if(growproc(n) < 0)
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
  while(ticks - ticks0 < n){
    if(killed(myproc())){
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

// trace sys-call
uint64
sys_trace(void)
{
  struct proc *mp = myproc();
  int temp;
  argint(0, &temp);

  if (mp->trace < 0)
  {
    return -1;
  }
  mp->trace = temp;
  return 0;
  
}

// sigalarm
uint64
sys_sigalarm(void)
{
  struct proc *np = myproc();

  int interval;
  uint64 func;
  argint(0, &interval);
  argaddr(1, &func);

  np->interval = interval;
  np->sighandler = func;    // sets sighandler to given function call
  np->time = 0;

  return 1;
}

// sigreturn
uint64
sys_sigreturn(void)
{
  struct proc *np = myproc();
  struct trapframe *mp = np->backup;
  *(np->trapframe) = *(mp);
  
  np->time = 0;
  np->sigflag = 0;  // set values to default

  kfree(mp);

  usertrapret();    // return to user space, to not modify a0 register unnecessarily
  return 1;
}