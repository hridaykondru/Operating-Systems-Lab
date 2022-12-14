#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h> 
#include<string.h>
#include <netdb.h>
#include <ctype.h>
#include <pthread.h>
#include<limits.h>
#define MAX_INPUT_SIZE 256

float eval(char* expr){
    char op;
    int n=strlen(expr);
    for(int i=0;i<n;++i){
        if(!isdigit(expr[i]) && expr[i]!=' '){
            op=expr[i];
            break;
        }
    }
    char* p=expr;            
    int num1=atoi(strtok_r(p," +/*-\n",&p));
    int num2=atoi(strtok_r(p," +/*-\n",&p));
    if(op=='+'){
        return(num1+num2);
    }
    else if(op=='-'){
        return(num1-num2);
    }
    else if(op=='*'){
        return(num1*num2);
    }
    else{
        if(num2==0){
            return(__FLT_MAX__);
        }
        else{
            return((float)num1/(float)num2);
        }
    }
}

int main(int argc, char *argv[])
{
int sockfd, portnum, n, optval=1, new_socket, clientsock[20]={0}, max_clients = 20, sockactive;
struct sockaddr_in server_addr;
fd_set rfds;
char inputbuf[MAX_INPUT_SIZE];
if (argc < 2) {
    fprintf(stderr,"usage %s <server-port>\n", argv[0]);
    exit(0);
}
portnum = atoi(argv[1]);
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)
{
    fprintf(stderr, "ERROR opening socket\n");
    exit(1);
}
if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval))) {
    fprintf(stderr, "ERROR in setsockopt\n");
    exit(1);
}
bzero((char *) &server_addr, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(portnum);

if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    fprintf(stderr, "ERROR in binding\n");
    exit(1);
}
if (listen(sockfd, 0) < 0) {
    fprintf(stderr, "ERROR in listening\n");
    exit(1);
}
int addrlen=sizeof(server_addr);
while(1){
    FD_ZERO(&rfds); 
    FD_SET(sockfd, &rfds);  
    int max_fd=sockfd;
    for(int i=0;i<max_clients;++i){
        if(clientsock[i]>0){
            FD_SET(clientsock[i],&rfds);
        }
        if(clientsock[i]>max_fd){
            max_fd=clientsock[i];
        }
    }
    sockactive=select(max_fd+1,&rfds,NULL,NULL,NULL);
    if ((sockactive < 0) && (errno!=EINTR)){
        fprintf(stderr,"ERROR in selecting\n");  
    } 
    if (FD_ISSET(sockfd, &rfds)){
        if ((new_socket = accept(sockfd, (struct sockaddr*)&server_addr, (socklen_t*)&addrlen)) < 0) {
            fprintf(stderr, "ERROR in accepting\n");
            exit(1);
        }
        for (int i = 0; i < max_clients; ++i){
            if( clientsock[i] == 0 ){
                clientsock[i] = new_socket;  
                printf("New Client connected to socket %d\n" , i);                        
                break;  
            }  
        }  
    }
    for(int i=0;i<max_clients;++i){
        int currfd=clientsock[i];
        if(FD_ISSET(currfd,&rfds)){
            bzero(inputbuf,MAX_INPUT_SIZE);
            int rs=read(currfd, inputbuf, MAX_INPUT_SIZE);
            if(rs==0){
                printf("Client socket %d ended connection\n",i);
                close(currfd);
                clientsock[i]=0;
            }
            else if(rs<0){
                fprintf(stderr, "ERROR in reading from client %d\n",i);
                close(currfd);
                clientsock[i]=0;
            }
            else{
                printf("Client socket %d sent message: %s",i,inputbuf);
                float evaled=eval(inputbuf);
                bzero(inputbuf,MAX_INPUT_SIZE);
                sprintf(inputbuf,"%f",evaled);
                printf("Sending reply: %s\n",inputbuf);
                if (write(currfd,inputbuf,strlen(inputbuf)) < 0)
                {
                    fprintf(stderr, "ERROR in writing to client %d\n",i);
                    close(currfd);
                    clientsock[i]=0;
                }
            }
            
        }
    }    
}
return 0;
}