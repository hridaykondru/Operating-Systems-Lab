#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <time.h>
#include <sys/msg.h>

key_t key1,key2,key3,key4;
int msgid1,msgid2,msgid3,msgid4;

 struct msgbuffer {
    long mtype;       /* message type, must be > 0 */
    char mtext[256];    /* message data */
}message;

void play(int pid){
    msgrcv(msgid2,&message,sizeof(message),pid,0);
    char* p=message.mtext;       
    int min_val=atoi(strtok_r(p," \n",&p));
    int max_val=atoi(strtok_r(p," \n",&p));
    int r=(rand()%(max_val-min_val+1))+min_val;
    message.mtype=pid;
    sprintf(message.mtext,"%d",r);
    msgsnd(msgid3, &message, sizeof(message), 0);
    // msgrcv(msgid4,&message,sizeof(message),pid,0);
    // int result=atoi(message.mtext);
    // if(result){
    //     printf("Child: I %d have won the round\n",pid);
    // }
    //printf("%d %d\n",pid,r);
}

int main(){    
    srand(time(0));
    key1 = ftok("queue", 100);
    key2 = ftok("queue", 200);
    key3 = ftok("queue", 300);
    key4 = ftok("queue", 99);
    msgid1 = msgget(key1, 0666 | IPC_CREAT);
    msgid2 = msgget(key2, 0666 | IPC_CREAT);
    msgid3 = msgget(key3, 0666 | IPC_CREAT);
    msgid4 = msgget(key4, 0666 | IPC_CREAT);
    int pid=getpid();
    message.mtype=1;
    sprintf(message.mtext,"%d",pid);
    msgsnd(msgid1, &message, sizeof(message), 0);
    //printf("%d\n",pid);
    int round_start=1;
    while(round_start){        
        play(pid);
        sleep(1);
        msgrcv(msgid1,&message,sizeof(message),pid,0);
        round_start=atoi(message.mtext);
    }
    
    return(0);
}
