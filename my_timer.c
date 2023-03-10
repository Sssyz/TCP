#include <stdio.h>
#include <stdlib.h>
#include "vxWorks.h"
#include "intLib.h"
#include "sysLib.h"
#include <taskLib.h>
#include <semaphore.h>
#include <msgQLib.h>
#include <wdLib.h>
#include "my_timer.h"
#include "my_task.h"
#include "my_tcp.h"



/* three line to manage the timers of different status */
Timer *timerAvailable;
Timer *timerRun;
Timer *timerAllocated;

Timer *isrTimer;
/*SEM_ID sem_timer1;*/
/*SEM_ID sem_isr;
WDOG_ID watchId;*/
int count=0;

/* init the timer */
void initTimer()
{
    int task_isr;
    int index;
    Timer *timer;
    STATUS result;
    
    /*sme for timer*/
    /*sem_timer1 = semBCreate(SEM_Q_FIFO,SEM_EMPTY); */
    
    /*watch dog*/
    /*
    sem_isr = semBCreate(SEM_Q_FIFO,SEM_EMPTY); 
    watchId = wdCreate();
    printf("watech id:%d\n",watchId);
    */

    timerAvailable = (Timer*)malloc(sizeof(Timer));
    timerRun = (Timer*)malloc(sizeof(Timer));
    timerAllocated = (Timer*)malloc(sizeof(Timer));
    timerAvailable->next = NULL;
    timerRun->next = NULL;
    timerAllocated->next = NULL;

    /* init the timerAvailable*/
    for(index=0;index<TimerNum;index++){
        timer = (Timer*)malloc(sizeof(Timer));
        timer->id = index;
        timer->seconds = 0;
        timer->task_id = 0;
        timer->status = Available;
        
        timer->next = timerAvailable->next;
        
        timerAvailable->next = timer;
    }

    /* ISR*/
    
    if(sysClkRateSet(SYS_CLK_RATE)==ERROR){
        printf("error set rate\n");
    }
    result = sysClkConnect((FUNCPTR)clkISR,0);
    sysClkEnable();
    if(result==OK){
        printf("isr success\n");
    }

  
    
}

/* request a timer , not active*/
Timer* getTimer(int task_id){
    
    Timer* timer;
    
    if(timerAvailable->next == NULL){ /* no available timer */
        return NULL;
    }else{ /* have available timer */
        
        timer = timerAvailable->next;
        timer->status = Allocated;
        timer->task_id = task_id;
        

        timerAvailable->next = timerAvailable->next->next;
        timer->next = timerAllocated->next;
        
        timerAllocated->next = timer;
        
        return timer;

    }
} 


/* free the timer,insert the timer into available line */
void freeTimer(Timer* timer){
   
    Timer *temp;
    
    if(timer->status == Available){ /* status Available */
        printf("this timer is already available\n");
    }
    else if(timer->status == Run){ /* status  Run*/
        /* find the timer */
        temp =  timerRun;
        while(temp->next!=timer&&temp!=NULL){
            temp = temp->next;
        }
        
        /* update the timerRun */
        temp->next = timer->next;

        /* update the timer*/
        timer->status = Available;
        timer->task_id = 0;
        timer->seconds = 0;

        /* update the timerAvailable*/
        timer->next = timerAvailable->next;
        timerAvailable->next = timer;
        
        
        printf("free Run timer success!\n");

    }else if(timer->status == Allocated){ /* status Allocated */
        
        /* find the timer */
        temp =  timerAllocated;
        while(temp->next!=timer&&temp!=NULL){
            temp = temp->next;
        }

        /* update the timerAllocated */
        temp->next = timer->next;
        timer->status = Available;

        /* update the timerAvailable*/
        timer->next = timerAvailable->next;
        timerAvailable->next = timer;
        printf("free allocated timer success!\n");
    }


}

/* activate the timer*/
int activateTimer(Timer* timer,int seconds){ /* insert the timer from allocated line into runtimer line*/

    Timer *temp_timer = timerAllocated;
    while(temp_timer->next!=timer){
        temp_timer = temp_timer->next;
    }

    temp_timer -> next = timer->next;
    timer->status = Run;
    timer->next = timerRun->next;
    timer->seconds = seconds*sysClkRateGet();
    timerRun->next = timer;
    
    
    
    
    return 1;

}
void stopTimer(Timer* timer){
    Timer *temp;
    if(timer->status == Run){ /* status  Run*/
        /* find the timer */
        temp =  timerRun;
        while(temp->next!=timer&&temp!=NULL){
            temp = temp->next;
        }
        
        /* update the timerRun */
        temp->next = timer->next;

        /* update the timer*/
        timer->status = Allocated;
        
        timer->seconds = 0;

        /* update the timerAvailable*/
        timer->next = timerAllocated->next;
        timerAllocated->next = timer;   
    }  
}

/* send timer out msg to task */
void sendTimeroutMsg(Timer *timer){
    MSG msg;
    msg.data=NULL;
    msg.OVERTIME=1;
    switch(timer->task_id){
        case 1:
            msgQSend(client_msg,(char*)(&msg),sizeof(MSG),NO_WAIT,MSG_PRI_NORMAL);
            break;
        case 2:
            msgQSend(server_msg,(char*)(&msg),sizeof(MSG),NO_WAIT,MSG_PRI_NORMAL);
        default:
            break;    


    }

}

/* show all timers' status */
void showTimer()
{
    int index;
    Timer *timer;
    printf("-----------------------\n");
    timer = timerAvailable;
    while(timer->next!=NULL){
        timer = timer->next;
        printf("timer:%d,status:available\n",timer->id);
    }

    timer = timerAllocated;
    while(timer->next!=NULL){
        timer = timer->next;
        printf("timer:%d,status:allocated, used by task%d\n",timer->id,timer->task_id);
    }
    timer = timerRun;
     while(timer->next!=NULL){
        timer = timer->next;
        printf("timer:%d,status:run, used by task%d\n",timer->id,timer->task_id);
    }
    printf("------------------------\n");

}
void clkISR()
{
    
    /*semGive(sem_isr);*/
    
    
    isrTimer = timerRun->next;
    while(isrTimer!=NULL){
        if(isrTimer->seconds>0){
            isrTimer->seconds--;
        }
        if(isrTimer->seconds==0){
            sendTimeroutMsg(isrTimer);
            stopTimer(isrTimer); /*stop the timer change status to allocated*/
            
        }
        isrTimer=isrTimer->next;
    }


    
}




