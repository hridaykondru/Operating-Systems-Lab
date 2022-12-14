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
#include <ctype.h>
#include <unistd.h>
#include<limits.h>
#include<unordered_map>
#include<unordered_set>
#include<vector>
using namespace std;

ofstream result_txt;

//For each process
//Valid=1, invalid=0
typedef vector<pair<int,int>> page_table;
//Common
unordered_map<int,int>TLB;
//Common
vector<int>free_frame_list;

int  k,m,f,s;

vector<int> getUniqueRandomNoVector(int start, int end){
    vector<int>rnd_vec;
    for(int i=start;i<=end;++i){
        rnd_vec.push_back(i);
    }
    int n=end-start+1;
    for(int i=0;i<n;++i){
        int temp=rnd_vec[i];
        int rnd_pos=rand()%n;
        rnd_vec[i]=rnd_vec[rnd_pos];
        rnd_vec[rnd_pos]=temp;
    }
    return(rnd_vec);
}

page_table initializeNewPageTable(int mi){
    page_table pt;
    for(int i=0;i<mi;++i){
        pt.push_back({-1,0});
    }
    return(pt);
}

int LRU_pt(vector<int>& ref_str, int curr_pos, page_table& pt){
    unordered_set<int>assigned_pages;
    int i=1;
    for(pair<int,int>fv:pt){
        if(fv.first!=-1 && fv.second!=0){
            assigned_pages.insert(i);
        }
        ++i;
    }
    int victim;
    
    while(assigned_pages.size()>0){
        --curr_pos;
        if(assigned_pages.count(ref_str[curr_pos])){
            assigned_pages.erase(ref_str[curr_pos]);
            victim=ref_str[curr_pos];
        }
    }
    //cout<<"\nvictim: "<<victim<<'\n';
    return(victim);
}

int LRU_TLB(vector<int>& ref_str, int curr_pos){
    unordered_set<int>assigned_pages;
    for(pair<int,int>pf:TLB){
        assigned_pages.insert(pf.first);
    }
    int victim;
    
    while(assigned_pages.size()>0){
        --curr_pos;
        if(assigned_pages.count(ref_str[curr_pos])){
            assigned_pages.erase(ref_str[curr_pos]);
            victim=ref_str[curr_pos];
        }
    }
    //cout<<"\nvictim: "<<victim<<'\n';
    return(victim);
}

void addToTLB(vector<int>& ref_str, int curr_pos, int frame){
    if(TLB.size()==s){
        int victim=LRU_TLB(ref_str,curr_pos);
        TLB.erase(victim);
        TLB[ref_str[curr_pos]]=frame;
    }
    else{
        TLB[ref_str[curr_pos]]=frame;
    }
}

void runProcess(int pid, int mi, int start_frame, int num_frames){
    cout<<"\nP"<<pid<<": Start of execution\n";
    result_txt<<"\nP"<<pid<<": Start of execution\n";
    page_table pt=initializeNewPageTable(mi);
    int ref_str_len=(2*mi)+(rand()%(10*mi)), page_fault_cnt=0;
    vector<int> ref_str;
    for(int i=0;i<ref_str_len;++i){
        ref_str.push_back(1+(rand()%mi));
    }
    cout<<"Reference string: ";
    result_txt<<"Reference string: ";
    for(int i: ref_str){
        cout<<i<<' ';
        result_txt<<i<<' ';
    }
    cout<<'\n';
    result_txt<<'\n';
    for(int i=0;i<ref_str_len;++i){
        //TLB hit
        if(TLB.count(ref_str[i])){
            cout<<"TLB hit: Page "<<ref_str[i]<<" Frame "<<TLB[ref_str[i]]<<'\n';
            result_txt<<"TLB hit: Page "<<ref_str[i]<<" Frame "<<TLB[ref_str[i]]<<'\n';
        }
        //Search page table
        else{
            //Page fault
            if(pt[ref_str[i]-1].first==-1 || pt[ref_str[i]-1].second==0){
                ++page_fault_cnt;
                cout<<"TLB miss with page fault on page: "<<ref_str[i]<<'\n';
                result_txt<<"TLB miss with page fault on page: "<<ref_str[i]<<'\n';
                //Check if free frame available
                bool free_available=false;
                for(int fcnt=0;fcnt<num_frames;++fcnt){
                    if(free_frame_list[(start_frame+fcnt)%f]){
                        //cout<<'\n'<<free_frame_list[(start_frame+fcnt)%f]<<'\n';
                        pt[ref_str[i]-1].first=((start_frame+fcnt)%f)+1;
                        pt[ref_str[i]-1].second=1;
                        free_available=true;
                        free_frame_list[(start_frame+fcnt)%f]=0;
                        break;
                    }
                }
                if(!free_available){
                    int victim=LRU_pt(ref_str,i,pt);
                    //Setting page table entry of victim page as invalid
                    if(TLB.count(victim)){
                        TLB.erase(victim);
                    }
                    pt[victim-1].second=0;
                    pt[ref_str[i]-1].first=pt[victim-1].first;
                    pt[ref_str[i]-1].second=1;
                }
                cout<<"Page "<<ref_str[i]<<" now assigned to frame "<<pt[ref_str[i]-1].first<<'\n';
                result_txt<<"Page "<<ref_str[i]<<" now assigned to frame "<<pt[ref_str[i]-1].first<<'\n';
            }
            //No page fault
            else{
                cout<<"TLB miss with no page fault on page: "<<ref_str[i]<<" frame: "<<pt[ref_str[i]-1].first<<'\n';
                result_txt<<"TLB miss with no page fault on page: "<<ref_str[i]<<" frame: "<<pt[ref_str[i]-1].first<<'\n';
            }
            //Adding entry to TLB
            addToTLB(ref_str,i,pt[ref_str[i]-1].first);
        }
    }
    cout<<"P"<<pid<<": Page fault count= "<<page_fault_cnt<<'\n';
    result_txt<<"P"<<pid<<" : Page fault count= "<<page_fault_cnt<<'\n';
    cout<<"P"<<pid<<": End of execution\n";
    result_txt<<"P"<<pid<<": End of execution\n";
}

