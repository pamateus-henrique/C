#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


void calculatePrimeNumbers(int n){
    int i = 3, count, c;

        if (n >= 1)
        {
            printf("\nFirst %d prime numbers are :", n);
            printf("2 ");
        }

        for (count = 2; count <= n;)
        {
            for (c = 2; c <= i - 1; c++)
            {
                if (i % c == 0)
                    break;
            }
            if (c == i)
            {
                printf("%d ", i);
                count++;
            }
            i++;
        }
        printf("\n");
}

void calculateSequence(int n){
    printf("i'm the son, (PID = %d, PPID: %d)\n", getpid(), getppid());
    int first, second;
    printf("type the first element of the sequence: ");
    scanf("%d", &first);

    printf("type the second element of the sequence: ");
    scanf("%d", &second);
    int prev, prevv, aux;
    prevv = first;
    prev = second;
    printf("the sequence is: ");
    printf("%d ", first);
    printf("%d ", second);
    while(n > 0){
        printf("%d ", prev+prevv);
        aux = prev+prevv;
        prevv = prev;
        prev = aux;
        n--;
    }
    printf("\ni'm the son, (PID = %d, PPID: %d) and I finished\n", getpid(), getppid());
}




int main()
{
    pid_t idProccess;
    pid_t idPPID;
    int counter, state = 0;

    int n;
    printf("type the number of elements to be generated and printed: ");
    scanf("%d", &n);

        idProccess = fork();

    if (idProccess < 0)
    {
        fprintf(stderr, "fork failed");
    }
    else if (idProccess != 0)
    { 
        printf("i'm the father (pid = %d, PPID: %d) I will wait for my son and then generate the prime numbers\n", getpid(), getppid());
        wait(&state);
        printf("i'm the father (pid = %d, PPID: %d) my son has finished, now I will generate the primes\n", getpid(), getppid());
        calculatePrimeNumbers(n);
        return 0;
    }
    else
    { // son
        calculateSequence(n);
        exit(1);
    }

    getchar();

    return 0;
}
