// Timed Mini Shell
// Disponível em: https://moj.naquadah.com.br/contests/fga_fso_2024_01_t01_trabalho1/fso-timedshell.pdf

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

double get_time(__time_t sec_start, __time_t sec_end, __suseconds_t usec_start, __suseconds_t usec_end)
{
    return (sec_end + (double) usec_end/1e6) - (sec_start + (double) usec_start/1e6);
}

int main()
{
    char command[255], arg[255];
    struct timeval total_start, total_end, partial_start, partial_end;
    int return_val;

    gettimeofday(&total_start, NULL);

    while (scanf(" %s %s", command, arg) != EOF)
    {
        gettimeofday(&partial_start, NULL);

        if(fork())
        {
            wait(&return_val);
            gettimeofday(&partial_end, NULL);

            double time = get_time(partial_start.tv_sec, partial_end.tv_sec, partial_start.tv_usec, partial_end.tv_usec);

            printf("> Demorou %.1lf segundos, retornou %d\n", time, return_val/256);
            fflush(stdout);
        }
        else
        {
            if(execl(command, command, arg, NULL) == -1)
            {
                printf("> Erro: %s\n", strerror(errno));
                fflush(stdout);
                exit(errno);
            }
        }

    }

    gettimeofday(&total_end, NULL);
    double time = get_time(total_start.tv_sec, total_end.tv_sec, total_start.tv_usec, total_end.tv_usec);

    printf(">> O tempo total foi de %.1lf segundos\n", time);

    return 0;
}