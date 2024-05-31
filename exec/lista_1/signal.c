#include <signal.h>
#include <stdio.h>
#include <unistd.h>


int contador = 0;

void handler(int signal_number) {
    contador++;
}



int main() {

    pid_t pid = getpid();
    
    struct sigaction sa = {};
    sa.sa_handler = &handler;
    sigaction (SIGUSR1, &sa, NULL);
    printf("Pressione E para encerrar o programa:\n");
    //kill(pid, SIGUSR1); 
    while (getchar() != 'E') 
        ;
    printf("SIGUSR1 recebido %d vezes\n", contador);
    return 0;
}
