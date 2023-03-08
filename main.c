#include <stdio.h>
#include "my_tcp.h"
#include "mem.h"
#include "my_timer.h"
#include "msgQLib.h"
#include "vxWorks.h"
#include "tasklib.h"
#include "my_task.h"
int *mem_add;
int main()
{
   
    mem_init(mem_add);
    initTimer();
    initTask();

    return 0;
}