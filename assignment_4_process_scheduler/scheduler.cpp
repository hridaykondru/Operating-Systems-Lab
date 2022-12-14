#include <stdio.h>
#include<iostream>
#include <list>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<signal.h>
#include<string.h>
#include <netdb.h>
#include <fstream>
#include <chrono>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include<limits.h>
#include<unordered_map>
using namespace std;
using namespace std::chrono;
#define MAX_INPUT_SIZE 256
#define sigexpired 7
#define sigrun 8

list<pair<int,int>>ready_queue;
pair<int,int> running_process=make_pair(-1,-1);
unordered_map<int,int>pid_pno_map;
unordered_map<int,int64_t>pid_wait;
unordered_map<int,int64_t>pid_turnaround;
unordered_map<int,int64_t>pid_response;
//bool first_process_flag=false;
int process_cnt=-1;

struct args{
    int portcnt;
    int new_socket;
};

void *threadFunc(void *vargp){    
    struct args* arghlder=(struct args*)vargp;
    int portcnt=arghlder->portcnt;
    int new_socket=arghlder->new_socket;
    char inputbuf[MAX_INPUT_SIZE];
    //Read pid
    bzero(inputbuf,MAX_INPUT_SIZE);
    int rs=read(new_socket, inputbuf, MAX_INPUT_SIZE);
    if(rs==0){
        printf("Process %d ended connection\n",portcnt);
        close(new_socket);
        return NULL;
    }
    else if(rs<0){
        fprintf(stderr, "ERROR in reading\n");
        close(new_socket);
        return NULL;
    }
    
    int pid=atoi(inputbuf);    
    printf("Process %d sent pid: %d\n",portcnt,pid);
    //Send confirmation
    bzero(inputbuf,MAX_INPUT_SIZE);
    sprintf(inputbuf,"pid");
    int n = write(new_socket,inputbuf,strlen(inputbuf));
    if (n < 0){
        fprintf(stderr, "ERROR sending confirmation");
        return NULL;
    }
    //Read priority
    bzero(inputbuf,MAX_INPUT_SIZE);    
    rs=read(new_socket, inputbuf, MAX_INPUT_SIZE);
    if(rs==0){
        printf("Process %d ended connection\n",portcnt);
        close(new_socket);
        return NULL;
    }
    else if(rs<0){
        fprintf(stderr, "ERROR in reading\n");
        close(new_socket);
        return NULL;
    }
    printf("Process %d sent priority: %s\n",portcnt,inputbuf);
    int priority=atoi(inputbuf);

    //Add -(arrival_time) to pid_wait, pid_turnaround, pid_response for pid
    int64_t arrival_time=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    pid_wait[pid]-=arrival_time;
    pid_turnaround[pid]-=arrival_time;
    pid_response[pid]-=arrival_time;

    ready_queue.push_back(make_pair(priority,pid));
    //first_process_flag=true;
    if(process_cnt==-1){
        process_cnt=1;
    }
    else{
        ++process_cnt;
    }

    pid_pno_map[pid]=portcnt;
     //Send confirmation
    bzero(inputbuf,MAX_INPUT_SIZE);
    sprintf(inputbuf,"priority");
    n = write(new_socket,inputbuf,strlen(inputbuf));
    if (n < 0){
        fprintf(stderr, "ERROR sending confirmation\n");
        return NULL;
    }
    do
    {
        bzero(inputbuf,MAX_INPUT_SIZE);
        rs=read(new_socket, inputbuf, MAX_INPUT_SIZE);
        if(rs==0){
            printf("Process %d ended connection\n",portcnt);
            close(new_socket);
            break;
        }
        else if(rs<0){
            fprintf(stderr, "ERROR in reading\n");
            close(new_socket);
            return NULL;
        }
        //Send confirmation
        //bzero(inputbuf,MAX_INPUT_SIZE);
        //sprintf(inputbuf,"msggot");
        n = write(new_socket,inputbuf,strlen(inputbuf));
        if (n < 0){
            fprintf(stderr, "ERROR sending confirmation\n");
            return NULL;
        }

        if(!strcmp(inputbuf,"io")){
            //Remove from cpu
            printf("P%d %d requests I/O\n",portcnt,pid);
            if(running_process.second==pid){
                //first_process_flag=false;
                running_process.first=-1;
                running_process.second=-1;
                //Subtracting current time from pid_wait for pid
                int64_t current_time=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
                pid_wait[pid]-=current_time;
            }            
        }
        else if(!strcmp(inputbuf,"iod")){
            //Put into ready queue
            ready_queue.push_back(make_pair(priority,pid));
            //first_process_flag=true;
            printf("P%d %d completes I/O\n",portcnt,pid);
        }
        else if(!strcmp(inputbuf,"done")){
            //Remove from ready queue
            printf("P%d %d completes execution successfully\n",portcnt,pid);
            int64_t completion_time=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            if(running_process.second==pid){
                running_process.first=-1;
                running_process.second=-1;
            }  
            //Adding completion time to pid_turnaround for pid
            pid_turnaround[pid]+=completion_time;
            --process_cnt;
            close(new_socket);
            return NULL;
        }
        
    } while(1);
    return NULL;
}

