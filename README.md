# so
Sistemas Operacionais - Conceitos


## Memória nos sistemas x86

## História
Quando a Intel projetou o x86 pela primeira vez, tinha a ideia de que cada item de memória estaria em um *segmento de memória*. Cada segmento era um pequeno pedaço de memória de um tamanho específico. Cada segmento serviria a um propósito diferente. 

### O registrador de segmento **CS** (Código)
- O segmento de código (CS) seria usado para armazenar código executável.

### O registrador de segmento **DS** (Dados)
- O segmento padrão (DS)* seria usado para armazenar dados. 

### O registrador de segmento **SS** (Pilha)
- O segmento de pilha (SS) conteria informações de pilha.

### O registrador de segmento **ES** (Estendido)
- O segmento estendido (ES) seria usado para qualquer programador pretendido. 

### Os registradores de segmento **FS** e **GS** (auxiliares)
Posteriormente ao conjunto de registradores de segmentos foram acrescentados os registradores FS e GS. Estes não têm um propósito definido pelo processador, mas o programador pode escolher o que usá-los para si mesmo. No final, as CPUs x86 modernas têm os seguintes seletores de segmento: CS, DS, SS, ES, FS, GS.

A menos que você tenha *habilitado a paginação em sua CPU*, seu processador está operando no *modo de segmentação*.

A CPU usa esses segmentos ao executar determinadas instruções. Por exemplo: 
- ao buscar a próxima instrução, a CPU usa o segmento CS para calcular o endereço físico onde a instrução pode ser encontrada;
- Ao usar a instrução *mov*, ele usa **DS**;
- Ao usar *push* ou *pop*, ele usa **SS**;

### Endereçamento no **Modo Real**
Como a CPU x86 começa a rodar no modo real que você provavelmente não quer usar e quer mudar para o modo de 32 bits com paginação habilitada ou modo de 64 bits (também conhecido como modo longo), precisamos alterar o modo de execução de alguma forma. Para fazer isso, precisamos configurar a CPU e executar determinadas instruções. Isso significa que precisaremos trabalhar um pouco (só um pouco) no modo real e entender como a CPU se comporta nesse modo.


## Segmentação de memória no *Modo Real*:
A CPU base dos sistemas x86, o processador Intel 8086 (e seu irmão 8088) possuem os seguintes registradores para processamento:
![](imagens/cpu-intel-8086.jpg)
![](imagens/8086.jpg)
Poder de endereçamento do processador 8088 (IBM-PC 5150, primeiro PC com MS-DOS 1.0)
*Barramento de dados* ISA (IBM-PC XT) de tamanho *8-bits* para promover compatibilidade de hardware com o processador 8080.
![](imagens/8088-barramento.jpg)
*Barramento de endereçamento de memória* de *20 bits*. O primeiro IBM-PC podia endereçar até 1 MB de memória:
| Operação Binária | Tamanho do Barramento de memória | Endereços possíveis | Tamanho em Bytes |
|------------------|----------------------------------|---------------------|------------------|
| 2^               | 20 bits                          | 1.048.576           | 1MB              |

Neste modelo, teoricamente, a memória de *1MB* podia ser dividia em até 16 segmentos de *64KB*.

| ordem do segmento | Endereço Absoluto | Tamanho segmento |
|:-----------------:|:-----------------:|:----------------:|
|    segmento-01    |    00000-0FFFF    |       64kb       |
|    Segmento-02    |    10000-1FFFF    |       64kb       |
|    Segmento-03    |    20000-2FFFF    |       64kb       |
|    Segmento-04    |    30000-3FFFF    |       64kb       |
|    Segmento-05    |    40000-4FFFF    |       64kb       |
|    Segmento-06    |    50000-5FFFF    |       64kb       |
|    Segmento-07    |    60000-6FFFF    |       64kb       |
|    Segmento-08    |    70000-7FFFF    |       64kb       |
|    Segmento-09    |    80000-8FFFF    |       64kb       |
|    Segmento-10    |    90000-9FFFF    |       64kb       |
|    Segmento-11    |    A0000-AFFFF    |       64kb       |
|    Segmento-12    |    B0000-BFFFF    |       64kb       |
|    Segmento-13    |    C0000-CFFFF    |       64kb       |
|    Segmento-14    |    D0000-DFFFF    |       64kb       |
|    Segmento-15    |    E0000-EFFFF    |       64kb       |
|    Segmento-16    |    F0000-FFFFF    |       64kb       |

A memória pode ser organizada em segmentos de 64KB totalizando até 1MB.
![](imagens/memoria-8086.jpg)

Ao carregar um programa em memória, a CPU 8088 o aloca dentro de um segmento de 64kb. O segmento é dividido em 5 áreas:

| Registrador CPU | Área (assembler) | Nome                           | Finalidade                          |
|-----------------|------------------|--------------------------------|-------------------------------------|
| CS              | . Text           | Código                         |                                     |
|                 | . BSS            | Bloco inicializado por símbolo | Variáveis não inicializadas         |
| DS              | . Data           | Dados                          | Variáveis com valores iniciais      |
|                 | . Heap           |                                | Empilhamento de variáveis dinâmicas |
| SS, SP e BP     | . Stack          | Stack ( pilha de apontamentos) | Empilhamento de ponteiros e funções |
O registrador **BP** contém o endereçamento da base da pilha de apontamento (Stack);
O registrador **SP** contém o endereçamento do topo da pilha de apontamento (Stack);


