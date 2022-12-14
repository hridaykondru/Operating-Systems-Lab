#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<string.h>
#include <netdb.h>
#include <ctype.h>
#include<unistd.h>
#include <pthread.h>
#include<limits.h>
#include <semaphore.h>
#include<stdatomic.h>

sem_t rw_lock, r_lock, sync_lock;
atomic_int r_count, consecutive_r_count, w_count;



void* writer(void *wno){
    int no=*(int*)wno;
    while (1){

        sem_wait(&sync_lock);
        sem_wait(&rw_lock); 
        w_count++;            
        
        /* writing is performed */ 
        printf("Writer %d is writing\n",no);
        sleep(2);        
        printf("Writer %d finished writing\n",no);
        --w_count;
        sem_post(&rw_lock); 
        sem_post(&sync_lock); 
        sleep(1);
    }
    return NULL;
}

void *reader(void *rno){
    int no=*(int*)rno;
    while (1){
        sem_wait(&sync_lock);
        
        if (r_count == 0){ // first reader            
            sem_wait(&rw_lock); 
        }
        r_count++;            
        sem_post(&sync_lock); 
        
        /* reading is performed */ 
        printf("Reader %d is reading\n",no);
        sleep(2);
        
        sem_wait(&r_lock);
        
        --r_count;
        printf("Reader %d finished reading\n",no);
        ++consecutive_r_count;
        if (r_count == 0){/* last reader */
            printf("Reader %d was the last reader\n",no);
            sem_post(&rw_lock); 
        }
        sem_post(&r_lock); 
        sleep(1);
    }
    return NULL;
}

int main(){
    int n_readers=5, n_writers=5;
    int rno[]={0,1,2,3,4}, wno[]={0,1,2,3,4};
    pthread_t reader_thread[n_readers];
    pthread_t writer_thread[n_writers];
    sem_init(&rw_lock,0,1);
    sem_init(&sync_lock,0,1);
    sem_init(&r_lock,0,1);
    r_count=0;
    w_count=0;
    consecutive_r_count=0;
    for(int i=0;i<n_readers;++i){
        sleep(1);
        pthread_create(&reader_thread[i], NULL, reader, (void*)&rno[i]); 
    }
    for(int i=0;i<n_writers;++i){
        sleep(1);
        pthread_create(&writer_thread[i], NULL, writer, (void*)&wno[i]); 
    }
    for (int i = 0; i < n_readers; ++i){ 
        pthread_join(reader_thread[i], NULL);
    }
    for (int i = 0; i < n_writers; ++i){ 
        pthread_join(writer_thread[i], NULL);
    }
    return 0;
}
