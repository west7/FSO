# Estrutura de um Sistema Operacional
O SO é formado por um conjunto de rotinas dedicado ao usuário e as aplicações. Esse conjunto pode ser denominado *kernel* (núcleo). Além disso o sistema também apresenta um conjunto de utilitários e linguagem de comando, porém esses não fazem parte da estrutura do SO e são apenas um apoio para o usuário.

## Funções do Kernel
As rotinas de um SO, diferentemente de uma aplicação comum, são concorrentes com base em eventos dissociados do tempo (assíncronos). As principais do núcleo funções são:

- Tratamento de interrupções;
- Criação e eliminação de processos e threads;
- Sincronização e comunicação entre processos e threads;
- Escalonamento e controle dos processos e threads;
- Gerência de memória;
- Gerência do sistema de arquivos;
- Gerência de dispositivos de I/O;
- Suporte a redes;
- Contabilização do uso do sistema; 
- Auditoria e segurança do sistena.

## Modos de acesso
Uma preocupação que surge no projeto de SOs é a proteção e o nível de acesso ao núcleo, pois caso alguma aplicação altere a integridade do kernel todo o sistema pode ser comprometido. Muitas dessas proteções utilizam uma função dos processadores conhecido como **modo de acesso**. 

Em geral, existem 2 modos de acesso: ***Modo Usuário e Modo Kernel (protegido)***. O primeiro permite acesso somente a instruções **não privilegiadas**. Já o Modo Kernel possui acesso a **todas** as instruções do processador. Um SO roda em **Modo Kernel**.

# Rotinas do Sistema e Syscalls
O controle de execução das rotinas do SO é realizado por um mecanismo chamado **System calls (Syscalls)**. Sempre que uma aplicação desejar chamar uma rotina ele realiza uma Syscall, e então, se a aplicação possui a permição necessária, o SO salva o conteúdo dos registradores, troca o modo para protegido, altera o PC para o endereço da rotina chamada. No final da execução o SO altera o modo para usuário e restaura o contexto dos registradores para que a aplicação continue de onde parou.

<div align="center">
    <img src="assets/image2.png" width=500>
</div>

