#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// globals
int quantum;
int currentTime;
int finished = 0; 
sem_t semaphore;


typedef struct queue_t queue;
typedef struct proccess_t proccess;
typedef struct thread_args t_args;

struct proccess_t
{
  int id;
  int root;
  int arrivalTime;
  int bursTime;
  proccess *prev; // WTF I'M I DOING????
  proccess *next; // ARE YOU SURE YOU WANNA DO THIS???
};

struct queue_t
{
  proccess *head;
  proccess *tail;
};

struct thread_args{
  queue *first;
  queue *second;
};

queue *create_queue()
{
  queue *q = (queue *)malloc(sizeof(queue));
  q->head = q->tail = NULL;
  return q;
}

int isEmpty(queue *self){
  if(self->head == NULL){
    return 1;
  } else {
    return 0;
  }
}


void print_queue(queue *self)
{
    proccess *aux = (proccess *)malloc(sizeof(proccess));
    aux = self->head;
    while (aux != NULL)
    {
        printf("id: %d\n", aux->id);
        printf("arrivalTime: %d\n", aux->arrivalTime);
        printf("burstTime: %d\n", aux->bursTime);
        printf("=======================\n");
        aux = aux->next;
    }
}

void insert_queue(queue *self, int id, int arrivalTime, int burstTime)
{
  proccess *p = (proccess *)malloc(sizeof(proccess));
  p->id = id;
  p->arrivalTime = arrivalTime;
  p->bursTime = burstTime;
  p->next = NULL;
  p->prev = NULL;
  //empty queue
  if(self->head == NULL){
    self->head = p;
    self->tail = p;
  } else {
    self->tail->next = p;
    self->tail->next->prev = self->tail;
  }

  self->tail = p;
}

proccess *remove_queue(queue *self){
  proccess *p = self->head;
  self->head = self->head->next;
  p->next = NULL;
  p->prev = NULL;
  return p;
} 


//QUE BAGUNÇA, VAI TOMAR NO CU
void *cpu(void* arg){
  t_args *args = (t_args *) arg;

  while(!isEmpty(args->first) || !isEmpty(args->second) || !finished){
    printf("KERNEL EMPTY: %d\n", isEmpty(args->second));
    printf("USER EMPTY: %d\n", isEmpty(args->first));
    if(!isEmpty(args->second)){
       printf("fila de KERNEL\n");
       printf("ID: %d\n", args->second->head->id);
       printf("BurstTime: %d\n", args->second->head->bursTime);
        proccess *p = remove_queue(args->second);
        p->bursTime = p->bursTime - quantum;
        if(p->bursTime > 0) {
          insert_queue(args->second, p->id, p->arrivalTime, p->bursTime);
        } else {
          printf("id: %d, REMOVIDO DA FILA\n", p->id);
        }
        currentTime = currentTime + quantum;
        sleep(1);
        continue;
    } else if(!isEmpty(args->first)){
      printf("fila de USER\n");
      proccess *p = remove_queue(args->first);
      p->bursTime = p->bursTime - quantum;
      if(p->bursTime > 0){
        insert_queue(args->first, p->id, p->arrivalTime, p->bursTime);
      } else {
        printf("id: %d, REMOVIDO DA FILA\n", p->id);
      }
      currentTime = currentTime + quantum;
      continue;
      sleep(1);
    }
    currentTime++;
    printf("currentTime: %d\n", currentTime);
    sleep(1);
  }
}

int main()
{
  sem_init(&semaphore, 0, 1);
  int proccesses = -1;
  int i = -1;
  queue *user_queue = create_queue();
  queue *root_queue = create_queue();
  t_args *args = (t_args *)malloc(sizeof(t_args));
  args->first = user_queue;
  args->second = root_queue;
  pthread_t cputid;
  pthread_create(&cputid, NULL, cpu, (void *)args);
  char *filename = "proccess.txt";
  FILE *fp = fopen(filename, "r");

  if (fp == NULL)
  {
    printf("Error: could not open file %s", filename);
    return 1;
  }

  // reading line by line, max 256 bytes
  const unsigned MAX_LENGTH = 256;
  char buffer[MAX_LENGTH];


  while (fgets(buffer, MAX_LENGTH, fp))
  {
    if (i < 0)
    {
      sscanf(buffer, "%d", &quantum);
      i++;
      continue;
    }
    int id, root, arrivalTime, burstTime;

    sscanf(buffer, "%d %d %d %d", &id, &root, &arrivalTime, &burstTime);

    while(arrivalTime > currentTime){
    }

    

    if(root == 1){
      printf("adicionado KERNEL, currentTime: %d\n", currentTime);
      insert_queue(root_queue, id, arrivalTime, burstTime);
    } else {
      printf("adicionado USER, currentTime: %d\n", currentTime);
      insert_queue(user_queue, id, arrivalTime, burstTime);
    }

    i++;
  }

  finished = 1;

  fclose(fp);
  pthread_join(cputid, NULL);
  print_queue(root_queue);
  print_queue(user_queue);
  sem_destroy(&semaphore);
  return 0;
}