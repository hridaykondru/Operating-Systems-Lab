#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include<signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string.h>
#include <netdb.h>
using namespace std;
#define MAX_INPUT_SIZE 256
#define sigexpired 7
#define sigrun 8

bool running=false;
char inputbuf[MAX_INPUT_SIZE];
int sockfd;

void handle_sig(int sig){
    if(sig==sigexpired){
        running=false;
    }
    else if(sig==sigrun){
        running=true;
    }
}

int main(int argc, char *argv[]){
    srand(time(0));
    int portnum, n;
    struct sockaddr_in server_addr;
    
    signal(sigexpired, handle_sig);
    signal(sigrun, handle_sig);
    int rounds,priority,sleep_prob_perc,sleep_time;

    if (argc < 5){
        fprintf(stderr,"usage %s <rounds> <priority> <sleep_prob> <sleep_time>\n", argv[0]);
        exit(0);
    }
    rounds=atoi(argv[1]);
    priority=atoi(argv[2]);
    sleep_prob_perc=(int)(atof(argv[3])*100);
    sleep_time=atoi(argv[4]);

    if(!(sleep_prob_perc>=0 && sleep_prob_perc<=100)){
        fprintf(stderr,"Sleep probability must be in the range [0,1]\n");
        exit(0);
    }

    portnum = 654321;
    /* Create client socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        fprintf(stderr, "ERROR opening socket\n");
        exit(1);
    }
    /* Fill in server address */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(!inet_aton("127.0.0.1", &server_addr.sin_addr)){
        fprintf(stderr, "ERROR invalid server IP address\n");
        exit(1);
    }
    server_addr.sin_port = htons(portnum);
    /* Connect to server */
    if (connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) <0){
        fprintf(stderr, "ERROR connecting\n");
        exit(1);
    }    
    printf("Connected to scheduler\n");
    //Send pid
    bzero(inputbuf,MAX_INPUT_SIZE);
    sprintf(inputbuf,"%d",getpid());
    n = write(sockfd,inputbuf,strlen(inputbuf));
    if (n < 0){
        fprintf(stderr, "ERROR writing pid to socket\n");
        exit(1);
    }
    //Read confirmation
    bzero(inputbuf,MAX_INPUT_SIZE);    
    int rs=read(sockfd, inputbuf, MAX_INPUT_SIZE);
    if(rs==0){
        printf("Scheduler ended\n");
        close(sockfd);
        exit(1);
    }
    else if(rs<0){
        fprintf(stderr, "ERROR in reading\n");
        close(sockfd);
        exit(1);
    }
    if(strcmp(inputbuf,"pid")){
        fprintf(stderr, "ERROR wrong confirmation recieved pid as %s \n",inputbuf);
        exit(1);
    }
    //Send priority
    bzero(inputbuf,MAX_INPUT_SIZE);
    sprintf(inputbuf,"%d",priority);
    n = write(sockfd,inputbuf,strlen(inputbuf));
    if (n < 0){
        fprintf(stderr, "ERROR writing priority to socket\n");
        exit(1);
    }
    //Read confirmation
    bzero(inputbuf,MAX_INPUT_SIZE);    
    rs=read(sockfd, inputbuf, MAX_INPUT_SIZE);
    if(rs==0){
        printf("Scheduler ended\n");
        close(sockfd);
        exit(1);
    }
    else if(rs<0){
        fprintf(stderr, "ERROR in reading\n");
        close(sockfd);
        exit(1);
    }
    if(strcmp(inputbuf,"priority")){
        fprintf(stderr, "ERROR wrong confirmation recieved priority as %s\n",inputbuf);
        exit(1);
    }
    int iteration_no=1;
    while(iteration_no<=rounds){
        //usleep(10000);
        while(running && iteration_no<=rounds){   
            printf("Iteration no: %d\n",iteration_no);
            //Do I/O with probability percentage sleep_prob_perc
            int rnd_val=1+ (rand() % 100);
            //cout<<rnd_val<<' '<<sleep_prob_perc<<'\n';
            if(rnd_val<=sleep_prob_perc){
                bzero(inputbuf,MAX_INPUT_SIZE);
                sprintf(inputbuf,"%s","io");
                printf("Performing I/O\n");
                n = write(sockfd,inputbuf,strlen(inputbuf));
                if (n < 0){
                    fprintf(stderr, "ERROR writing io to socket\n");
                    exit(1);
                }
                //Read confirmation
                bzero(inputbuf,MAX_INPUT_SIZE);    
                rs=read(sockfd, inputbuf, MAX_INPUT_SIZE);
                if(rs==0){
                    printf("Scheduler ended\n");
                    close(sockfd);
                    exit(1);
                }
                else if(rs<0){
                    fprintf(stderr, "ERROR in reading\n");
                    close(sockfd);
                    exit(1);
                }
                if(strcmp(inputbuf,"io")){
                    fprintf(stderr, "ERROR wrong confirmation recieved io as %s\n",inputbuf);
                    exit(1);
                }
                running=false;
                sleep(sleep_time);
                //Sending I/O done
                bzero(inputbuf,MAX_INPUT_SIZE);
                sprintf(inputbuf,"%s","iod");
                printf("I/O done\n");
                n = write(sockfd,inputbuf,strlen(inputbuf));
                if (n < 0){
                    fprintf(stderr, "ERROR writing io to socket\n");
                    exit(1);
                }
                //Read confirmation
                bzero(inputbuf,MAX_INPUT_SIZE);    
                rs=read(sockfd, inputbuf, MAX_INPUT_SIZE);
                if(rs==0){
                    printf("Scheduler ended\n");
                    close(sockfd);
                    exit(1);
                }
                else if(rs<0){
                    fprintf(stderr, "ERROR in reading\n");
                    close(sockfd);
                    exit(1);
                }
                if(strcmp(inputbuf,"iod")){
                    fprintf(stderr, "ERROR wrong confirmation recieved iod as %s\n",inputbuf);
                    exit(1);
                }
            }
            else{
                //Perform operations in iteration for 10 milliseconds
                usleep(10000);
            }
            ++iteration_no;
        } 
    }
    bzero(inputbuf,MAX_INPUT_SIZE);
    sprintf(inputbuf,"%s","done");
    printf("Execution completed\n");
    n = write(sockfd,inputbuf,strlen(inputbuf));
    if (n < 0){
        fprintf(stderr, "ERROR writing done to socket\n");
        exit(1);
    }
    //Read confirmation
    bzero(inputbuf,MAX_INPUT_SIZE);    
    rs=read(sockfd, inputbuf, MAX_INPUT_SIZE);
    if(rs==0){
        printf("Scheduler ended\n");
        close(sockfd);
        exit(1);
    }
    else if(rs<0){
        fprintf(stderr, "ERROR in reading\n");
        close(sockfd);
        exit(1);
    }
    if(strcmp(inputbuf,"done")){
        fprintf(stderr, "ERROR wrong confirmation recieved done as %s\n",inputbuf);
        exit(1);
    }
    return 0;
}