void *RRthread(void *vargp){
    int64_t process_start_time, current_time;
    while(1){
        current_time=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        //cout<<ready_queue.size()<<'\n';
        //No process running
        if(running_process.first==-1 && running_process.second==-1 && ready_queue.size()!=0){   
            running_process.first=ready_queue.front().first;
            running_process.second=ready_queue.front().second;
            ready_queue.pop_front();
            //Adding current time if never added previously to pid_response for pid
            //This gives the time after which the process first gets hold of the CPU
            if(pid_response[running_process.second]<0){                
                pid_response[running_process.second]+=current_time;
            }
            //Adding current time to pid_wait for pid 
            pid_wait[running_process.second]+=current_time;

            kill(running_process.second,sigrun);
            printf("P%d %d started\n",pid_pno_map[running_process.second],running_process.second);
            process_start_time=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        }
        //A process is running
        else if(running_process.first!=-1 && running_process.second!=-1){            
            //Process executed for more than 10 sec ~ 1000 iterations
            if(current_time-process_start_time>=10200){

                //Subtracting current time to pid_wait for pid 
                pid_wait[running_process.second]-=current_time;

                kill(running_process.second,sigexpired);                  
                ready_queue.push_back(make_pair(running_process.first,running_process.second));
                usleep(100000);         
                printf("P%d %d expired\n",pid_pno_map[running_process.second], running_process.second);
                running_process.first=-1;
                running_process.second=-1;
            }
        }
        //Ready queue is empty and no process is running, then print analysis and exit
        else if(running_process.first==-1 && running_process.second==-1 && ready_queue.size()==0 && process_cnt==0){
            ofstream result;
            int64_t avg_wait=0, avg_turnaround=0, avg_response=0, no_proc=0;
            result.open("result_RR.txt");
            result<<"Scheduler used: RR\nNote 1: Total waiting time includes time spent on I/O\nNote 2: All times are in seconds\n";
            for(pair<int,int> p:pid_pno_map){
                ++no_proc;
                avg_wait+=pid_wait[p.first];
                avg_turnaround+=pid_turnaround[p.first];
                avg_response+=pid_response[p.first];
                result<<"P"<<p.second<<' '<<p.first<<" Response time: "<<pid_response[p.first]/1000<<'\n';
                result<<"P"<<p.second<<' '<<p.first<<" Total waiting time: "<<pid_wait[p.first]/1000<<'\n';
                result<<"P"<<p.second<<' '<<p.first<<" Turn-around time: "<<pid_turnaround[p.first]/1000<<'\n';
            }
            avg_wait=avg_wait/(no_proc*1000);
            avg_turnaround=avg_turnaround/(no_proc*1000);
            avg_response=avg_response/(no_proc*1000);
            result<<"Average Response time: "<<avg_response<<'\n';
            result<<"Average Total waiting time: "<<avg_wait<<'\n';
            result<<"Average Turn-around time: "<<avg_turnaround<<'\n';
            result.close();
            break;
        }
    }
    printf("RR scheduled done and terminated\n");
    exit(0);
}

