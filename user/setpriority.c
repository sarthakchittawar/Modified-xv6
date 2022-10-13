#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    // check if params are string
    if (set_priority(atoi(argv[1]), atoi(argv[2])) == -1)
    {
        printf("No runnable process with given pid\n");
    }

    return 0;
}