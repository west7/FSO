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

int main()
{
    char command[255], arg[255];
    struct timeval total_start, total_end, partial_start, partial_end;
    int return_val;

    return 0;
}