// Distribuição de trabalhos: pthreads e mutex
// Disponível em: https://moj.naquadah.com.br/contests/fga_fso_2024_01_t01_trabalho1/fso-pthreads-mutex.pdf

#include <stdio.h>
#include <pthread.h>

int count;
pthread_mutex_t mut;

struct thread_arg
{
    int vezes;
};

int work(int id);

void *thread_func(void *arg)
{
    struct thread_arg *t_arg = (struct thread_arg *)arg;
    for (int i = 0; i < t_arg->vezes; i++)
    {
        int id;
        pthread_mutex_lock(&mut);
        id = count;
        count++;
        pthread_mutex_unlock(&mut);

        work(id);
    }
    return NULL;
}
