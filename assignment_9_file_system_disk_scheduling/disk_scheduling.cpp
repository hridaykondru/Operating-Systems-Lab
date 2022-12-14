#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<string.h>
#include <netdb.h>
#include<iostream>
#include <ctype.h>
#include<unistd.h>
#include <pthread.h>
#include<limits.h>
#include <semaphore.h>
#include <chrono>
#include<algorithm>
#include<vector>
#include<unordered_map>
using namespace std;
using namespace std::chrono;
unordered_map<int,milliseconds>req_comp_t;
unordered_map<int,milliseconds>req_arr_t;
unordered_map<int,milliseconds>req_start_t;
vector<int>order;
vector<int>request_queue;
int n_cylinders,n_req,total_dist;
milliseconds start;
void* SSTF(void* _){
    int curr_pos=0,unit_seek_time=1000;
    while (req_comp_t.size()<n_req){
        if(request_queue.size()>0){
            int next_req=request_queue[0],next_pos;
            for(int i=0;i<request_queue.size();++i){
                if(abs(request_queue[i]-curr_pos)<=abs(next_req-curr_pos)){
                    next_req=request_queue[i];
                    next_pos=i;
                }
            }
            request_queue.erase(request_queue.begin() + next_pos);
            milliseconds curr = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
            req_start_t[next_req]=(curr-start);
            cout<<"Request for "<<next_req<<" is chosen as the next request\n";
            usleep(abs(curr_pos-next_req)*unit_seek_time);
            total_dist+=abs(curr_pos-next_req);
            cout<<"Request for "<<next_req<<" has been completed\n";
            order.push_back(next_req);
            curr = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
            req_comp_t[next_req]=curr-start;
            curr_pos=next_req;
        }
        
    }
    return (NULL);
}

void * request_simulator(void *r_list){
    vector<pair<int,int>>request_list=*(vector<pair<int,int>>*)r_list;
    for(auto req:request_list){
        usleep(req.second);
        cout<<"Request for cylinder "<<req.first<<" arrives\n";
        milliseconds curr = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        req_arr_t[req.first]=curr-start;
        request_queue.push_back(req.first);
    }
    return (NULL);
}

int main(){
    cout<<"Enter total number of cylinders: ";
    cin>>n_cylinders;
    cout<<"Enter number of requests: ";
    cin>>n_req;
    total_dist=0;
    vector<pair<int,int>>request_list(n_req);
    cout<<"Enter request <cylinder_no arrival_time>\n";
    for(int i=0;i<n_req;++i){
        cin>>request_list[i].first;
        cin>>request_list[i].second;
    }
    pthread_t SSTF_thread;
    pthread_t request_simulator_thread;
    start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    pthread_create(&SSTF_thread, NULL, SSTF, (void*)NULL); 
    pthread_create(&request_simulator_thread, NULL, request_simulator, (void*)&request_list); 

    pthread_join(request_simulator_thread, NULL);

    pthread_join(SSTF_thread, NULL);
    cout<<"Request sequence: ";
    for(auto p:order){
        cout<<p<<' ';
    }
    cout<<'\n';
    int avg=0;
    for(auto p:order){
        cout<<"Waiting time for request "<<p<<": ";
        avg+=(req_start_t[p]-req_arr_t[p]).count();
        cout<<(req_start_t[p]-req_arr_t[p]).count()<<" microseconds\n";
    }
    cout<<"Average wait time= "<<avg/order.size()<<" microseconds\n";
    avg=0;
    for(auto p:order){
        cout<<"Turn around time for request "<<p<<": ";
        avg+=(req_comp_t[p]-req_arr_t[p]).count();
        cout<<(req_comp_t[p]-req_arr_t[p]).count()<<" microseconds\n";
    }
    cout<<"Average turnaround time="<<avg/order.size()<<" microseconds\n";
    cout<<"Total cylinder movement: "<<total_dist<<'\n';
    return 0;
}