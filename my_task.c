#include "my_task.h"
#include <taskLib.h>
#include "my_tcp.h"
#include "my_timer.h"
#include "mem.h"

/* sim TCP client*/
void Client(){
    int index;
    Timer *timer;
    MSG msg,remsg;
    char *mem;
    mem = (char*)malloc_mem(32);
    timer = getTimer(1);
    /* begin three-way handshake*/
    Listen(&client_task);
    showTaskStatus(&client_task);
    /* request to link */
    msg.data=NULL;
    msg.FIN=0;
    msg.ACK=0;
    msg.SYN = 1;
    msg.seq = 1;
    msg.ack= 1;
    remsg = SendToServer(&msg,timer);
    /* confirm the link and send message*/
    strcpy(mem,"i can hear you!");
    msg.data= mem;
    msg.FIN=0;
    msg.ACK=1;
    msg.SYN = 0;
    msg.seq = remsg.ack;
    msg.ack = remsg.seq+1;
    remsg = SendToServer(&msg,timer);

    
    
    /* communication for 5 times*/
    
    for(index=5;index>0;index--){
        strcpy(mem,"i wanna apple!");
        msg.data= mem;
        msg.FIN=0;
        msg.ACK=1;
        msg.SYN = 0;
        msg.seq = remsg.ack;
        msg.ack = remsg.seq+1;
        remsg = SendToServer(&msg,timer);

    }
    
     
    

    /* begin four-way wavehand */
    
    msg.data = NULL;
    msg.FIN = 1;
    msg.ACK=1;
    msg.SYN = 0;
    msg.seq = remsg.ack;
    msg.ack = remsg.seq+1;
    remsg = SendToServer(&msg,timer); /*FIN WAIT 2*/
    
    msgQReceive(client_msg,(char*)&remsg,sizeof(MSG),WAIT_FOREVER); /* wait server FIN*/

    msg.data = NULL;
    msg.FIN = 0;
    msg.ACK = 1;
    msg.SYN = 0;
    msg.seq = remsg.ack;
    msg.ack = remsg.seq+1;
    remsg = SendToServer(&msg,timer); /* last msg*/
    
    /* free mem*/
    free_mem((int*)mem);




}

/* sim TCP server */
void Server(){
    int index;
    Timer *timer;
    MSG msg,re_msg;
    char *mem;
    mem = (char*)malloc_mem(32);
    printf("i am server\n");
    timer = getTimer(2);  /* 2 MEANS SERVER*/
    Listen(&server_task);
    /* begin three-way handshake */
    msgQReceive(server_msg,&re_msg,sizeof(MSG),WAIT_FOREVER);
    printf("server get a msg, seq=%d,ack=%d,data=%s\n,",re_msg.seq,re_msg.ack,re_msg.data);
    if(re_msg.SYN==1){
        
        Syn_rcvd(&server_task);
        showTaskStatus(&server_task);
    }

    msg.data=NULL;
    msg.FIN=0;
    msg.ACK=1;
    msg.SYN = 1;
    msg.seq = re_msg.ack;
    msg.ack = re_msg.seq+1;
    re_msg = SendToClient(&msg,timer);
    
    strcpy(mem,"what do you need?");
    msg.data=mem;
    msg.FIN=0;
    msg.ACK=1;
    msg.SYN = 0;
    msg.seq = re_msg.ack;
    msg.ack = re_msg.seq+1;
    re_msg = SendToClient(&msg,timer);

    
      /* communication for 5 times*/
    
    for(index=5;index>0;index--){
        strcpy(mem,"here you are!");
        msg.data= mem;
        msg.FIN=0;
        msg.ACK=1;
        msg.SYN = 0;
        msg.seq = re_msg.ack;
        msg.ack = re_msg.seq+1;
        re_msg = SendToClient(&msg,timer);

    }
    /* begin four-way wavehand */
    msg.data = NULL;
    
    msg.FIN=0;
    msg.ACK=1;
    msg.SYN = 0;
    msg.seq = re_msg.ack;
    msg.ack = re_msg.seq+1;
    re_msg = SendToClient(&msg,timer);
    
    /* free mem*/
    free_mem((int*)mem);


}

/* init the task server and client */
void initTask(){
     Timer *timer;
     MSG msg;
    /* init the task struct */
    client_task.task_id = 1;
    client_task.status = CLOSED;
    server_task.task_id = 2;
    server_task.status = CLOSED;
    /* init msg */
    client_msg = msgQCreate(1,sizeof(MSG),MSG_Q_FIFO);
    server_msg = msgQCreate(1,sizeof(MSG),MSG_Q_FIFO);
    
    /* create client and server*/
    client = taskSpawn("client",200,0,1000,(FUNCPTR)Client,0,0,0,0,0,0,0,0,0,0);
    server = taskSpawn("server",200,0,1000,(FUNCPTR)Server,0,0,0,0,0,0,0,0,0,0);
    
    
}