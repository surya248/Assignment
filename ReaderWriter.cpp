#include<stdio.h>
#include<pthread.h>
#include <time.h>
#include <unistd.h>
#include<semaphore.h>

sem_t mutex,write_sema;
int data = 0,r = 0;

void *reader(void* arg)
{
  int f;
  f = ((int)arg);
  sem_wait(&mutex);
  r = r + 1;
  if(r==1)
   sem_wait(&write_sema);
  sem_post(&mutex);
  printf("Reader %d Reads %d\n",f,data);
  sleep(1);
  sem_wait(&mutex);
  r = r - 1;
  if(r==0)
   sem_post(&write_sema);
  sem_post(&mutex);
}

void *write_func(void* arg)
{
  int f;
  f = ((int) arg);
  sem_wait(&write_sema);
  data++;
  printf("Writer %d write_semas %d\n",f,data);
  sleep(1);
  sem_post(&write_sema);
}

int main()
{
  pthread_t read[10],write[10];
  sem_init(&mutex,0,1);
  sem_init(&write_sema,0,1);
  for(int i=0;i<5;i++)
  {
    pthread_create(&write[i],NULL,write_func,(void*)i);
    pthread_create(&read[i],NULL,reader,(void*)i);
  }
  for(int i=0;i<10;i++)
  {
    pthread_join(write[i],NULL);
    pthread_join(read[i],NULL);
  }
  return 0;
}
