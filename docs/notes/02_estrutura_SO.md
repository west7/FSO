# 2. Estrutura de um Sistema Operacional
O SO é formado por um conjunto de rotinas dedicado ao usuário e as aplicações. Esse conjunto pode ser denominado *kernel* (núcleo). Além disso o sistema também apresenta um conjunto de utilitários e linguagem de comando, porém esses não fazem parte da estrutura do SO e são apenas um apoio para o usuário.

## 2.1 Funções do Kernel
As rotinas de um SO, diferentemente de uma aplicação comum, são concorrentes com base em eventos dissociados do tempo (assíncronos). As principais funções do núcleo são:

- Tratamento de interrupções;
- Criação, eliminação, sincronização e comunicação de [processos e threads](../notes/03_processos_e_threads.md);
- [Escalonamento](../notes/04_gerencia_do_processador.md#41-escalonador-e-criterios-de-escalonamento);
- Gerência de memória;
- Gerência do sistema de arquivos;
- Gerência de dispositivos de I/O;
- Suporte a redes;
- Contabilização do uso do sistema; 
- Auditoria e segurança do sistena.

## 2.2 Modos de acesso
Uma preocupação que surge no projeto de SOs é a proteção e o nível de acesso ao núcleo, pois caso alguma aplicação altere a integridade do kernel todo o sistema pode ser comprometido. Muitas dessas proteções utilizam uma função dos processadores conhecido como **modo de acesso**. 

Em geral, existem 2 modos de acesso: ***Modo Usuário e Modo Kernel (protegido)***. O primeiro permite acesso somente a instruções **não privilegiadas**. Já o Modo Kernel possui acesso a **todas** as instruções do processador. Um SO roda em **Modo Kernel**.

## 2.3 Rotinas do Sistema e Syscalls
O controle de execução das rotinas do SO é realizado por um mecanismo chamado **System calls (Syscalls)**. Sempre que uma aplicação desejar chamar uma rotina ele realiza uma Syscall, e então, se a aplicação possui a permição necessária, o SO salva o conteúdo dos registradores, troca o modo para protegido, altera o PC para o endereço da rotina chamada. No final da execução o SO altera o modo para usuário e restaura o contexto dos registradores para que a aplicação continue de onde parou.

## 2.4 Linguagem de comando
A linguagem de comandos é uma forma pela qual o usuário pode se comunicar diretamente com o SO, o que possibilita executar tarefas específicas do sistema. Os comandos digitados são interpretados pelo *shell* que interpreta o comando e realiza as syscalls, retornando algum valor para o usuário. Posteriormente as linguagens de comando evoluíram para interfaces gráficas, possibilitando uma interação mais simples e intuitiva.

Aqui está uma lista de alguns comandos Linux mais utilizados:

| Comando           | Descrição                                                                   |
| ----------------- | --------------------------------------------------------------------------- |
| `ls`              | Lista os arquivos e diretórios no diretório atual.                          |
| `cd`              | Altera o diretório atual.                                                   |
| `pwd`             | Exibe o diretório atual.                                                    |
| `mkdir`           | Cria um novo diretório.                                                     |
| `rm`              | Remove arquivos ou diretórios.                                              |
| `cp`              | Copia arquivos e diretórios.                                                |
| `mv`              | Move ou renomeia arquivos e diretórios.                                     |
| `touch`           | Cria um novo arquivo vazio.                                                 |
| `cat`             | Exibe o conteúdo de um arquivo.                                             |
| `grep`            | Pesquisa por padrões em arquivos.                                           |
| `head`            | Exibe as primeiras linhas de um arquivo.                                    |
| `tail`            | Exibe as últimas linhas de um arquivo.                                      |
| `less`            | Visualiza o conteúdo de um arquivo página por página.                       |
| `chmod`           | Altera as permissões de acesso de arquivos e diretórios.                    |
| `chown`           | Altera o proprietário de arquivos e diretórios.                             |
| `ps`              | Lista os processos em execução.                                             |
| `top`             | Exibe uma lista atualizada em tempo real dos processos e suas estatísticas. |
| `kill`            | Envia um sinal a um processo, geralmente usado para encerrar processos.     |
| `tar`             | Cria ou extrai arquivos compactados no formato tar.                         |
| `gzip` / `gunzip` | Comprime ou descomprime arquivos no formato gzip.                           |
| `find`            | Busca arquivos em um diretório específico.                                  |
| `grep`            | Pesquisa por padrões em arquivos.                                           |
| `awk`             | Processa e extrai dados de arquivos de texto.                               |
| `sed`             | Edita texto usando expressões regulares.                                    |
| `wget`            | Baixa arquivos da internet.                                                 |
| `curl`            | Transfere dados de ou para um servidor.                                     |
| `ssh`             | Conecta-se a um servidor remoto de forma segura.                            |
| `scp`             | Copia arquivos entre computadores via SSH.                                  |
| `rsync`           | Sincroniza e copia arquivos e diretórios entre sistemas de forma eficiente. |
| `df`              | Exibe informações sobre o espaço em disco.                                  |
| `du`              | Exibe o uso do espaço em disco por diretório.                               |
| `free`            | Exibe informações sobre o uso da memória.                                   |
| `uname`           | Exibe informações sobre o sistema operacional.                              |
| `date`            | Exibe ou define a data e hora do sistema.                                   |
| `whoami`          | Exibe o nome do usuário atual.                                              |
| `history`         | Exibe uma lista dos comandos digitados anteriormente.                       |
| `man`             | Exibe o manual de um comando específico.                                    |

Esses são apenas alguns dos comandos mais comuns no Linux. Há muitos outros comandos e opções disponíveis, e você pode explorar mais detalhes sobre cada comando utilizando a opção `man` seguida do nome do comando (por exemplo, `man ls` para obter o manual do comando `ls`).

## 2.5 Arquitetura do Núcleo 
Em sistemas operacionais, um dos assuntos mais importantes é sobre as diferentes estruturas do *kernel*. Nos sistemas operacionais modernos, o número de linhas de código pode exceder 40 milhões, e grande parte do código é escrita em C/C++. Linguagens de alto nível, facilitam no desenvolvimento, manutenção e portabilidade do código. No entanto, essas linguagens perdem em muito para linguagens de máquina em relação ao desempenho, por isso partes vitais do código como o [escalonador](../notes/04_gerencia_do_processador.md) e o **gestor de interrupções** são desenvolvidos em linguagem *assembly*. A seguir veremos as principais arquiteturas do *kernel*.

!!! Tip "Resumo das arquiteturas:"

    === "Monolítica"
          - Núcleo único
          - Eficiente
          - Complexo
          - Baixa escalabilidade

    === "Camadas"
          - Hierarquia entre níveis
          - Abstração e redução da complexidade
          - Redução no desempenho 

    === "Máquina virtual"
          - Isolamento
          - Segurança
          - Portabilidade e escalabilidade
          - Consolidação em servidores (rodar várias aplicações em uma máquina física)

    === "Micro-Kernel"
          - Funcionalidades mínimas
          - Proteção do núcleo
          - Escalabilidade alta
          - Difícil implementação


    === "Exo-Kernel"
          - Abstração
          - Isolamento de recursos
          - Segurança aprimorada
          - Alta Eficiência das aplicações
          - Flexibilidade

### 2.5.1 Arquitetura Monolítica
É modelo de arquitetura mais comum e amplamente utilizado. Pode ser comparada à uma aplicação com diferentes módulos que são compilados individualmente e depois ligados, formando um grande bloco executável. É descrito por **Tanenbaum[^1]** como uma **"grande bagunça"**. Mesmo assim, possui a melhor organização para o tempo de resposta. A arquitetura de núcleo monolítico agrega todas as funções do Sistema Operacional no núcleo do sistema.

[^1]: Tanenbaum, A. S. Sistemas Operacionais Modernos. 4. ed. São Paulo: Pearson, 2015.


### 2.5.2 Arquitetura de Camadas
O sistema é dividido em níveis sobrepostos, nas quais cada camada pode utilizar os recursos apenas das camadas superiores. As vantagens desse modelo são o isolamento das funções do SO, o que facilita a manutenção, a criação de hierarquia entre os níveis, protegendo os níveis inferiores.

### 2.5.3 Máquina Virtual
Este modelo cria um **nível intermediário** entre o *hardware* e o *sistema operacional*, onde nesse nível estão alocadas diversas máquinas virtuais que simulam o hardware. Isso permite que cada VM tenha seu próprio SO e execute as tarefas como se todo o computador estivesse dedicado a elas. Esse fato provê grande segurança para esse tipo de sistema.

### 2.5.4 Micro-kernel
Os sistemas modernos tendem a tornar o *núcleo* cada vez menor e mais simples. A arquitetura de micro-kernel aplica essa ideia, e funciona como uma relação de *cliente-servidor*. Neste caso a principal função do **núcleo(servidor)** é intermediar a comunicação entre os **processos(clientes)** por meio de **"mensagens"**. Sendo assim apenas o *kernel* roda em **modo protegido** e a maioria dos serviços em **modo usuário**.

### 2.5.5 Exo-kernel
Essa arquitetura, delega a responsabilidade de gerenciamento de recursos direto aos aplicativos. Consiste na multiplexação segura do *hardware*. Fornece alta **abstração** para as aplicações. O conceito de SO é dividido em dois:

1. **Exo-kernel**: Responsável pela multiplexação do hardware.
2. **LibOS (biblioteca do SO)**: conjunto de bibliotecas que gerenciam os recursos e fornecem funções de alto nível para as aplicações. 


