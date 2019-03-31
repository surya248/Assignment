#include <iostream>
#include <pthread.h> 
#include <semaphore.h>
#include <random>
#include <unistd.h>

#define PHILOSOPHERS 5
#define LEFT (philosopher_num + 4) % PHILOSOPHERS
#define RIGHT (philosopher_num + 1) % PHILOSOPHERS

using namespace std;

const int THINKING = 2;
const int HUNGRY = 1;
const int EATING = 0;
  
int state[PHILOSOPHERS];

sem_t lock;
sem_t S[PHILOSOPHERS]; 

void check(int philosopher_num);
void take_chopstick(int philosopher_num);
void put_chopstick(int philosopher_num);
void *philospher(void *arg);

int main(){
    pthread_t philospher_thread[PHILOSOPHERS]; 
    // initialize the semaphores 
    sem_init(&lock, 0, 1);

    for(int i = 0; i < PHILOSOPHERS; i++) 
        sem_init(&S[i], 0, 0);
    // create philosopher processes
    for (int i = 0; i < PHILOSOPHERS; i++) {
        pthread_create(&philospher_thread[i], NULL, philospher, (void *)(intptr_t)i);   
        cout << "Philosopher " << i+1 << "is thinking" << endl; 
    } 
    pthread_exit(NULL);
}

void check(int philosopher_num){ 
    if (state[philosopher_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){ 
        state[philosopher_num] = EATING; 
        sleep(rand()%4); 
        cout << "Philosopher " << philosopher_num+1 << " takes fork " << LEFT+1 << " and " << philosopher_num+1 << endl;
        cout << "Philosopher " << philosopher_num+1 << " is Eating" << endl;
        sem_post(&S[philosopher_num]); 
    } 
} 

void take_chopstick(int philosopher_num){ 
    sem_wait(&lock); 
    state[philosopher_num] = HUNGRY;
    cout << "Philosopher " << philosopher_num+1 << " is Hungry" << endl;
    check(philosopher_num); 
    sem_post(&lock); 
    sem_wait(&S[philosopher_num]);   
    sleep(rand()%3); 
} 

void put_chopstick(int philosopher_num){ 
    sem_wait(&lock); 
    state[philosopher_num] = THINKING; 
    cout << "Philosopher " << philosopher_num+1 << " putting fork " << LEFT+1 << " and " << philosopher_num+1 <<  " down" << endl; 
    cout << "Philosopher " << philosopher_num+1 << "is thinking" << endl; 
    check(LEFT);
    check(RIGHT); 
    sem_post(&lock); 
}

void* philospher(void* arg){
    while (true) { 
        int philosopher_num = ((intptr_t)arg); 
        sleep(rand()%2); 
        take_chopstick(philosopher_num);
        put_chopstick(philosopher_num);
    }
}
