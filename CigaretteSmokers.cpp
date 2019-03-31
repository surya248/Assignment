#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <random>
#include <unistd.h>

using namespace std;

// Semaphore Declaration
sem_t tobacco_and_paper;
sem_t matches_and_paper;
sem_t tobacco_and_matches;
sem_t done_smoking;
sem_t lock;

void *agent(void *arg);
void *first(void *arg);
void *second(void *arg);
void *third(void *arg);

int main(){
    // thread declaration
    pthread_t smoker_thread[3];
    pthread_t agent_thread;
    //Semaphore initialization
    sem_init(&done_smoking, 0, 1);
    sem_init(&lock, 0, 1);
    sem_init(&tobacco_and_matches, 0, 0);
    sem_init(&matches_and_paper, 0, 0);
    sem_init(&tobacco_and_paper, 0, 0);

    // Thread creation
    pthread_create(&agent_thread, NULL, agent, NULL);
    pthread_create(&smoker_thread[0], NULL, first, NULL);
    pthread_create(&smoker_thread[1], NULL, second, NULL);
    pthread_create(&smoker_thread[0], NULL, third, NULL);

    pthread_exit(NULL);
}

void *agent(void *arg){
    while(true){
        sem_wait(&lock);
        int rand = rand()%3 + 1;
        switch (rand)
        {
            case 1:
                sem_post(&tobacco_and_matches);
                break;
            case 2:
                sem_post(&tobacco_and_paper);
                break;
            case 3:
                sem_post(&matches_and_paper);
                break;
            default:
                break;
        }
        sem_post(&lock);
        sem_wait(&done_smoking);
    }
}

void *first(void *arg){
    while(true){
        sem_wait(&tobacco_and_matches);
        sem_wait(&lock);
        cout << "First picks up matches." << endl;
        cout << "First smokes" << endl;
        sem_post(&done_smoking);
        sem_post(&lock);
    }
}

void *second(void *arg){
    while(true){
        sem_wait(&tobacco_and_paper);
        sem_wait(&lock);
        cout << "Second picks up matches." << endl;
        cout << "Second smokes" << endl;
        sem_post(&done_smoking);
        sem_post(&lock);
    }
}

void *third(void *arg){
        while(true){
        sem_wait(&matches_and_paper);
        sem_wait(&lock);
        cout << "Third picks up matches." << endl;
        cout << "Third smokes." << endl;
        sem_post(&done_smoking);
        sem_post(&lock);
    }
}
