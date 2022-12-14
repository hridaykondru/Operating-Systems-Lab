#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/msg.h>
#include<limits.h>
#include <time.h>

key_t key1,key2,key3,key4;
int msgid1,msgid2,msgid3,msgid4;

struct msgbuffer {
    long mtype;       /* message type, must be > 0 */
    char mtext[256];    /* message data */
}message;

int game_round(int k, int min_val, int max_val, int pids[]){
    int rndclient[100];
    double delta[100];
    for(int i=0;i<k;++i){
        message.mtype=pids[i];
        sprintf(message.mtext,"%d %d",min_val,max_val);
        msgsnd(msgid2, &message, sizeof(message), 0);
    }
    int g=(rand()%(max_val-min_val+1))+min_val;
    for(int i=0;i<k;++i){
        msgrcv(msgid3,&message,sizeof(message),pids[i],0);         
        rndclient[i]=atoi(message.mtext);
    }
    // for(int i=0;i<k;++i){
    //     printf("rnd %d - %d\n",pids[i],rndclient[i]);
    // }
    double denom=0;
    for(int i=0;i<k;++i){
        delta[i]=(double)abs(rndclient[i]-g);
    }
    for(int i=0;i<k;++i){
        denom+=delta[i]*delta[i];
    }
    if(denom==0){
        denom=1;
    }
    int min_client=0;
    double min_delta=__DBL_MAX__;
    for(int i=0;i<k;++i){
        delta[i]=delta[i]/denom;
        if(min_delta>delta[i]){
            min_client=i;
            min_delta=delta[i];
        }

    }  
    //  for(int i=0;i<k;++i){
    //     printf("delta %d - %f\n",pids[i],delta[i]);
    // }
    
    // for(int i=0;i<k;++i){
    //     message.mtype=pids[i];
    //     sprintf(message.mtext,"%d",result[i]);
    //     msgsnd(msgid4, &message, sizeof(message), 0);
    // }
    printf("Random value chosen by server(G): %d\n",g);
    printf("Random values chosen by clients:\n");
    for(int i=0;i<k;++i){
        printf("%d ",rndclient[i]);
    }
    printf("\nComputed δ values:\n");
    for(int i=0;i<k;++i){
        printf("%f ",delta[i]);
    }
    return(min_client);  
}


int main(int argc, char** argv){
    srand(time(0));
    
    key1 = ftok("queue", 100);
    key2 = ftok("queue", 200);
    key3 = ftok("queue", 300);
    key4 = ftok("queue", 99);
    msgid1 = msgget(key1, 0666 | IPC_CREAT);
    msgid2 = msgget(key2, 0666 | IPC_CREAT);
    msgid3 = msgget(key3, 0666 | IPC_CREAT);
    msgid4 = msgget(key4, 0666 | IPC_CREAT);
    if(argc<=3){
        printf("Please provide arguements in the format: <num_clients> <min> <max>\n");
        exit(0);
    }
    int k=atoi(argv[1]);
    int min_val=atoi(argv[2]);
    int max_val=atoi(argv[3]);
    int pids[100];
    
    int result[100]={0};
    for(int i=0;i<k;++i){
        msgrcv(msgid1,&message,sizeof(message),1,0);         
        pids[i]=atoi(message.mtext);
    }
    // printf("-----server---------\n");
    // for(int i=0;i<k;++i){
    //     printf("%d\n",pids[i]);
    // }
    // printf("-----server---------\n");
    int not_won=1;
    int round_win_client;
    int i=1;
    while(not_won){
        printf("\n----------------Round %d----------------\n",i);
        round_win_client=game_round(k,min_val,max_val,pids);
        result[round_win_client]+=5; 
        printf("\nScore after round %d:\n",i);
        for(int i=0;i<k;++i){
            printf("%d ",result[i]);
        }
        printf("\nClient %d wins the round\n",round_win_client+1);
        if(result[round_win_client]==50){
            not_won=0;
            for(int i=0;i<k;++i){
                message.mtype=pids[i];
                sprintf(message.mtext,"%d",0);
                msgsnd(msgid1, &message, sizeof(message), 0);
            }
        }
        else{
            for(int i=0;i<k;++i){
                message.mtype=pids[i];
                sprintf(message.mtext,"%d",1);
                msgsnd(msgid1, &message, sizeof(message), 0);
            }
        }
        ++i;
        sleep(2);
    }
    printf("\nClient %d has won the game with 50 points!\n",round_win_client+1);    
    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
    msgctl(msgid3, IPC_RMID, NULL);
    msgctl(msgid4, IPC_RMID, NULL);
    
    return(0);
}
