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


## Segmentação de memória no *modo Real*:
Como a CPU inicia no modo real, vamos primeiro falar sobre como a segmentação funciona neste modo.

No modo real, a segmentação é relativamente simples. Um endereço lógico de formato A:B (onde A ∈ [CS, SS, DS, ES, FS, GS]) é usado. Este endereço lógico é então traduzido em endereço físico usando A * 0x10 + B. Como no modo real os registradores são limitados a 16 bits para endereçamento, isso nos permite endereçar 2^16 = 64Kb de memória dentro de um único segmento.