void *PRRthread(void *vargp){
    int64_t process_start_time, current_time;
    while(1){
        current_time=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        //No process running
        if(running_process.first==-1 && running_process.second==-1 && ready_queue.size()!=0){  
            //Searching for process with highest priority (lowest priority value)
            list<pair<int,int>>::iterator min_priority_elem=ready_queue.begin();
            for (list<pair<int,int>>::iterator itr = ready_queue.begin(); itr != ready_queue.end(); ++itr) {
                if(itr->first<min_priority_elem->first){
                    min_priority_elem=itr;
                }
            }          
            running_process.first=min_priority_elem->first;
            running_process.second=min_priority_elem->second;
            ready_queue.erase(min_priority_elem);

            //Adding current time if never added previously to pid_response for pid
            //This gives the time after which the process first gets hold of the CPU
            if(pid_response[running_process.second]<0){
                pid_response[running_process.second]+=current_time;
            }

            //Adding current time to pid_wait for pid 
            pid_wait[running_process.second]+=current_time;

            kill(running_process.second,sigrun);
            printf("P%d %d started\n",pid_pno_map[running_process.second],running_process.second);
            process_start_time=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        }
        //A process is running
        else if(running_process.first!=-1 && running_process.second!=-1){            
            //Process executed for more than 20 sec ~ 2000 iterations
            if(current_time-process_start_time>=20300){

                //Subtracting current time to pid_wait for pid 
                pid_wait[running_process.second]-=current_time;

                kill(running_process.second,sigexpired);                  
                ready_queue.push_back(make_pair(running_process.first,running_process.second));
                usleep(100000);         
                printf("P%d %d expired\n",pid_pno_map[running_process.second], running_process.second);
                running_process.first=-1;
                running_process.second=-1;
            }
        }
        //Ready queue is empty and no process is running, then print analysis and exit
        else if(running_process.first==-1 && running_process.second==-1 && ready_queue.size()==0 && process_cnt==0){
            ofstream result;
            int64_t avg_wait=0, avg_turnaround=0, avg_response=0, no_proc=0;
            result.open("result_PRR.txt");
            result<<"Scheduler used: PRR\nNote 1: Total waiting time includes time spent on I/O\nNote 2: All times are in seconds\n";
            for(pair<int,int> p:pid_pno_map){
                ++no_proc;
                avg_wait+=pid_wait[p.first];
                avg_turnaround+=pid_turnaround[p.first];
                avg_response+=pid_response[p.first];
                result<<"P"<<p.second<<' '<<p.first<<" Response time: "<<pid_response[p.first]/1000<<'\n';
                result<<"P"<<p.second<<' '<<p.first<<" Total waiting time: "<<pid_wait[p.first]/1000<<'\n';
                result<<"P"<<p.second<<' '<<p.first<<" Turn-around time: "<<pid_turnaround[p.first]/1000<<'\n';
            }
            avg_wait=avg_wait/(1000*no_proc);
            avg_turnaround=avg_turnaround/(1000*no_proc);
            avg_response=avg_response/(1000*no_proc);
            result<<"Average Response time: "<<avg_response<<'\n';
            result<<"Average Total waiting time: "<<avg_wait<<'\n';
            result<<"Average Turn-around time: "<<avg_turnaround<<'\n';
            result.close();
            break;
        }
    }
    printf("PRR scheduled done and terminated\n");
    exit(0);
}

int main(int argc, char *argv[])
{
int sockfd, portnum, n, optval=1, new_socket;
struct sockaddr_in server_addr;
struct args* arghlder;
arghlder=(struct args*)malloc(sizeof(struct args));
int portcnt=0;
if (argc < 2) {
    fprintf(stderr,"usage %s <RR/PRR>\n", argv[0]);
    exit(0);
}
char* algo=argv[1];
portnum = 654321;
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
pthread_t RRthread_id,PRRthread_id;
if(!strcmp(algo,"RR")){
    pthread_create(&RRthread_id, NULL, RRthread, (void*)NULL);
}
else if(!strcmp(algo,"PRR")){
    pthread_create(&PRRthread_id, NULL, PRRthread, (void*)NULL);
}
while(1){
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
        printf("New Process connected to socket %d\n",portcnt);
        pthread_t thread_id,RRthread_id,PRRthread_id;
        arghlder->new_socket=new_socket;
        arghlder->portcnt=portcnt;
        pthread_create(&thread_id, NULL, threadFunc, (void*)arghlder);
        
    }

    
}



return 0;
}
