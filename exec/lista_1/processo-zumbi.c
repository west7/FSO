// Criando um Zumbi
// Disponível em: https://moj.naquadah.com.br/contests/fga_fso_2024_01_t01_trabalho1/processo-zumbi.pdf

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void signal_handler(int signal)
{
}

int main()
{

    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    pause();

    __pid_t pid = fork();

    if (pid == 0)
    {
        exit(0);
    }
    else if (pid > 0)
    {
        pause();
        wait(NULL);
        pause();
        exit(0);
    }
}