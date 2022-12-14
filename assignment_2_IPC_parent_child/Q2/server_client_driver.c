#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<string.h>
#include <dirent.h>
#include<unistd.h>
#include <limits.h>
#include<sys/wait.h>
#include <fcntl.h>

void make_client(){
    int fork1=fork();
    if(fork1==-1){
        printf("Error occured creating the client process\n");
        exit(-1);
    }
    else if(fork1!=0){
        return;
    }
    else{
        //client          
        execl("client","client",(char*)0);
    }
}

int main(int argc,char* argv[]){
    if(argc<=3){
        printf("Please provide arguements in the format: <num_clients> <min> <max>\n");
        exit(0);
    }
    printf("\n~Initialising~\n");
    int k=atoi(argv[1]);
    int fork1=fork();    
    
    if(fork1==-1){
        printf("Error occured creating the server process\n");
        exit(-1);
    }
    else if(fork1!=0){
    	 
        for(int i=0;i<k;++i){        
            sleep(1); 
            make_client();             
        }
        sleep(1); 
    }
    else{
        //server 
        execl("server","server",argv[1],argv[2],argv[3],NULL);
    }
    for(int i=0;i<=k;++i){
        wait(NULL);
    }
    return(0);
}
