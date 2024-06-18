# 6. Sincronização e comunicação entre processos
Em ambientes [multithread](../notes/05_thread.md#54-ambiente-multithread), onde existem vaŕios processos ou threads executando de forma concorrente, ou até mesmo paralela em sistemas com múltiplos processadores, o compartilhamento de recursos tais como: Arquivos, áreas de memória, variáveis, dispositivos de I/O, entre outros, podem ocasionar problemas na execução de uma aplicação e levar à resultados indesejados. Nesse contexto, se faz necessária uma coordenação entre os processos, de modo que eles posssam atuar de maneira sincronizada. 

## 6.1 A arte de multiprogramar

Converter código [monothread](../notes/05_thread.md#53-ambiente-monothread) em [multithread](../notes/05_thread.md#54-ambiente-multithread) é uma tarefa bastante difícil, especialmente quando se lida com variáveis globais.

<div style="display: flex; justify-content: space-between;">
    <div style="width: 48%;">

        ```c title="não-thread-safe:"
        #include <stdio.h>

        int vetor[256];
        int thread(int val, int pos){
            vetor[pos] = val;
        }
        ```
    </div>
    <div style="width: 48%;">

        ```c title="thread-safe:" 
        #include <stdio.h>

        int thread(int val, int pos){
            int vetor[256]; 
            vetor[pos] = val;
        }
        ```
    </div>
</div>  

Nos sistemas Unix há algumas variáveis que forma definidas globalmente e são muito utilizadas, como: `errno`. Esses problemas podem ocorrer via [syscalls](../notes/02_estrutura_SO.md#23-rotinas-do-sistema-e-syscalls). Por exemplo:


```C
int *p;
p = malloc(sizeof(int) * 10);
```

E se houver troca de contexto no meio da chamada? O malloc salva as informações em uma tabela global.

Porém nem todos os problemas são causados pelas variáveis globais, variáveis compartilhadas entre as threads também exige cuidado, pois a ordem de execução é definida pelo SO, como [visto anteriormente](../notes/05_thread.md#55-programacao-multithreading). Alguns desses problemas são conhecidos como: **Condição de corrida**.

## 6.2 Compartilhamento de recursos (condição de corrida)

Como é o sistema operacional, através do escalonador, que determina a ordem de execução dos processos/threads, nós não sabemos quando um processo irá executar. Trocas de contexto podem ocorrer a qualquer momento. 

Considere os seguintes processos/threads incrementando uma variável em memória compartilhada:

```C
int x = 0;

int thread_A(int x){
    return x++;
}

int thread_B(int x){
    return x++;
}
```
!!! Example "Assumindo que `x` está na posição de memória `0x2000`."

    === "Escalonamento A :octicons-arrow-right-24: B"

        | Processo/Thread A      | Processo\Thread B      |
        | ---------------------- | ---------------------- |
        | LOAD R1, 0x2000 (x=0)  |                        |
        | INC R1                 |                        |
        | STORE R1, 0x2000 (x=1) |                        |
        |                        | LOAD R1, 0x2000 (x=1)  |
        |                        | INC R1                 |
        |                        | STORE R1, 0x2000 (x=2) |

        - **`x = 2`**
  
    === "Escalonamento A :octicons-arrow-right-24: B :octicons-arrow-right-24: A"

        | Processo/Thread A      | Processo\Thread B      |
        | ---------------------- | ---------------------- |
        | LOAD R1, 0x2000 (x=0)  |                        |
        | INC R1                 |                        |
        |                        | LOAD R1, 0x2000 (x=1)  |
        |                        | INC R1                 |
        |                        | STORE R1, 0x2000 (x=2) |
        | STORE R1, 0x2000 (x=1) |                        |

        - **`x = 1`**


Este comportamento é bastante indesejável, visto que para o programador a variável `x` foi incrementada duas vezes, porém ela em algum caso aparece com apenas um incremento. Debugar essas operações pode ser bastante complexo.

## 6.3 Exclusão mútua
As condições de corrida levam a resultados inesperados, por isso é necessário assegurar que os processos que estão trabalhando na mesma região de memória não sejam interrompidos, ou aguardem o término do outro processo para iniciar sua execução. Esse procedimento é denominado **Exclusão mútua**.

A exclusão mútua deve afetar apenas processos concorrentes, e quando estiverem acessando algum recurso compartilhado. O trecho do código que há leitura ou escrita dos dados compartilhados é chamado de **Região crítica**. No exemplo anterior de [condição de corrida](../notes/06_sincronizacao_e_comunicacao.md#62-compartilhamento-de-recursos-condicao-de-corrida), a área crítica é a operação de incremento. Normalmente, a região crítica compreende **mais de uma operação**. O procedimento consiste em não permitir que dois ou mais processos acessem a seção crítica de forma simultânea, se isso for garantido, o problema estará solucionado.

Para isso, são posicionadas funções antes e depois de uma região crítica. Essas funções utilizam diversas técnicas para impedir a entrada de dois processos.

!!! notes "Variáveis de impedimento"
    As variáveis de impedimento buscam marcar se alguém está ou não na região crítica. Observe o código:

    ```C hl_lines="3 4" linenums="1"
    int thread(){
        while (true){
            while (lock == 1);
            lock = 1;
            critical_section();
            lock = 0
        }
    }
    ```
    Aparentemente a variável `lock` impede que outro processo entre na região crítica. Porém se houver uma troca de contexto bem entre as linhas 3 e 4 (depois de sair do loop e antes de trocar o valor de `lock` para 1), haverá uma condição de corrida. Em conclusão, a solução desses problemas não é fácil.

## 6.4 Técnicas de implementação
Existem várias abordagens para implementar exclusão mútua, cada uma com suas próprias vantagens e desvantagens. Aqui estão as principais divisões dessas implementações:

### 6.4.1 Inibir interrupções
Essa técnica consiste no processo, se for possível, desabilitar e habilitar antes e depois de acessar sua seção crítica, respectivamente. Um processo que desabilitou as interrupções não pode ser interrompido pelo escalonador e retirado da CPU. Sendo assim, não há problemas de acesso concorrente.

```c
desabilita();
critical_section();
habilita();
```

Contudo, esta solução é bastante limitada. Primeiro que ela afeta seriamente a multiprogramação, que consiste basicamente na interrupção de processos pelo SO. Segundo que, caso o processo desligasse as interrupções e não voltasse à liga-las, isto causaria **grande impacto** no sistema. Apesar disso, essa é uma função amplamente usada no núcleo do sistema, deixada fora do alcance do programador no espaço do usuário.

### 6.4.2 Espera ocupada
Também chamada de *busy waiting*, a espera ocupada desperdiça o tempo que tem de CPU fazendo um teste trivial. Deve ser usada quando há a expectativa de esperar muito pouco. As vezes pode ser obrigatória em modo kernel.

```C
while (turn != mine);
```
!!! quote "Espera Ocupada ?"
     Mesmo quando não é a vez do processo, ou seja, ele está em espera, ainda assim ele consome CPU fazendo a verificação do loop.

#### 6.4.2.1 Estrita alternância

É um método de sincronização que se baseia no revezamento de **dois** processos. A ideia é que um processo sinalize que está na seção crítica e o outro processo aguarda a saída. Não deve ser usada quando um processo é muito mais lento que o outro. Ela viola a regra de um processo fora da seção crítica bloquear outro processo. O acesso ao recurso só pode ser realizado por dois processos, e sempre de forma alternada. Se houver um problema com um processo, de modo que não altere a variável, o outro ficará bloqueado indefinidamente.


```C title="Solução:"
int turn = 0;
```
<div style="display: flex; justify-content: space-between;">
    <div style="width: 48%">

        ```c title=""
        int thread_A(){
            while (true) {
                while (turn != 1);
                critical_section();
                turn = 0;
                non_critical_section();
            }
        }
        ```
    </div>
    <div style="width: 48%">

        ```c title=""
        int thread_B(){
            while (true) {
                while (turn != 0);
                critical_section();
                turn = 1;
                non_critical_section();
            }
        }
        ```

    </div>
</div>

#### 6.4.2.2 Algoritmo de Peterson

Uma otimização do algoritmo de Dekker. É um algoritmo clássico de exclusão mútua entre dois processos, mas que pode ser generalizada para N processos. Utiliza variáveis de condição, indicando o desejo de um processo de entrar na região crítica e uma variável indicativa de turno. 

Os processos possuem um id único (0 ou 1). O processo deve chamar uma função enter_region, que retorna só quando for seguro entrar na seção. Ao terminar o processamento, a função leave_region deve ser chamada para indicar que outros processos podem prosseguir. Isso garante que um processo fora da seção crítica não bloqueie outros. Garante a exclusão mútua e que um processo nunca ficará bloqueado indeterminadamente graças a variável `turn`.

```C title=""
int interested[2] = {false, false};
int turn = 0;

void enter_region(int process_id) {

    int other = 1 - process;

    interested[process_id] = true;
    turn = other;
    while (interested[other] && turn == other);
}

void leave_region(int process_id) {
    interested[process_id] = false;
}
```

#### 6.4.2.3 Utilizar hardware adicional (TSL)

Utiliza instruções **atômicas** fornecidas pelo hardware, como `test_and_set` ou `compare_and_swap`. Desta forma, podemos fazer um código parecido
com as variáveis de impedimento:
```C
while (test_and_set(v) != 0){ }
```
A instrução TSL equivale logicamente a:
```C
while (lock == 0) {};
lock = 1;
```
> Faz o teste do loop e muda a variável atomicamente (na mesma instrução), não permitindo que uma troca de contexto no meio cause uma condição de corrida.

---

!!! failure "Espera Ocupada: Desvantagens"
    - Deve ser usada apenas em modo protegido ou com tempo de lock baixo.
    - Se isso não for atingido é desejável colocar o processo pra dormir para que não consuma a CPU.
    - Além disso, existe o problema de prioridades invertidas, quando utiliza-se um escalonador de prioridade estática.

### 6.4.3 Bloqueio de processos

O processo aguarda a permissão para a entrada na região crítica e realiza uma syscall para causar seu bloqueio até que a região seja liberada. O bloqueio ocasiona em uma troca de contexto entre os processos e pode gerar uma espera longa. Chamadas blocantes podem não estar disponíveis em modo kernel.int turn = 0;


```C
if (turn != mine) 
    wait_my_turn(turn);
```

<a id="prod-cons"></a>

??? abstract "Problema Produtor-Consumidor"
    Neste problema um thread escreve em um *buffer* outra lê este mesmo *buffer*, concorrentemente. O problema é que a **thread que grava os dados (produtor)**, não deve escrever em um buffer cheio, e **thread que lê (consumidor)** não deve ler um buffer vazio. Assim, o produtor deve *dormir* caso o buffer esteja cheio, e o consumidor caso esteja vazio. Um deve acordar o outro.

    ```C title="Pseudo-código em C"
    #define N 100 //Tamanho do buffer
    int count = 0 

    void producer()
    {
        int item;

        while (true)
        {
            item = produce_item();

            if (count == N)
                sleep(producer);
            insert_item(item);
            count++;
            if (count == 1)
                wakeup(consumer)
        }
    }

    void consumer()
    {
        int item;

        while (true)
        {
            if (count == 0)
                sleep(consumer);
            item = remove_item();
            count--;
            if (count == N - 1)
                wakeup(producer);

            consume_item(item);
        }
    }
    ```

#### 6.4.3.1 Semáforo

São variáveis de controle que servem como contadores de quantos sinais foram recebidos. Se baseiam, bem como o TSL, em operações atômicas.

- Operação `down(sem)`:
    - Decrementa o valor do semáforo. Se for maior que zero continua, ou bloqueia se o valor chegar a zero.

- Operação `up(sem)`:
    - Incrementa o valor do semáforo. Se algum processo estiver dormindo nele, algum é escolhido para tratar.
    - Nenhum processo é bloqueado ao dar `up(sem)`. 

```C title="Exemplo"
#define semaphore int
semaphore s = 0;

int thread1(){
    printf("T1 - start\n");
    up(s);
    printf("T1 - end\n");
}

int thread2(){
    down(s);
    printf("T2\n")
}
```

- A mensagem "`T1 - start\n`" sempre será impressa primeiro, as outras duas "`T1 - end\n`" e "`T2\n`" dependem da ordem de escalonamento.

```C title="Semáforo Produtor-Consumidor:"
#define N 100
#define sempahore int
semaphore mutex = 1;
semaphore empty = N;
semaphore full = 0;
```
<div style="display: flex; justify-content: space-between;">
    <div style="width: 48%">

    ```C
    void producer()
    {
        int item;
        while (true)
        {
            item = produce_item();
            down(&empty);
            down(&mutex)
            insert_item(item);
            up(&mutex);
            up(&full)
        }
    }
    ```
    </div>
    <div style="width: 48%">

    ```C
    void consumer()
    {
        int item;
        while (true)
        {
            down(&full);
            down(&mutex)
            item = remove_item();
            up(&mutex);
            up(&empty)
            consume_item(item);
        }
    }
    ```
    </div>
</div>

Neste exemplo foram combinados diferentes usos do semáforo. O semáforo `mutex` foi usado para garantir que apenas um entrasse na região crítica por vez, e os dois: `empty` e `full` para sincronizar o trabalho.

#### 6.4.3.2 Mutex
Quando não há a necessidade de usar a capacidade de contagem dos semáforos, pode se usar uma versão simplificada, chamada de mutex. São variáveis binárias podendo estar em estado `travado` e `destravado`. São uma solução simples e de fácil implementação.

```C title="Exemplo"
    void thread()
    {
       mutex_lock(&mutex);
       critical_region();
       mutex_unlock(&mutex);
    }
```

!!! success "Mutex são uma das mais eficientes e rápidas estruturas de comunicação entre threads."

#### 6.4.3.3 Locks
Mutex podem ser chamados de locks exclusivos, ou seja, apenas um processo pode estar na região crítica por vez. Porém, muitas vezes as threads irão compartilhar dados entre si, mas não irão escrever, apenas ler. Nessse caso, não há problema que mais de uma thread esteja na região crítica ao mesmo tempo. Assim, os locks definem duas funções:

- `read_lock`: Indicando que a thread irá apenas ler os dados da seção.
- `write_lock`: Indica que a thread irá escrever na região e, por isso, pode ser a única ali naquele momento.


#### 6.4.3.4 Monitores
Por mais que os exemplos anteriores sejam simples e eficientes, erros ainda podem surgir em um projeto multithread. Como exemplo, no código do [produtor-consumidor](../notes/06_sincronizacao_e_comunicacao.md#prod-cons), se as linhas do mutex forem invertidas, o caos estará implantado. Semáforos e mutex fornecem explicitamente as ferramentas de sincronização para o programador. Já os monitores são mecanismos de sincronização de alto nível que tornam mais simples o desenvolvimento de programas concorrentes. Monitores são um tipo especial de módulo/pacote que contêm uma coleção de rotinas, variáveis e estruturas de dados. Eles permitem a implementação da multiprogramação de forma mais simples, fácil e segura. Hoje, a maioria das linguagens de programação fornecem rotinas para o uso de monitores.

Os processos podem acessar as rotinas de um monitor, mas não seus dados. Apenas um processo pode estar ativo em um monitor por vez. Desta maneira, o programador apenas sinaliza as regiões críticas e ==o compilador se encarrega de implementar o paralelismo e exclussão mútua==.

```java title="Em java:"
class Monitor {
    synchronized void criticalSection() {
        // seção crítica
    }
}
```

!!! notes "Variáveis de condição"
    Nessa técnica se faz necessária o uso de variáveis de condição. Essas variáveis são manipuladas por funções especias de sincronização, são elas `wait` e `signal`. A instrução `wait` coloca o processo em um estado de espera até que algum outro processo sinalize com `signal` que a condição de espera foi atendida e ele pode prosseguir. No exemplo do [produtor-consumidor](../notes/06_sincronizacao_e_comunicacao.md#prod-cons):

    ```C hl_lines="4 6"
        void producer(){
            int item;

            item = produce_item();
            if(n == N)
                wait(&condition);
            insert_item();
        }
    ```
    > As operações marcadas são atômicas! Estão dentro de um monitor.

    Em C, que não existe monitores, o pacote Pthreads definiu que a variável de condição exige o uso de um mutex em conjunto para garantir que essa condição é satisfeita.

    Variáveis de condição não são contadores, caso alguém dê um `signal` antes de um `wait`, o sinal será perdido.


!!! notes "Produtor-consumidor com Monitores"
    
    === "Monitor"

        ```C title=""
            #define N 100
            int full, empty;
            int count = 0;

            void insert_item(item)
            {
                if (count == N) wait(&full);
                insert_item(item);
                count++;
                if (count == 1) signal(&empty);
            }

            int remove_item()
            {
                if (count == 0) wait(&empty);
                item = remove_item();
                count--;
                if (count == N - 1) signal(&full);
                return item;
            }
        ```

    === "Programa"

        ```C 
            void producer()
            {
                while (true)
                {
                    item = produce_item();
                    monitor.insert_item(item);
                }
            }

            void consumer()
            {
                while (true)
                {
                    item = monitor.remove_item();
                    consume_item(item);
                }
            }
        ```

## 6.5 Troca de mensagens
Este é um mecanismo de comunicação e sincronização de threads. Deve ser realizada por um canal de comunicação, que pode ser um *buffer* ou uma *rede*. Essa comunicação ocorre por meio de duas rotinas `send(msg)` ou `recv(msg)`. Nesses projetos temos algumas questões:

- O processo deve bloquear até o outro receber a mensagem ou pode executar?
- Um processo pode enviar uma mensagem apenas para um processo ou pode enviar para vários?

Essa troca é muito utilizada em programação paralela e é muito simples de se implementar:

<div style="display: flex; justify-content:space-between;">
        <div style="width: 48%;">
        ```C 
        void producer()
        {
            while (true)
            {
                int item = produce_item();
                send(item);
            }
        }
        ```
        </div>
        <div style="width: 48%;">
            ```C 
            void consumer()
            {
                while (true)
                {
                    recv(item);
                    consume_item(item);
                }
            }
            ```
        </div>
    </div>

## 6.6 Barreiras
Barreiras são outro tipo de mecanismo de sincronização. Dependendo da situação pode ser interessante que um processo siga para a próxima fase apenas quando todas as outras terminarem a primeira fase. Isso é possível com o uso de barreiras ao final de cada fase.

```C title="Exemplo de barreira:"
condition cond;
mutex lock;

void sync_threads()
{
    mutex_lock(&lock);
    if (++sync_count < N_THREADS)
        cond.wait(lock);
    else
    {
        sync_count = 0;
        cond.notify_all();
    }
    mutex_unlock(&lock);
}

```