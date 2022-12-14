#include <iostream>
#include<vector>
using namespace std;

bool bankers(vector<vector<int>>&allocation,vector<vector<int>>&max,vector<int>available,int n, int m){
    vector<vector<int>>need(n,vector<int>(m));
    for(int i=0;i<n;++i){
        for(int j=0;j<m;++j){
            need[i][j]=max[i][j]-allocation[i][j];
        }
    }
    
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            bool feasible=true;
            for(int k=0;k<m;++k){
                if(need[j][k]==-1 || available[k]-need[j][k]<0){
                    feasible=false;
                    break;
                }
            }
            if(feasible){
                for(int k=0;k<m;++k){
                    available[k]+=allocation[j][k];
                    need[j][k]=-1;
                }
                break;
            }
        }
    }
    bool safe=true;
    for(int i=0;i<n;++i){
        if(need[i][0]!=-1){
            safe=false;
            break;
        }
    }
    return(safe);
}
 
int main(){ 
    int n, m;
    cout<<"Enter number of processes: ";
    cin>>n; 
    cout<<"Enter number of resources: ";
    cin>>m; 
    vector<vector<int>>allocation(n,vector<int>(m));
    vector<vector<int>>max(n,vector<int>(m));
    vector<int>available(m);
    cout<<"Enter available: ";
    for(int i=0;i<m;++i){
        cin>>available[i];
    }
    for(int i=0;i<n;++i){
        cout<<"Enter allocation for P"<<i<<": ";
        for(int j=0;j<m;++j){
            cin>>allocation[i][j];
        }
    }
    for(int i=0;i<n;++i){
        cout<<"Enter max for P"<<i<<": ";
        for(int j=0;j<m;++j){
            cin>>max[i][j];
        }
    }
    vector<int>request(m);
    int request_process;
    cout<<"Enter process number of process requesting for resources: ";
    cin>>request_process;
    cout<<"Enter the number of resources requested by P"<<request_process<<": ";
    for(int i=0;i<m;++i){
        cin>>request[i];
    }
    if(bankers(allocation,max,available,n,m)){
        cout<<"Current allocation is safe\n";
    }
    else{
        cout<<"Current allocation is not safe, hence, allocation of the new request is not possible\n";
        return(0);
    }
    for(int i=0;i<m;++i){
        allocation[request_process][i]+=request[i];
        available[i]-=request[i];
        if(allocation[request_process][i]>max[request_process][i] ){
            cout<<"New request from P"<<request_process<<" is not possible because it exceeds the max that can be requested by it.\n";
            return(0);
        }
        else if(available[i]<0 ){
            cout<<"New request from P"<<request_process<<" is not possible because it exceeds the available resources.\n";
            return(0);
        }
        
    }
    if(bankers(allocation,max,available,n,m)){
        cout<<"The new request is safe and can be accomodated\n";
    }
    else{
        cout<<"The new request leads to an unsafe state and cannot be accomodated.\n";
        return(0);
    }
    return (0);
}