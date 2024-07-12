# 10. Sistemas de arquivos

Os arquivos são gerenciados pelo o sistema operacional para armazenar (ler e gravar) grande volume de dados de maneira permanente. A parte do sistema responsável por essa atividade é chamada de **sistema de arquivos**. É a parte mais visível de um sistema operacional, visto que a manipulação de arquivos é uma das principais atividades de um usuário.

## 10.1 O que é um arquivo?

Um arquivo nada mais é que uma coleção de dados ou informações que é armazenada em dispositivo de armazenamento, como um SSD ou HD. Arquivos são a unidade básica de um sistema de arquivos e podem armazenar qualquer tipo de dados digitalizados como imagens, vídeos, textos, programas executáveis, entre outros. 

!!! info "Características principais de um arquivo"
    - **Nome**: Todo arquivo é indentificado por um nome, uma sequência de caracteres. Em alguns sistemas há distinção de alfabéticos maiúsculos e minúsculos.
    - **Extensão**: Aos arquivos, geralmente, também são associados um extensão que indica o tipo de dado do conteúdo dele. (.txt, .jpg, .exe, etc).
    - **Localização**: Arquivos são armazenados em [diretórios](./10_arquivos.md#105-diretorios) (pastas), no sistema. A localização de um arquivo é especificada pelo seu caminho (path) absoluto ou relativo.
    - **Atributos**: Arquivos têm atributos que podem ter informações como tamanho, permissão de acesso, data de criação, data de modificação e proprietário.

## 10.2 Organização de arquivos

A organização de arquivos dita como os arquivos serão armazenados no sistema. Cada tipo de arquivo possui uma estrutura interna diferente, por exemplo, um arquivo de texto armazena caracteres em uma determinada codificação, enquanto um arquivo executável possui um formato específico, completamente diferente. No momento de sua criação, o proprietário pode definir a estrutura que será usada no arquivo. Esta organização pode ser suportada pelo SO ou definida pelo própria aplicação.

### 10.2.1 Sequência de bytes

Esta é uma estrutura de organização de arquivos simples, onde não existe estrutura e o arquivo é visto como uma sequência simples de bytes. Neste tipo de organização, o sistema não define nenhuma estrutura lógica para os dados e a aplicação é livre para estabelecer seus prórprios critérios. É um modelo muito flexível para criação de dados, porém o controle de acesso é exclusivo da aplicação. 

### 10.2.2 Sequência de registros

Neste formato, os arquivos são divididos em unidade de mesmo tamanho, chamada registros (semelhante às [páginas](./09_memoria_virtual.md#95-memoria-virtual-por-paginacao)). Há uma dificuldade em utilizar os registros e se o tamanho do registro não for completamente usado têm se uma perda de espaço.

### 10.2.3 Árvores de registros

Neste modelo, o arquivo é um árvore de registros de tamanho variável. Cada registro é ordenado na árvore por um **campo chave**, que têm posição fixa no registro, para tentar otimizar o acesso randômico aos registros.

## 10.3 Tipos de arquivos

Existem 4 principais tipos de arquivos em um sistema:

1. **Regulares**: São aqueles que contêm informações do usuário, geralmente arquivos de texto ASCII ou arquivos binários.
2. **Diretórios**: São um tipo de arquivo usado pelo SO para manter a estrutura do sistema de arquivos.
3. **Caracteres**: Arquivos de caracteres são usadas para manipular dispositivos de I/O, como terminais, impressoras e redes.
4. **Bloco**: Arquivos especiais de blocos são usados para modelar discos.

## 10.4 Métodos de acesso

1. **Acesso Sequencial**: No ínicio, os sistemas só armazenavam informações em fitas magnéticas. Por isso, o acesso aos arquivos só era permitido na ordem em que eram gravados, e a gravação de novos dados só podia ser no final do arquivo.
2. **Acesso Direto**: Neste modelo, o acesso era permitido diretamente a partir de seu número, que era sua posição relativa ao ínicio do arquivo. Nesse contexto, não há restrições em relação a ordem de acesso, desde que seja sempre especificada o número do registro (só funciona se os registros tiverem tamanho fixo).
3. **Acesso com chave**: Neste tipo de acesso, baseado no acesso direto, o acesso aos registros é feito por meio de um índice que armazena uma ponteiro para o registro desejado. Com a chave, o sistema busca pelo índice na área de índices e realiza o acesso a partir do ponteiro.

## 10.5 Diretórios

Diretórios são estruturas de dados que ajudam a organizar logicamente os arquivos. Eles contêm entradas associadas a outros arquivos, armazenando informações e atributos desses arquivos. Existem três principais formas de organização dos diretórios: 

1. **Diretório de nível único**: Existe um diretório único, diretório raiz, que armazena todos os arquivos do sistema. É o nível mais simples de organização.
2. **Diretórios de níveis hierárquicos**: Múltiplos diretórios podem ser criados de maneira hierárquica (diretórios dentro de diretórios), criando uma forma de árvore. 

## 10.6 Gerência de espaço livre em disco

Quando o sistema precisa alocar um arquivo, ele precisa ter a informação sobre quais blocos estão livres e quais não estão. Alguma estrutura de dados armazena essas informações. Assim que um arquivo é alocado o endereço é removido do sistema, para que arquivos não sejam sobreescritos. Da mesma maneira, quando um arquivo é apagado a posição em que ele está alocado é adicionada na estrutura. Pode ser implementado por meio de um [mapa de bits](./08_memoria.md#841-mapa-de-bits) ou uma [lista encadeada](./08_memoria.md#842-lista-encadeada) por exemplo.

## 10.7 Gerência de alocação de espaço

Da mesma forma, também é necessário armazenar as informações sobre os espaços ocupados no sistema.

### 10.7.1 Alocação Contígua 

Os arquivos são armazenados de maneira sequencial no sistema, um bloco atrás do outro. O principal problema é a alocação de novos arquivos, onde deve existir uma quantidade de blocos contíguos suficientes no sistema para suportar o arquivo. Em comparação, o disco pode ser visto como um vetor, em que cada índice é um bloco de registro. Os algoritmos para a alocação de registros são os mesmos que para alocação de memória: [First Fit](./08_memoria.md#831-first-fit-primeiro-encaixe), [Best Fit](./08_memoria.md#832-best-fit-melhor-encaixe) e [Worst Fit](./08_memoria.md#833-worst-fit-pior-encaixe).

Indepedente da forma de alocação, em sistemas de alocação contígua ocorrerá **fragmentação externa** dos espaços livres. O problema pode ser grave quando existe espaço no sistema para alocar um novo arquivo, porém não de maneira contígua. Isso pode ser resolvida por meio de rotinas para reorganizar os registros e formar um grande bloco livre. Sendo assim, o maior problema da alocação contígua é determinar o espaço necessário em disco para alocar um arquivo, visto que o tamanho de um arquivo nem sempre é conhecido no momento de sua criação e pode aumentar com o tempo.
      
### 10.7.2 Alocação Encadeada.

Nesta forma de organização, os registros são armazenados como uma corrente de blocos encadeados, de forma que eles não precisem estar contíguos nem mesmo próximos fisicamente. Ao contrário do modelo anterior, não há problema de fragmentação aqui, o que ocorre é a quebra de um arquivo em vários blocos denominados *extents*, isso resulta em um aumento de tempo de leitura do arquivo, já que ao terminar a leitura de um bloco o sistema precisa deslocar o mecanismo de leitura para a posição do próximo *extent*. A **desfragmentação** também pode ser usada, periodicamente, para diminuir este tempo de acesso.

### 10.7.3 Alocação indexada.

Esta técnica soluciona um dos principais problemas da técnica de alocação encadeada, que é o acesso direto a um determinado bloco de registro sem precisar passar pelos outros blocos anteriores. O primeiro bloco é na verdade um **bloco de índices**, que armazena as posições de todos os outros blocos do arquivo.

## 10.8 Operações com arquivos

| **Operação**                           | **Descrição**                                                                                        |
| -------------------------------------- | ---------------------------------------------------------------------------------------------------- |
| **Create (Criar)**                     | Cria um novo arquivo no sistema de arquivos, especificando nome e atributos iniciais.                |
| **Delete (Excluir)**                   | Remove um arquivo existente do sistema de arquivos, liberando o espaço de armazenamento ocupado.     |
| **Open (Abrir)**                       | Abre um arquivo para acesso (leitura, escrita ou ambos) e retorna um identificador de arquivo.       |
| **Close (Fechar)**                     | Fecha um arquivo aberto, liberando recursos e garantindo que todas as modificações sejam salvas.     |
| **Read (Ler)**                         | Lê dados de um arquivo aberto, especificando a quantidade e a posição de início da leitura.          |
| **Write (Escrever)**                   | Escreve dados em um arquivo aberto, substituindo dados existentes ou escrevendo em uma nova posição. |
| **Append (Anexar)**                    | Adiciona dados ao final de um arquivo aberto sem modificar o conteúdo existente.                     |
| **Seek (Buscar)**                      | Move o ponteiro de leitura/escrita para uma posição específica no arquivo.                           |
| **Get Attributes (Obter Atributos)**   | Recupera informações sobre um arquivo, como tamanho, permissões e datas.                             |
| **Set Attributes (Definir Atributos)** | Modifica os atributos de um arquivo, como permissões e datas.                                        |
| **Rename (Renomear)**                  | Altera o nome de um arquivo existente no sistema de arquivos.                                        |


