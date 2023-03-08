#include <stdio.h>
#include "my_tcp.h"
#include "vxworks.h"
#include "syslib.h"
#include "tasklib.h"
#include "my_task.h"

int Listen(TASK *task){
    if(task->status == CLOSED){
        task->status = LISTENED;
        return 1;
    }
    return -1;
}

int Close(TASK *task){
    if(task->status==LAST_ACK || task->status==TIME_WAIT){
        task->status = CLOSED;
        return 1;
    }
    return -1;
    
}

int Syn_send(TASK *task){
    if(task->status==LISTENED){
        task->status = SYN_SENT;
        return 1;
    }
    return -1;
    
}

int Syn_rcvd(TASK *task){
    if(task->status==LISTENED){
        task->status = SYN_RCVD;
        return 1;
    }
    return -1;
    
}

int Estab(TASK *task){
    if(task->status==SYN_RCVD || task->status == SYN_SENT){
        task->status = ESTAB;
        return 1;
    }
    return -1;
}

int Fin_wait1(TASK *task){
    if(task->status==ESTAB){
        task->status = FIN_WAIT1;
        return 1;
    }
    return -1;
    
}

int Fin_wait2(TASK *task){
    if(task->status == FIN_WAIT1){
        task->status = FIN_WAIT2;
        return 1;
    }
    return -1;
    
}

int Closed_waited(TASK *task){
    if(task->status==ESTAB){
        task->status = CLOSED_WAIT;
        return 1;
    }
    return -1;
    
}

int Time_wait(TASK *task){
    if(task->status == FIN_WAIT2){
        task->status = TIME_WAIT;
        return 1;
    }
    return -1;
    

}

int Last_ack(TASK *task){
    if(task->status == CLOSED_WAIT){
        task->status = LAST_ACK;
        return 1;
    }
    return -1;

}
/* show the status of the task */
void showTaskStatus(TASK *task){
    if(task->task_id==1){
        printf("client status:%d\n",task->status);
    }else if(task->task_id==2){
        printf("server status:%d\n",task->status);
    }
    
}

/* client send msg to server (change the client status / if timerover resend )*/
MSG SendToServer(MSG *msg,Timer *timer){
    MSG re_msg;
    while(1){
        msgQSend(server_msg,(char*)msg,sizeof(MSG),NO_WAIT,MSG_PRI_NORMAL);
        printf("client send a msg ,seq =%d,ack=%d,data=%s\n",msg->seq,msg->ack,msg->data);
        
        /* change status to syn send */
        if(msg->SYN==1){
            
            Syn_send(&client_task);
            
            showTaskStatus(&client_task);
        }
        /* change status to fin wait1*/
        if(msg->FIN==1){
            
            Fin_wait1(&client_task);
            
            showTaskStatus(&client_task);
        }
        if(msg->ACK==1&&client_task.status==FIN_WAIT2){
                Time_wait(&client_task);
                showTaskStatus(&client_task);
        }

        activateTimer(timer,2);
        msgQReceive(client_msg,(char*)&re_msg,sizeof(MSG),WAIT_FOREVER);
        
        if(re_msg.OVERTIME==1){
            if(client_task.status==TIME_WAIT){/* if is time_waited ,timerout means link can be closed*/
                stopTimer(timer);
                Close(&client_task);
                showTaskStatus(&client_task);
                printf("TCP LINK is closed by client!\n");
                break;
            }  
            stopTimer(timer);
            printf("overtimer! resend to server\n");
            continue;
        }else{
            stopTimer(timer);
            printf("client receive a msg ,seq =%d,ack=%d,data=%s\n",re_msg.seq,re_msg.ack,re_msg.data);
            if(re_msg.SYN==1&&re_msg.ACK==1&&re_msg.data==NULL){
                Estab(&client_task);
                showTaskStatus(&client_task);
            }
            if(re_msg.ACK=1&&client_task.status==FIN_WAIT1){
                Fin_wait2(&client_task);
                showTaskStatus(&client_task);
            }
            

            return re_msg;
        }
    }
    
    
}

/* server send msg to client (change the server status / if timerover resend )*/
MSG SendToClient(MSG *msg,Timer *timer){
    MSG re_msg;
    while(1){
        msgQSend(client_msg,(char*)msg,sizeof(MSG),NO_WAIT,MSG_PRI_NORMAL);
        printf("server send a msg ,seq =%d,ack=%d,data=%s\n",msg->seq,msg->ack,msg->data);
        if(server_task.status==CLOSED_WAIT){
            msg->data = NULL;
    
            msg->FIN=1;
            msg->ACK=1;
            msg->SYN = 0;
            msg->seq = msg->seq++;
            msg->ack = msg->ack;
            msgQSend(client_msg,(char*)msg,sizeof(MSG),WAIT_FOREVER,MSG_PRI_NORMAL);
            Last_ack(&server_task);
            showTaskStatus(&server_task);
            printf("server send a msg ,seq =%d,ack=%d,data=%s\n",msg->seq,msg->ack,msg->data);
        }
        activateTimer(timer,2);
        msgQReceive(server_msg,(char*)&re_msg,sizeof(MSG),WAIT_FOREVER);
        
        if(re_msg.OVERTIME==1){
            stopTimer(timer);
            printf("overtimer! resend to client\n");
            continue;
        }else{
            stopTimer(timer);
            printf("server receive a msg ,seq =%d,ack=%d,data=%s\n",re_msg.seq,re_msg.ack,re_msg.data);
            if(re_msg.ACK==1&&server_task.status==SYN_RCVD){
                Estab(&server_task);
                showTaskStatus(&server_task);
            }
            if(re_msg.FIN==1&&server_task.status==ESTAB){
                Closed_waited(&server_task);
                showTaskStatus(&server_task);
            }
            if(re_msg.ACK==1&&server_task.status==LAST_ACK){
                Close(&server_task);
                showTaskStatus(&server_task);
                printf("TCP link is closed by server!\n");
            }
            return re_msg;
        }
    }
    
    
}

