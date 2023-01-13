#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int value1;
    int value2;
} t_Args;
void* routine(void *arg) {
    t_Args *args = (t_Args *) arg;

    if(args->value1 == NULL){
        printf("type the first value of the sequence: ");
        scanf("%d", &args->value1);
        return (void*) args;
    }
    if(args->value2 == NULL){
        printf("type the first value of the sequence: ");
        scanf("%d", &args->value2);
        printf("sequência: %d %d ", args->value1, args->value2);
        return (void*) args;
    }
   
   int aux = args->value2;
   args->value2 = args->value1 + args->value2;
   args->value1 = aux;
   printf("%d ", args->value2);
   return (void*) args;

}
int main(int argc, char* argv[]){
    int nThreads = atoi(argv[1]);
    pthread_t tid[nThreads];
     int* res;
    t_Args *arg;
    arg = malloc(sizeof(t_Args) * nThreads);
    
    for(int t = 0; t<nThreads; t++){
         pthread_create(&tid[t], NULL, routine, (void *) arg);
         pthread_join(tid[t], (void**) &res);
         arg = (t_Args *)res;
        
            
        }
    
}