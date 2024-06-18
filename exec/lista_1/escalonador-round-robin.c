// Escalonador Round-Robin
// Disponível em: https://moj.naquadah.com.br/contests/fga_fso_2024_01_t01_trabalho1/fso-escalonador-round-robin.pdf

#include <stdio.h>
#include <stdlib.h>

typedef struct Processo
{
    int pid;
    int p_time;
    int turnaround;
} Processo;

void shift(Processo *v, int index, int N)
{
    Processo temp = v[index];
    for (int i = index; i < N - 1; i++)
    {
        v[i] = v[i + 1];
    }
    v[N - 1] = temp;
}

int main()
{
    int N, quantum;
    scanf("%d %d", &N, &quantum);

    Processo *v = malloc((N + 1) * sizeof(Processo));

    int pid, t;
    for (int i = 0; i < N; i++)
    {
        scanf(" %d %d", &pid, &t);
        v[i] = (Processo){.pid = pid, .p_time = t * 1000, .turnaround = 0};
    }

    int total_time = 0;
    while (N > 0)
    {
        for (int i = 0; i < N; i++)
        {
            if (v[i].p_time > 0)
            {
                if (v[i].p_time > quantum)
                {
                    total_time += quantum;
                    v[i].p_time -= quantum;
                }
                else
                {
                    total_time += v[i].p_time;
                    v[i].turnaround = total_time;
                    v[i].p_time = 0;
                    printf("%d (%d)\n", v[i].pid, v[i].turnaround);
                    shift(v, i--, N--);
                }
            }
        }
    }

    free(v);

    return 0;
}