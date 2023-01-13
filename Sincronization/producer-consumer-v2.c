/// implementation trying to use more than one semaphore
//overcommenting everything cause I'm lost af right now, ps: remember to erase those un-necessary comments before git commit

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

// globals
int push = 0, pop = 0;
int primeIndex = 0;
int multinputs = 0;
int consumed = 0;
//buffers
int primebuffer[8];
int fibbonaccibuffer[5];
//semaphores
sem_t empty;
sem_t full;
sem_t lock;


int isPrime(int n){
     int num = n;
     int i, prime = 1;

    if(num == 1 || num == 0){
        return 0;
    }


    for (i = 2; i < num; i++) {
    if (num % i == 0) {
      prime = 0;
      break;
    }
  }

  if (prime) {
    return 1;
  } else {
    return 0;
  }

    return 0;

}


void *consumer(){
    int consumido;
    int prime;
    // int prime = 0;
    while(consumed != 25){
        //locking buffer
        sem_wait(&full);
        sem_wait(&lock);
        consumido = fibbonaccibuffer[pop];
        pop = (pop + 1) % 5;
        printf("Thread X consumiu: %d\n", consumido);
        consumed++;
        prime = isPrime(consumido);
        if(prime){
            primebuffer[primeIndex] = consumido;
            primeIndex++;
            printf("prime buffer: ");
            for(int z = 0; z < 8; z++){
                printf("%d ", primebuffer[z]);
            }
            printf("\n");
        }
        sem_post(&empty);
        //unlocking buffer
        sem_post(&lock);
        sleep(1);
    }
    return NULL;
}


void *producer(){
    int prev = 0;
    int next = 1; 
    int produced = 0;

    while(produced < 25){

        for(int j = 0; j < multinputs; j++){
            sem_wait(&empty);
        }
        //locking the buffer
        sem_wait(&lock);

        for(int i = 0; i < multinputs; i++){

            //first two elements
            if(produced == 0){
                fibbonaccibuffer[push] = 0;
                produced++;
                push = (push + 1) % 5;
                sem_post(&full);
                continue;
            } else if (produced == 1){
                fibbonaccibuffer[push] = 1;
                produced++;
                push = (push + 1) % 5;
                sem_post(&full);
                continue;
            }

            int new = prev + next;
            fibbonaccibuffer[push] = new;
            prev = next;
            next = new;
            produced++;
            push = (push + 1) % 5;
            sem_post(&full);
        }
        //unlocking buffer
        sem_post(&lock);
        printf("fibbonacci buffer: ");
        for(int j = 0; j < 5; j++){
            printf("%d ", fibbonaccibuffer[j]);
        }
        printf("\n");

    }

    return NULL;
}   



int main(int argc, char *argv[]){
    //receiving args
     int nThreads = atoi(argv[1]);
     multinputs = atoi(argv[2]);
     //creating threads id
     pthread_t thr_producer;
     pthread_t thr_consumer[nThreads];

     sem_init(&empty, 0, 5); //since the buffer has a fixed size, we can start this semaphore using 5
     sem_init(&full, 0, 0); //starting in 0 because currently the buffer is empty
     sem_init(&lock, 0 , 1); //semaphore that we will use to avoid two threads entering the critical section at the same time

     //creating threads           pthread_create(thread-id, NULL, pointer to the function, args);
      pthread_create(&thr_producer, NULL, producer, NULL); 
        for(int x = 0; x < nThreads; x++){
            pthread_create(&thr_consumer[x], NULL, consumer, NULL);
      }
      //joining threads
      pthread_join(thr_producer, NULL);  
      for(int x = 0; x < nThreads; x++){
              pthread_join(thr_consumer[x], NULL);
          }

      //ðestroing semaphores, freeing memory? research later. IN THE EXAMPLE THE LOCK SEM IS NOT DESTROYED!!!!!!
      sem_destroy(&empty);
      sem_destroy(&full);
      sem_destroy(&lock);

      for(int k = 0; k < 8; k++){
        printf("%d ", primebuffer[k]);
      }

      return 0;

}