#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<string.h>
#include<iostream>
#include <dirent.h>
#include<unistd.h>
#include <limits.h>
#include<sys/wait.h>
#include <fcntl.h>
using namespace std;

void make_client(char** args,char* name){
    int fork1=fork();
    if(fork1==-1){
        printf("Error occured creating the process\n");
        exit(-1);
    }
    else if(fork1!=0){
        sleep(1);
        return;
    }
    else{
        //Process     
        execlp("/usr/bin/xterm","xterm","-T",name,"-e","./process",args[0],args[1],args[2],args[3],NULL);
    }
}

int main(int argc,char* argv[]){
    srand(time(0));
    printf("\n~Initialising~\n");
    char prio1[4];
    char prio2[4];
    char prio3[4];
    char prio4[4];
    sprintf(prio1,"%d",1+ (rand() % 5));
    sprintf(prio2,"%d",1+ (rand() % 5));
    sprintf(prio3,"%d",6+ (rand() % 5));
    sprintf(prio4,"%d",6+ (rand() % 5));
    char* args1[4]={"2000",prio1,"0.01","1"};
    char* args2[4]={"2000",prio2,"0.01","1"};
    char* args3[4]={"1000",prio3,"0.1","2"};
    char* args4[4]={"1000",prio4,"0.1","2"};
    printf("P1 rounds: %s, priority: %s, I/O probability: %s, I/O time: %s\n","2000",prio1,"0.01","1");
    printf("P2 rounds: %s, priority: %s, I/O probability: %s, I/O time: %s\n","2000",prio2,"0.01","1");
    printf("P3 rounds: %s, priority: %s, I/O probability: %s, I/O time: %s\n","1000",prio3,"0.1","2");
    printf("P4 rounds: %s, priority: %s, I/O probability: %s, I/O time: %s\n","1000",prio4,"0.1","2");
    sleep(1);
    make_client(args1,"P1");
    make_client(args2,"P2");
    make_client(args3,"P3");
    make_client(args4,"P4");
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return(0);
}
