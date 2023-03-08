#ifndef _MYTASK_
#define _MYTASK_
#include <msgQLib.h>
#include "my_tcp.h"
int client,server;
TASK client_task,server_task; 
MSG_Q_ID client_msg,server_msg;
void Client();
void Server();
void initTask();
#endif