//Item is a random number between 0 and 1000

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <random>

using namespace std;

#define BUFF_SIZE 500

int buff[BUFF_SIZE];
int i=0;

sem_t full,empty;
pthread_mutex_t mutex;

void* produce(void* arg){
	while(1){
		sleep(1);
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		int item = rand()%1000;
		buff[i++] = item;
		cout<<item<<" has produced "<<endl;
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

void* consume(void* arg){
	while(1){
		sleep(1);
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		int item = buff[--i];
		cout<<item<<" has consumed "<<endl;
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}

int main(){
	pthread_t producer,consumer;
	sem_init(&empty,0,BUFF_SIZE);
	sem_init(&full,0,0);
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&producer,NULL,produce,NULL);
	pthread_create(&consumer,NULL,consume,NULL);
	pthread_exit(NULL);
}
