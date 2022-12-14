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
#define thinking 0
#define hungry 1
#define eating 2
 
sem_t pickup_lock;
sem_t *chopsticks;
int *philosopher_state;


void pickup (int i) { 
    philosopher_state[i] = hungry;
    sem_wait(&pickup_lock);
    sem_wait(&chopsticks[i]);
    sem_wait(&chopsticks[(i+1)%5]);
    philosopher_state[i]=eating;
    printf("Philosopher %d took the chopsticks %d, %d and started eating\n",i+1,(i)+1,((i+1)%5)+1);
    sem_post(&pickup_lock);
}
	
void putdown (int i) { 
    philosopher_state[i] = thinking;
    sem_post(&chopsticks[i]);
    sem_post(&chopsticks[(i+1)%5]);
    printf("Philosopher %d put down the chopsticks %d, %d\n",i+1,(i)+1,((i+1)%5)+1);
}

void *philosopher(void *pno)
{
    int i=*(int*)pno;
    while(1){
        printf("Philosopher %d is thinking\n",i+1);
        sleep(1);
        pickup(i);
        sleep(1);
        putdown(i);

    }
    return NULL;
}



int main(){
    int n_philosophers=5;
    int pno[]={0,1,2,3,4};
    chopsticks=(sem_t*)malloc(sizeof(sem_t)*n_philosophers);
    philosopher_state=(int*)malloc(sizeof(int)*n_philosophers);
    pthread_t philosopher_thread[n_philosophers];
    //Initially lock available
    sem_init(&pickup_lock,0,1);
    for(int i=0;i<n_philosophers;++i){
        //Initially chopsticks available
        sem_init(&chopsticks[i],0,1);
        philosopher_state[i]=thinking;
    }
    for(int i=0;i<n_philosophers;++i){
        pthread_create(&philosopher_thread[i], NULL, philosopher, (void*)&pno[i]); 
    }
    for (int i = 0; i < n_philosophers; ++i){ 
        pthread_join(philosopher_thread[i], NULL);
    }
    
    return 0;
}
