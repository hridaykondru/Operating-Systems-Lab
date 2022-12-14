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
#define preempted 3
 
sem_t *chopsticks;
int *philosopher_state;
int* chopstick_allocation;
int n_philosophers;

void pickup (int i) { 
    philosopher_state[i] = hungry;
    sem_wait(&chopsticks[i]);
    chopstick_allocation[i]=i;
    printf("Philosopher %d took chopstick %d\n",i+1,(i)+1);
    sem_wait(&chopsticks[(i+1)%5]);
    chopstick_allocation[(i+1)%5]=i;
    if(philosopher_state[i]==preempted){
        return;
    }
    printf("Philosopher %d took chopstick %d\n",i+1,((i+1)%5)+1);
    philosopher_state[i]=eating;
    printf("Philosopher %d started eating\n",i+1);
}
	
void putdown (int i) { 
    chopstick_allocation[i]=-1;
    sem_post(&chopsticks[i]);
    chopstick_allocation[(i+1)%5]=-1;
    sem_post(&chopsticks[(i+1)%5]);

    if(philosopher_state[i]!=preempted){
        printf("Philosopher %d put down the chopsticks %d, %d\n",i+1,(i)+1,((i+1)%5)+1);
    }
    philosopher_state[i] = thinking;
}

void *philosopher(void *pno)
{
    int i=*(int*)pno;
    while(1){
        printf("Philosopher %d is thinking\n",i+1);
        sleep(rand()%2);
        pickup(i);
        sleep(rand()%6);
        putdown(i);

    }
    return NULL;
}

void *check_deadlock()
{
    while(1){
        int deadlock=1;
        for(int i=0;i<n_philosophers;++i){
            if(chopstick_allocation[i]!=i){
                deadlock=0;
            }
        }
        int preempted_process=0;
        if(deadlock){
            philosopher_state[preempted_process]=preempted;
            sem_post(&chopsticks[preempted_process]);
            printf("Deadlock detected\nProcess %d preempted\n",preempted_process+1);


        }        
        sleep(10);
    }
    return NULL;
}

int main(){
    n_philosophers=5;
    int pno[]={0,1,2,3,4};
    chopstick_allocation=(int*)malloc(sizeof(int)*n_philosophers);
    chopsticks=(sem_t*)malloc(sizeof(sem_t)*n_philosophers);
    philosopher_state=(int*)malloc(sizeof(int)*n_philosophers);
    pthread_t philosopher_thread[n_philosophers];
    for(int i=0;i<n_philosophers;++i){
        //Initially chopsticks available
        sem_init(&chopsticks[i],0,1);
        philosopher_state[i]=thinking;
        chopstick_allocation[i]=-1;
    }
    for(int i=0;i<n_philosophers;++i){
        pthread_create(&philosopher_thread[i], NULL, philosopher, (void*)&pno[i]); 
    }
    pthread_t check_deadlock_thread;
    pthread_create(&check_deadlock_thread,NULL, check_deadlock, (void*)NULL);
    for (int i = 0; i < n_philosophers; ++i){ 
        pthread_join(philosopher_thread[i], NULL);
    }
    
    return 0;
}