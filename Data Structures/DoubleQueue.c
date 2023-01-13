#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct queue_node qnode;
typedef struct doublylinkedlistqueue queue;

struct queue_node
{
    int age;
    char name[30];
    qnode *next;
    qnode *prev;
};
struct doublylinkedlistqueue
{
    qnode *head;
    qnode *tail;
};

queue *create_queue()
{
    queue *q = (queue *)malloc(sizeof(queue));
    q->head = q->tail = NULL;
    return q;
}

/* in this case we will consider prioritary people above 60 years */

void print_queue(queue *self)
{
    qnode *aux = (qnode *)malloc(sizeof(qnode));
    aux = self->head;
    while (aux != NULL)
    {
        printf("name: %s\n", aux->name);
        printf("age: %d\n", aux->age);
        printf("=======================\n");
        aux = aux->next;
    }
}

void print_queue_recursive(qnode *self)
{
    if (self->next != NULL)
    {
        print_queue_recursive(self->next);
    }
    printf("name: %s\n", self->name);
    printf("age: %d\n", self->age);
    printf("=======================\n");
}

void print_queue_reverse(queue *self)
{
    qnode *aux = (qnode *)malloc(sizeof(qnode));
    aux = self->tail;
    while (aux != NULL)
    {
        printf("name: %s\n", aux->name);
        printf("age: %d\n", aux->age);
        printf("=======================\n");
        aux = aux->prev;
    }
}

/* puts this person ahead of all non-prioritary persons */

void insert_queue_priority(queue *self, qnode *q)
{
    qnode *aux = (qnode *)malloc(sizeof(qnode));
    aux = self->head;

    if (self->head == self->tail && aux->age < 60)
    {
        q->next = aux;
        aux->prev = q;
        q->prev = NULL;
        self->head = q;
        return;
    }
    if (self->head == self->tail && aux->age > 60)
    {
        aux->next = q;
        q->prev = aux;
        q->next = NULL;
        self->tail = q;
        return;
    }
    while (aux->next != NULL && aux->age > 60)
    {
        aux = aux->next;
    }
    if (aux->prev == NULL)
    {
        q->prev = aux->prev;
        q->next = aux;
        aux->prev = q;
        self->head = q;
        return;
    }
    aux->prev->next = q;
    q->prev = aux->prev;
    q->next = aux;
    aux->prev = q;
}

void insert_queue(queue *self, int age, char *name)
{
    qnode *q = (qnode *)malloc(sizeof(qnode));
    q->age = age;
    strcpy(q->name, name);
    q->next = NULL;
    q->prev = self->tail;
    if (age > 60 && self->head != NULL)
    {
        insert_queue_priority(self, q);
        return;
    }
    if (self->head == NULL)
    {
        self->head = q;
    }
    else
    {
        self->tail->next = q;
    }

    self->tail = q;
}

int main()
{
    queue *fila = create_queue();
    insert_queue(fila, 79, "Sidoka");
    insert_queue(fila, 21, "Mateus");
    insert_queue(fila, 31, "João");
    insert_queue(fila, 71, "Gerusa");
    insert_queue(fila, 23, "Abel");
    //prints for test porpouse only
    print_queue_recursive(fila->head); //prints the queue
    //printf("idade primeira pessoa: %d\n", fila->head->age); //print the age of the first person in the queue
    //printf("idade última pessoa: %d\n", fila->tail->age);   //print the age of the last person in the queue
}
