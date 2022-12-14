#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<string.h>
#include <dirent.h>
#include<unistd.h> 
#include <limits.h>
#include<sys/wait.h>
#include<vector>
#include <fcntl.h>
using namespace std;


void execute(char* command_full, char** history_arr,int tot, char * envp[]){
    char check_background=command_full[strlen(command_full)-2];
    bool background=false;
    if(check_background=='&'){
        command_full[strlen(command_full)-2]='\0';
        background=true;
    }
    char* p=command_full;            
    char* command=strtok_r(p," \n",&p);
    if(!strcmp(command,"clear")){
        system("clear");
        
    }
    else if(!strcmp(command,"env")){
        for(int i=0;envp[i]!=NULL;++i){
            printf("%s\n",envp[i]);
        }
        
    }
    else if(!strcmp(command,"cd")){
        char* dir=strtok_r(p," \n",&p);
        if(dir==NULL){
            dir="/home";
        }
        int error=chdir(dir);
        if(error){
            printf("No such file or directory\n");
        }
        else{
            printf("Changed directory to %s\n",dir);
        }
        
    }
    else if(!strcmp(command,"pwd")){
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n",cwd);
    }
    else if(!strcmp(command,"mkdir")){
        char* dir=strtok_r(p," \n",&p);
            int error=mkdir(dir,0777);
        if(error){
            printf("Unable to create directory\n");
        }
        else{
            printf("Created directory %s\n",dir);
        }
        
    }
    else if(!strcmp(command,"rmdir")){
        char* dir=strtok_r(p," \n",&p);
            int error=rmdir(dir);
        if(error){
            printf("Unable to remove directory\n");
        }
        else{
            printf("Removed directory %s\n",dir);
        }
        
    }
    else if(!strcmp(command,"ls")){
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d){
            while ((dir = readdir(d)) != NULL){
                printf("%s\n", dir->d_name);
            }
            closedir(d);
        }
        
    }
    else if(!strcmp(command,"history")){
        char* n=strtok_r(p," \n",&p);
        int num;
        if(n!=NULL){
            num=atoi(n);
            num=min(num,tot);
        }
        else{
        num=tot;
        }
        for(int i=1;i<=num;++i){
        printf("%s\n",history_arr[tot-i]);
        }
    }

    else if(!strcmp(command,"exit")){
        exit(0);
    }

    else{            
        printf("%s\n",p);
        int fork_val=fork();
        if(fork_val>0 ){
            wait(NULL);
        }
        else if(fork_val>0 && background){
            printf("%d\n",fork_val);
        }
        else if(fork_val==0){
            char* arghld[1000]={NULL};
            int argc=1;
            arghld[0]=command;
            char* arg=strtok_r(p," \n",&p);
            while(arg!=NULL){
                if(argc==1000){
                    printf("Command length has exceeded maximum limit of 1000 words\n");
                    exit(-1);
                }
                arghld[argc]=(char*)malloc(sizeof(char)*strlen(arg));
                strcpy(arghld[argc],arg);
                ++argc;                    
                arg=strtok_r(p," \n",&p);
            }
            execvp(arghld[0],arghld);
            printf("%s: command not found\n",command);
            exit(-1);
        }
        else{
            printf("Unknown error\n");
        }
    }
}

int main(int argc, char *argv[], char * envp[]){
    char command_full[4500];
    char* history_arr[100000];
    int tot=0;
    //Creating child shell process to execute script
    if(argc>1){
        int fork_status=fork();
        if(fork_status==0){
            int fileid=open(argv[1],O_RDONLY | O_CREAT);
            dup2(fileid, 0);
            close(fileid);
        } 
        else if(fork_status>0){
            wait(NULL);
        }  
        else{
            printf("Error executing script\n");
        }     
    }
    while(1){
        printf("custom_shell$ ");
        char* status=fgets(command_full, 4500, stdin);
        
        if(status==NULL){
            exit(0);
        }
        if(strlen(command_full)==1){
            continue;
        }
        int n=strlen(command_full)-1;
        //Adding command to history       
        history_arr[tot]=(char*)malloc(sizeof(char)*strlen(command_full)-1);
        strncpy(history_arr[tot],command_full,strlen(command_full)-1);
        ++tot;
        execute(command_full,history_arr,tot,envp);         
        
    }
    
    return(0);
}