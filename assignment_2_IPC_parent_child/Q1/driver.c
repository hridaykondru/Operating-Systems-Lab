#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<string.h>
#include <dirent.h>
#include<unistd.h>
#include <limits.h>
#include<sys/wait.h>
#include <fcntl.h>
int main(int argc,char* argv[]){
    if(argc<=2){
        printf("Please enter the arguments in the order: <input_file_name> <output_file_name>\n");
        exit(0);
    }
    int fd_in = open(argv[1], O_RDONLY);
    int pfd[2];
    if(pipe(pfd)==-1){
        printf("Error occured while creating the pipe\n");
        exit(-1);
    }
    if(fd_in==-1){
        printf("Error occured while reading the file\n");
        exit(-1);
    }
    int fd_out=creat(argv[2],0644);
    if(fd_out==-1){
        printf("Error occured creating the file\n");
        exit(-1);
    }
    int fork1=fork();
    if(fork1==-1){
        printf("Error occured creating the child process\n");
        exit(-1);
    }
    else if(fork1!=0){
        int fork2=fork();
        if(fork2==-1){
            printf("Error occured creating the child process\n");
            exit(-1);
        }
        else if(fork2!=0){
            close(fd_in);
            close(fd_out);
            close(pfd[0]);
            close(pfd[1]);
            wait(NULL);
            wait(NULL);
            
        }        
        else{
            //Convert
            close(pfd[1]);
            dup2(pfd[0],STDIN_FILENO);
            close(pfd[0]);
            dup2(fd_out,STDOUT_FILENO);
            close(fd_out);
            execl("convert","convert",(char*)0);
        }
    }
    else{
        //Count
        dup2(fd_in,STDIN_FILENO);
        close(fd_in);
        close(pfd[0]);
        dup2(pfd[1],STDOUT_FILENO);
        close(pfd[1]);
        execl("count","count",(char*)0);
    }
    return(0);
}
