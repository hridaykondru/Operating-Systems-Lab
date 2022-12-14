#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<string.h>
#include <netdb.h>
#include <ctype.h>
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
int sockfd, portnum, n, optval=1, new_socket;
struct sockaddr_in server_addr;
char inputbuf[MAX_INPUT_SIZE];
int portcnt=0;

if (argc < 2) {
    fprintf(stderr,"usage %s <server-port>\n", argv[0]);
    exit(0);
}
portnum = atoi(argv[1]);

while(1){
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
    if ((new_socket = accept(sockfd, (struct sockaddr*)&server_addr, (socklen_t*)&addrlen)) < 0) {
        fprintf(stderr, "ERROR in accepting\n");
        exit(1);
    }
    else{
        ++portcnt;
        printf("New Client connected to socket %d\n",portcnt);
        
    }
    close(sockfd);
    do
    {
        bzero(inputbuf,MAX_INPUT_SIZE);
        int rs=read(new_socket, inputbuf, MAX_INPUT_SIZE);
        if(rs==0){
            printf("Client socket %d ended connection\n",portcnt);
            close(new_socket);
            break;
        }
        else if(rs<0){
            fprintf(stderr, "ERROR in reading\n");
            close(new_socket);
            exit(1);
        }
        printf("Client socket %d sent message: %s",portcnt,inputbuf);
        float evaled=eval(inputbuf);
        bzero(inputbuf,MAX_INPUT_SIZE);
        sprintf(inputbuf,"%f",evaled);
        printf("Sending reply: %s\n",inputbuf);
        if (write(new_socket,inputbuf,strlen(inputbuf)) < 0)
        {
            fprintf(stderr, "ERROR in writing\n");
            close(new_socket);
            exit(1);
        }
        
    } while(1);
}



return 0;
}