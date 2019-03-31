#include <iostream>
#include <pthread.h>
#include <semaphore.h>
using namespace std;
const int seats = 5;

sem_t barberReady;
sem_t accessSeats;
sem_t customerReady;
int freeSeats = seats;
void initialization(){
    sem_init(&barberReady, 0, 0);
    sem_init(&accessSeats, 0, 1);
    sem_init(&customerReady, 0, 0);
}

void barberCutsHair(){
}

void customerGetsHairCut(){
}

void *barberRoutine(void *arg){
    while(true){
        sem_wait(&customerReady);
        sem_wait(&accessSeats);     
        barberCutsHair();
        freeSeats++;    
        sem_post(&barberReady);        
        sem_post(&accessSeats);       
        }
}

void *customerRoutine(void *arg){
    while(true){
        sem_wait(&accessSeats);         
        if(freeSeats > 0){
            freeSeats--;               
            sem_post(&customerReady);  
            sem_post(&accessSeats);    
            sem_wait(&barberReady);
            customerGetsHairCut();
        }         
        else{                  
            sem_post(&accessSeats);
           // customer is back
        }
        
    }
}
int main(){
    pthread_t barber;
    pthread_t customer;
    initialization();
    pthread_create(&barber, NULL, barberRoutine, NULL);
    pthread_create(&customer, NULL, customerRoutine, NULL);
    pthread_exit(NULL);
}
