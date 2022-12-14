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
#include<fstream>
#include<math.h>
#include<vector>
#include<string>
using namespace std;
#define char_bytes 1
#define int_bytes 4

typedef struct FILESYSTEM FILESYSTEM; 
typedef struct SUPERBLOCK SUPERBLOCK; 
typedef struct BLOCK1 BLOCK1; 
typedef struct BLOCK BLOCK; 
typedef struct DIRECTORY DIRECTORY; 
typedef struct DIRECTORYENTRY DIRECTORYENTRY; 

struct BLOCK1{
    int* FAT;
};
struct BLOCK{
    string data;
};
struct DIRECTORY{
    unordered_map<string,pair<int,int>>files;
};
struct SUPERBLOCK{
    int disk_block_size, total_filesystem_size,num_blocks,max_str_len;
    string volume_name;
    bool* free_blocks;
    unordered_map<string,DIRECTORY>directories;
    string open_directory,open_file;
};
struct FILESYSTEM{
    SUPERBLOCK super_block;
    BLOCK1 block_1;
    vector<BLOCK> blocks;
};

FILESYSTEM file_system;

int nxt_free_block(){
    for(int i=0;i<file_system.super_block.num_blocks;++i){
        if(file_system.super_block.free_blocks[i]==true){
            return(i);
        }
    }
    return(-1);
}

void my_open(){
    string file_name;
    cin>>file_name;
    if(file_system.super_block.open_directory==""){
        cout<<"Please select directory using my_chdir first\n";
        return;
    }
    if(file_system.super_block.directories[file_system.super_block.open_directory].files.count(file_name)){
        file_system.super_block.open_file=file_name;
        cout<<"File "<<file_name<<" opened\n";
    }
    else{
        int block_no=nxt_free_block();
        if(block_no==-1){
            cout<<"No free space\n";
            return;
        }
        file_system.super_block.open_file=file_name;
        file_system.super_block.free_blocks[block_no]=false;
        file_system.super_block.directories[file_system.super_block.open_directory].files[file_name]={block_no,block_no};
        cout<<"File "<<file_name<<" created and opened\n";
    }
}
void copy_helper_my_open(string file_name){
    if(file_system.super_block.open_directory==""){
        cout<<"Please select directory using my_chdir first\n";
        return;
    }
    if(file_system.super_block.directories[file_system.super_block.open_directory].files.count(file_name)){
        file_system.super_block.open_file=file_name;
        cout<<"File "<<file_name<<" opened\n";
    }
    else{
        int block_no=nxt_free_block();
        if(block_no==-1){
            cout<<"No free space\n";
            return;
        }
        file_system.super_block.open_file=file_name;
        file_system.super_block.free_blocks[block_no]=false;
        file_system.super_block.directories[file_system.super_block.open_directory].files[file_name]={block_no,block_no};
        cout<<"File "<<file_name<<" copied and opened\n";
    }
}
void my_close(){
    file_system.super_block.open_file="";
}
void my_read(){
    if(file_system.super_block.open_directory==""){
        cout<<"Please select directory using my_chdir first\n";
        return;
    }
    if(file_system.super_block.open_file==""){
        cout<<"Please open a file using my_open first\n";
        return;
    }
    int curr_block=file_system.super_block.directories[file_system.super_block.open_directory].files[file_system.super_block.open_file].first;
    while(curr_block!=-1){
        cout<<file_system.blocks[curr_block].data;
        curr_block=file_system.block_1.FAT[curr_block];
    }
    cout<<'\n';
}
void my_write(){
    string inp_data;
    getline(cin,inp_data,'$');
    if(file_system.super_block.open_directory==""){
        cout<<"Please select directory using my_chdir first\n";
        return;
    }
    if(file_system.super_block.open_file==""){
        cout<<"Please open a file using my_open first\n";
        return;
    }
    DIRECTORY *curr_dir=&(file_system.super_block.directories[file_system.super_block.open_directory]);
    if(curr_dir->files[file_system.super_block.open_file].first!=curr_dir->files[file_system.super_block.open_file].second){
        //freeing current held blocks by the file
        int curr_block=file_system.block_1.FAT[curr_dir->files[file_system.super_block.open_file].first];
        while(curr_block!=-1){
            file_system.super_block.free_blocks[curr_block]=true;            
            file_system.blocks[curr_block].data="";
            curr_block=file_system.block_1.FAT[curr_block];
        }
        curr_dir->files[file_system.super_block.open_file].first!=curr_dir->files[file_system.super_block.open_file].second;
        
    }
    int curr_block=curr_dir->files[file_system.super_block.open_file].first;
    file_system.blocks[curr_block].data="";

    for(char c:inp_data){
        if(file_system.blocks[curr_block].data.length()==file_system.super_block.max_str_len){
            int nxt=nxt_free_block();
            if(nxt==-1){
                cout<<"No free space\n";
                return;
            }
            file_system.block_1.FAT[curr_block]=nxt;
            file_system.super_block.free_blocks[nxt]=false;
            curr_dir->files[file_system.super_block.open_file].second=nxt;
            curr_block=nxt;
        }
        file_system.blocks[curr_block].data+=c;
    }
    file_system.block_1.FAT[curr_block]=-1;
}
void copy_helper_my_write(string inp_data){
    if(file_system.super_block.open_directory==""){
        cout<<"Please select directory using my_chdir first\n";
        return;
    }
    if(file_system.super_block.open_file==""){
        cout<<"Please open a file using my_open first\n";
        return;
    }
    DIRECTORY *curr_dir=&(file_system.super_block.directories[file_system.super_block.open_directory]);
    if(curr_dir->files[file_system.super_block.open_file].first!=curr_dir->files[file_system.super_block.open_file].second){
        //freeing current held blocks by the file
        int curr_block=file_system.block_1.FAT[curr_dir->files[file_system.super_block.open_file].first];
        while(curr_block!=-1){
            file_system.super_block.free_blocks[curr_block]=true;
            curr_block=file_system.block_1.FAT[curr_block];
            file_system.blocks[curr_block].data="";
        }
        curr_dir->files[file_system.super_block.open_file].first!=curr_dir->files[file_system.super_block.open_file].second;
        
    }
    int curr_block=curr_dir->files[file_system.super_block.open_file].first;
    file_system.blocks[curr_block].data="";

    for(char c:inp_data){
        if(file_system.blocks[curr_block].data.length()==file_system.super_block.max_str_len){
            int nxt=nxt_free_block();
            if(nxt==-1){
                cout<<"No free space\n";
                return;
            }
            file_system.block_1.FAT[curr_block]=nxt;
            file_system.super_block.free_blocks[nxt]=false;
            curr_dir->files[file_system.super_block.open_file].second=nxt;
            curr_block=nxt;
        }
        file_system.blocks[curr_block].data+=c;
    }
    file_system.block_1.FAT[curr_block]=-1;
}
void my_mkdir(){
    string dirname;
    cin>>dirname;
    file_system.super_block.open_directory="";
    file_system.super_block.open_file="";
    if(file_system.super_block.directories.count(dirname)){
        cout<<"Directory already exists\n";
    }
    else{
        file_system.super_block.directories[dirname]=*new DIRECTORY();
        cout<<"Directory "<<dirname<<" created\n";
    }
    
}
void my_chdir(){
    string dirname;
    cin>>dirname;
    file_system.super_block.open_file="";
    if(file_system.super_block.directories.count(dirname)){
        file_system.super_block.open_directory=dirname;
        cout<<"Directory changed to "<<dirname<<'\n';
    }
    else{
        cout<<"No such directory exists\n";
    }
    
}
void my_rmdir(){
    string dirname;
    cin>>dirname;
    file_system.super_block.open_directory="";
    file_system.super_block.open_file="";
    if(file_system.super_block.directories.count(dirname)){
        if(file_system.super_block.directories[dirname].files.size()==0){
            file_system.super_block.directories.erase(dirname);
            cout<<"Directory "<<dirname<<" removed\n";
        }
        else{
            cout<<"Unable to remove directory because it is not empty\n";
        }
    }
    else{
        cout<<"No such directory exists\n";
    }
    
}
void my_copy(){
    fstream inp_file;
    string fname,inp_str;
    cin>>fname;
	inp_file.open(fname, ios::in);
	if (!inp_file) {
		cout << "File doesn't exist\n";
	}
	else {
		inp_str="";
        char c;
		while (inp_file) {
            if(inp_file.eof()){
                break;
            }
			c=inp_file.get();
			inp_str+=c;
		}

	}
	inp_file.close();
    copy_helper_my_open(fname);
    if(file_system.super_block.open_file==fname){
        copy_helper_my_write(inp_str);
    }
    
}


