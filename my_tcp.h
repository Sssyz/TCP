#ifndef _MYTCP_
#define _MYTCP_
#include "my_timer.h"
#include <msgQLib.h>
#define CLOSED 0
#define LISTENED 1
#define SYN_SENT 2
#define SYN_RCVD 3
#define ESTAB 4
#define FIN_WAIT1 5
#define FIN_WAIT2 6
#define CLOSED_WAIT 7
#define TIME_WAIT 8
#define LAST_ACK 9
typedef int TCP_STATUS;

typedef struct TASK{
    int task_id;
    TCP_STATUS status;
}TASK;

typedef struct MSG{
    char *data;
    int ACK;
    int SYN;
    int FIN;
    int ack;
    int seq;
    int OVERTIME;


}MSG;
/*
    function (change status)

*/
/* transform to status LISETNED */
int Listen(TASK *task);

/* transform to status LISETNED */
int Close(TASK *task);

int Syn_send(TASK *task);

int Syn_rcvd(TASK *task);

int Estab(TASK *task);

int Fin_wait1(TASK *task);

int Fin_wait2(TASK *task);   

int Closed_waited(TASK *task);

int Time_wait(TASK *task);

int Last_ack(TASK *task);

MSG SendToServer(MSG* msg,Timer* timer);

MSG SendToClient(MSG* msg,Timer* timer);
void showTaskStatus(TASK *task);

#endif