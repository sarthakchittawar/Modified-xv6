Specification 1 (System Calls):

trace:
    Firstly we are asked to reference the syscall as 1 << n, so in syscall.c we right shift the same value wrt num.
    If this value is not 0, it would mean that we include this in the tracing of the given ID number and print out certain details like pid, name, params, return value of the syscall process using its struct proc* and struct trapframe (registers)

sigalarm + sigreturn:
    We initialise a variable pointer p->time and p->interval in struct proc.
    p->time is initialised to 0, and p->interval is given as an argument in the syscall.
    In trap.c, namely in the usertrap() function, we check if the interrupt is a timer interrupt or not. If it is, then we increment p->time and if p->time >= p->interval, we store the values of registers as a backup in p->backup.
    Also, we change location of Program Counter to the sighandler given to us as an argument.
    We set a sigflag = 1 to prevent it being called again.

    Now, when sigreturn is called, we restore the registers using p->backup, reset time and sigflag to 0. We need to call usertrapret() in order to prevent the return register getting modified.

    Testing via sigalarm works with all Scheduling policies.

Note: README contents of Spec-2 (Scheduling) is done in the Report pdf.