int main(){
    int fmb,bkb,fn_bytes,bn_bytes;
    cout<<"Enter file system size (in MB): ";
    cin>>fmb;
    cout<<"Enter block size (in KB): ";
    cin>>bkb;
    fn_bytes=fmb*pow(2,20);
    bn_bytes=bkb*pow(2,10);
    file_system=*new FILESYSTEM();
    file_system.super_block=*new SUPERBLOCK();
    file_system.block_1=*new BLOCK1();
    file_system.super_block.total_filesystem_size=fn_bytes;
    file_system.super_block.disk_block_size=bn_bytes;
    file_system.super_block.volume_name="D";
    int n_blocks=fn_bytes/bn_bytes;
    n_blocks-=2;
    file_system.super_block.num_blocks=n_blocks;
    file_system.block_1.FAT=(int*)malloc(sizeof(int)*n_blocks);
    file_system.super_block.max_str_len=(bn_bytes-int_bytes)/char_bytes;
    file_system.super_block.free_blocks=(bool*)malloc(sizeof(bool)*n_blocks);
    file_system.blocks=vector<BLOCK>(n_blocks);
    for(BLOCK& b:file_system.blocks){
        b.data="";
    }
    file_system.super_block.open_directory="";
    file_system.super_block.open_file="";
    for(int i=0;i<n_blocks;++i){
        file_system.super_block.free_blocks[i]=true;
    }
    string command;
    while(1){
        cout<<"->";
        cin>>command;
        if(command=="my_open"){
            my_open();
        }
        else if(command=="my_close"){
            my_close();
        }
        else if(command=="my_read"){
            my_read();
        }
        else if(command=="my_write"){
            my_write();
        }
        else if(command=="my_mkdir"){
            my_mkdir();
        }
        else if(command=="my_chdir"){
            my_chdir();
        }
        else if(command=="my_rmdir"){
            my_rmdir();
        }
        else if(command=="my_copy"){
            my_copy();
        }
        else{
            cout<<"Invalid command\n";
        }
    }
    return 0;
}