void zeroOutFrames(int start, int num_frames){
    for(int i=0;i<num_frames;++i){
        free_frame_list[(start+i)%f]=1;
    }
}


int main(int argc, char *argv[]){
    result_txt.open("result.txt");
    if (argc < 5) {
        fprintf(stderr,"Please enter the following arguements: <Total no. of processes> <max pages required per process> <total number of frames in main memory> <size of TLB>\n");
        result_txt<<"Please enter the following arguements: <Total no. of processes> <max pages required per process> <total number of frames in main memory> <size of TLB>\n";
        result_txt.close();
        exit(0);
    }
    k=atoi(argv[1]);
    m=atoi(argv[2]);
    f=atoi(argv[3]);
    s=atoi(argv[4]);

    if(f>=m || s>=f){
        fprintf(stderr,"Please ensure that 'max pages required per process' > 'total number of frames in main memory' and 'total number of frames in main memory' > 'size of TLB'\n");
        result_txt<<"Please ensure that 'max pages required per process' > 'total number of frames in main memory' and 'total number of frames in main memory' > 'size of TLB'\n";
        result_txt.close();
        exit(0);
    }

    int total_pages=0;
    for(int i=0;i<f;++i){
        free_frame_list.push_back(1);
    }
    //Vector of pids
    vector<int>pid_vect=getUniqueRandomNoVector(1,k);
    //Num of pages required by process i
    vector<int>mi_vect;
    //Num of frames allocated for process i (allocated proportionally to mi)
    vector<int>fi_vect;
    //Frame start address for process i
    vector<int>start_i_vect;
    for(int i=0;i<k;++i){
        int mi=1+(rand()%m);
        mi_vect.push_back(mi);
        total_pages+=mi;
    }
    int i=0;
    for(int mi: mi_vect){
        int fi=max((mi*f)/total_pages,1);
        fi_vect.push_back(fi);
        cout<<"Allocating "<<fi<<" frames for process with pid "<<pid_vect[i]<<" which needs "<<mi<<" pages totally\n";
        result_txt<<"Allocating "<<fi<<" frames for process with pid "<<pid_vect[i]<<" which needs "<<mi<<" pages totally\n";
        ++i;
    }
    start_i_vect.push_back(0);
    for(int i=0;i<k-1;++i){
        start_i_vect.push_back((start_i_vect[i]+fi_vect[i])%f);
    }
    for(int i=0;i<k;++i){
        //Reloading TLB
        TLB.clear();
        //Zeroing out frames assigned to the process
        zeroOutFrames(start_i_vect[i],fi_vect[i]);
        //Running the process
        runProcess(pid_vect[i],mi_vect[i],start_i_vect[i],fi_vect[i]);
    }
    result_txt.close();
    return 0;
}