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
#include<vector>
using namespace std;
int n_customers;

class monitor{
    public:
    int number_of_customer,n_chairs;
    sem_t barber,customer,mutex;
    void get_customer(){
        printf("Barber is waiting for a customer\n");
        sem_wait(&customer);
        sem_post(&barber);
        //Cutting the hair
        printf("Barber is cutting hair\n");
        sleep(2);
    }
    void get_haircut(int i){
        printf("Customer %d came for a haircut\n",i);
        sem_wait(&mutex);
        if (number_of_customer == n_chairs+1){
            sem_post(&mutex);
            printf("Customer %d left without getting a haircut\n",i);
            return;
        }
        ++number_of_customer;
        sem_post(&mutex);

        sem_post(&customer);
        sem_wait(&barber);
        printf("Customer %d is getting a haircut done\n",i);
        sleep(2);
        sem_wait(&mutex);
        --number_of_customer;
        sem_post(&mutex);
    }
}barberShopMonitor;
 




void* barber_thread(void* _){
    while (1){
        barberShopMonitor.get_customer();   
    }
    return NULL;
}

void * customer_thread(void *cno){
    int no=*(int*)cno;
    barberShopMonitor.get_haircut(no);
    return NULL;
}

int main(){
    cout<<"Enter number of customers: ";
    cin>>n_customers;
    cout<<"Enter number of chairs: ";
    cin>>barberShopMonitor.n_chairs;
    vector<int>cno(n_customers);
    for(int i=0;i<n_customers;++i){
        cno[i]=i;
    }
    pthread_t barber_thread_id;
    pthread_t customer_thread_id[n_customers];
    sem_init(&barberShopMonitor.mutex,0,1);
    sem_init(&barberShopMonitor.customer,0,0);
    sem_init(&barberShopMonitor.barber,0,0);
    sleep(1);
    pthread_create(&barber_thread_id, NULL, barber_thread, (void*)NULL); 
    for(int i=0;i<n_customers;++i){
        sleep(1);
        pthread_create(&customer_thread_id[i], NULL, customer_thread, (void*)&cno[i]); 
    }
    pthread_join(barber_thread_id, NULL);
    for (int i = 0; i < n_customers; ++i){ 
        pthread_join(customer_thread_id[i], NULL);
    }
    return 0;
}
