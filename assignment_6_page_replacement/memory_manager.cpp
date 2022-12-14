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
#include <algorithm>
#include <unistd.h>
#include<limits.h>
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include<queue>
using namespace std;

ofstream result_txt;

int fifo(int nframes, int lref_str, int* ref_str){
    unordered_set <int>active_pages;
    queue<int>page_table;
    int page_faults=0;
    for(int i=0;i<lref_str;++i){
        if(!active_pages.count(ref_str[i])){
            ++page_faults;
            if(page_table.size()==nframes){
                //Evict a page
                int victim=page_table.front();
                page_table.pop();
                active_pages.erase(victim);
                page_table.push(ref_str[i]);
                active_pages.insert(ref_str[i]);
            }
            else{
                page_table.push(ref_str[i]);
                active_pages.insert(ref_str[i]);
            }
        }
    }
    return(page_faults);
}

int lru(int nframes, int lref_str, int* ref_str){
    unordered_set <int>active_pages;
    vector<int>lru_queue;
    int page_faults=0;
    for(int i=0;i<lref_str;++i){
        if(!active_pages.count(ref_str[i])){
            ++page_faults;
            if(lru_queue.size()==nframes){
                //Evict a page
                int victim=lru_queue.front();
                lru_queue.erase(lru_queue.begin());
                active_pages.erase(victim);
                lru_queue.push_back(ref_str[i]);
                active_pages.insert(ref_str[i]);
            }
            else{
                lru_queue.push_back(ref_str[i]);
                active_pages.insert(ref_str[i]);
            }
        }
        else{
            lru_queue.erase(find(lru_queue.begin(),lru_queue.end(),ref_str[i]));
            lru_queue.push_back(ref_str[i]);
        }
    }
    return(page_faults);
}

int optimal(int nframes, int lref_str, int* ref_str){
    unordered_set <int>active_pages;
    vector<int>page_table;
    int page_faults=0;
    for(int i=0;i<lref_str;++i){
        if(!active_pages.count(ref_str[i])){
            ++page_faults;
            if(page_table.size()==nframes){
                //Evict a page
                int victim = 0, farthest = i;
                for (int j = 0; j < nframes; ++j) {
                    int k;
                    for (k = i; k < lref_str; ++k) {
                        if (page_table[j] == ref_str[k] && k > farthest) {
                            farthest = k;
                            victim = j;
                            break;
                        }
                    }
                    if (k == lref_str){
                        victim=j;
                        break;
                    }
                }
                //page_table.erase(find(page_table.begin(),page_table.end(),page_table[victim]));
                active_pages.erase(page_table[victim]);
                page_table[victim]=ref_str[i];
                active_pages.insert(ref_str[i]);
            }
            else{
                page_table.push_back(ref_str[i]);
                active_pages.insert(ref_str[i]);
            }
        }
        // for(int j=0;j<nframes;++j){
        //     cout<<page_table[j]<<' ';
        // }
        // cout<<'\n';
    }
    return(page_faults);
}

int main(){
    int nframes,lref_str,*reference_str;
    printf("Enter the maximum number of frames: ");
    scanf("%d", &nframes);
    
    result_txt.open("result.txt");
    for(int i=1;i<=nframes;++i){
        cout<<'\n';
        result_txt<<'\n';
        printf("Enter the length of reference string: ");
        scanf("%d", &lref_str);
        reference_str=(int*)malloc(sizeof(int)*lref_str);
        cout<<"Enter reference string: ";
        for(int j=0;j<lref_str;++j){
            cin>>reference_str[j];
        }
        cout<<"Getting page faults for "<<i<<" number of frames allocated to the process\n";
        result_txt<<"Getting page faults for "<<i<<" number of frames allocated to the process\nWith Reference String:\n";
        for(int j=0;j<lref_str;++j){
            result_txt<<reference_str[j]<<' ';
        }
        result_txt<<'\n';
        int fifo_pf=fifo(i,lref_str,reference_str);
        cout<<"Page faults using FIFO algorithm: "<<fifo_pf<<'\n';
        result_txt<<"Page faults using FIFO algorithm: "<<fifo_pf<<'\n';
        int lru_pf=lru(i,lref_str,reference_str);
        cout<<"Page faults using LRU algorithm: "<<lru_pf<<'\n';
        result_txt<<"Page faults using LRU algorithm: "<<lru_pf<<'\n';
        int optimal_pf=optimal(i,lref_str,reference_str);
        cout<<"Page faults using Optimal algorithm: "<<optimal_pf<<'\n';
        result_txt<<"Page faults using Optimal algorithm: "<<optimal_pf<<'\n';
    }
    // int pg[] = { 0, 1, 2, 3, 0, 1, 4, 0, 1, 2, 3, 4};
	// int pn = sizeof(pg) / sizeof(pg[0]);
	// int fn = 4;
	// optimal(fn,pn,pg);
    return 0;
}