Como a CPU inicia no modo real, vamos primeiro falar sobre como a segmentação funciona neste modo.
No modo real, a segmentação é relativamente simples. Um endereço lógico de formato A:B (onde A ∈ [CS, SS, DS, ES, FS, GS]) é usado. Este endereço lógico é então traduzido em endereço físico usando *A * 10(hex) + B* . Como no modo real os registradores são limitados a 16 bits para endereçamento, isso nos permite endereçar 2^16 = 64Kb de memória dentro de um único segmento.

Exemplo:
```
O tamanho da pilha de apontamentos STACK de um determinado processo do MS-DOS é o intervalo entre os registradores BP e SP.
Supondo BP contém o valor 3333(hex).
Supondo SP contém o valor 4444(hex)
Usando a fórmula do endereço físico no modo real **A * 10(hex) + B**, calcule o endereço físico de 20 bits na memória para o início e o fim da pilha.

O valor de BP, 3333(hex) é o *A*.
*A* = 3333(hex)

O valor de SP, 4444(hex) é o *B*
*B* = 4444(hex)

Fórmula **A * 10(hex) + B**:
3333(hex) x 10(hex) + 4444(hex)
33330(hex) + 4444(hex) = 37774(hex)

O endereço físico da pilha de apontamentos começa em 33330(hex) e termina em 37774(hex) naquele instante da execução do programa.
```

Destacando a porção de memória dinâmica (HEAP) e a porção de memória de apontamentos, ponteiros e funções (Stack).

![](imagens/ponteiro_memoria.jpg)

Como ficaria um único programa (processo) carregado na memória:

![](imagens/programa_em_memoria.jpg)


## Segmentação de memória no *Modo Protegido*:
No modo protegido, usamos a mesma forma A:B que no modo real, exceto que no modo protegido A não é um valor absoluto do segmento, mas um seletor de segmento. Seletor é um índice em uma tabela. Cada uma das entradas na tabela descreve o segmento (endereço físico, nível de proteção, etc...). O endereço físico do segmento é lido a partir da entrada da tabela (que também é conhecida como seletor de segmento) e B (também conhecido como deslocamento) é adicionado a esse endereço físico para obter o endereço físico real do local da memória.

Existem duas tabelas onde os seletores de segmento podem ser armazenados. Eles são chamados de **Global Descriptor Table** (também conhecido como **GDT** ) e **Local Descriptor Table** ( **LDT** ).

O que isso significa que antes de podermos mudar nossa CPU para o modo protegido, precisamos configurar pelo menos o **GDT**.

Felizmente, há uma boa chance de você nunca precisar configurar o **GDT** para o modo protegido, pois o x86 pode alternar para o modo protegido com a paginação habilitada diretamente do modo real.

## Paginação de Memória
A paginação é uma maneira diferente de acessar a memória física. Em vez de a memória ser dividida em segmentos, ela é dividida em **páginas** e **quadros**. 

### Quadros (Frames)
Quadros ou Frame é um bloco de memória física de um tamanho específico (por exemplo, 4Kb).

### Páginas (Pages)
Página é um bloco de memória virtual do mesmo tamanho. As páginas são do mesmo tamanho que os quadros. 

### Endereços Físicos x Endereços Virtuais: A inclusão da MMU na CPU
Neste modo o programa não acessa a memória usando o endereço físico. Em vez disso, os endereços de memória virtual são usados. Esses endereços virtuais são traduzidos em endereços físicos pela CPU. Este é um processo muito caro comparado ao acesso direto à memória e quase todas as CPUs têm circuitos adicionais para ajudar com isso. Esse circuito é chamado de **MMU** ou **Unidade de Gerenciamento de Memória**.

A imagem a seguir ilustra o processo de tradução de memória. Tenha em mente que a imagem é bastante simplificada e não representa exatamente o que acontece em uma CPU. No entanto, ilustra conceitos básicos sobre os quais construiremos quando estivermos configurando a paginação.

![](imagens/memoria1.jpg)

Para uma CPU traduzir um endereço virtual 0x5006 em um endereço físico, a CPU primeiro pega um prefixo e procura esse prefixo em sua "tabela de páginas" e descobre qual endereço físico está associado a esse prefixo. Por enquanto você pode pensar na tabela de páginas como um lugar especial na CPU. No nosso caso, o endereço físico associado à entrada da tabela de páginas 0x500 é 0x0011. A CPU então pega esse endereço (0x011) e adiciona a última parte do endereço (0x6). 0x0010 + 0x6 = 0x0016. E é assim que a CPU traduz o endereço virtual 0x5006 para o endereço físico 0x0016.

Essa abordagem pode ser estendida para tabelas de páginas de vários níveis:


![](imagens/memoria2.jpg)

Na imagem acima, os primeiros dois bytes são usados como índice em "Diretório de páginas". O byte a seguir é usado como índice na "Tabela de páginas" e o último byte é deslocado na página de memória.

Processadores x86 normalmente usam paginação de 3 ou 4 níveis com os mais recentes podendo usar até sistema de paginação de 5 níveis.


