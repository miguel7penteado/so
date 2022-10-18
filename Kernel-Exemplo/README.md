#JOS
Um kernel simples de sistema operacional

## Pré-Requisitos

Aqui utilizaremos uma máquina linux com base para gerar a máquina virtual e as ferramentas utilizadas no exercício:

## Cadeia de ferramentas do compilador
A maioria das distribuições Linux e BSDs modernas tem uma cadeia de ferramentas ELF compatível com os laboratórios JOS. Ou seja, o gcc padrão do sistema, as, ld e objdump devem funcionar. O laboratório Makefile deve detectar isso automaticamente. Se o makefile não detectar suas ferramentas de compilação, você pode especificar sua localização adicionando a seguinte linha a conf/env.mk:

```C
GCCPREFIX=
```

Se você estiver usando algo diferente do linux x86 padrão ou BSD, você precisará da cadeia de ferramentas do compilador GNU C, configurada e construída como um compilador cruzado para o destino 'i386-jos-elf', bem como o depurador GNU, configurado para a cadeia de ferramentas i386-jos-elf. Você pode baixar versões que são conhecidas por trabalhar com JOS através dos links a seguir, embora versões mais recentes do gcc e binutils e GDB também devam funcionar:

- [http://ftp.gnu.org/gnu/binutils/binutils-2.19.tar.bz2](http://ftp.gnu.org/gnu/binutils/binutils-2.19.tar.bz2)
- [http://ftp.gnu.org/gnu/gcc/gcc-4.1.2/gcc-core-4.1.2.tar.bz2](http://ftp.gnu.org/gnu/gcc/gcc-4.1.2/gcc-core-4.1.2.tar.bz2)
- [http://ftp.gnu.org/gnu/gdb/gdb-7.2a.tar.gz](http://ftp.gnu.org/gnu/gdb/gdb-7.2a.tar.gz)

Depois de descompactar esses arquivos, execute os seguintes comandos como root:

```bash 
cd binutils-2.19
./configure --target=i386-jos-elf --disable-nls
make
make install
cd ../gcc-4.1.2
./configure --target=i386-jos-elf --disable-nls --without-headers \
              --with-newlib --disable-threads --disable-shared \
              --disable-libmudflap --disable-libssp
make
make install
cd ../gdb-7.2a
./configure --target=i386-jos-elf --program-prefix=i386-jos-elf- \
              --disable-werror
make
make install
```

Então você terá em /usr/local/bin um monte de binários com nomes como i386-jos-elf-gcc. O makefile do laboratório deve detectar essa cadeia de ferramentas e usá-la de preferência à cadeia de ferramentas padrão da sua máquina. Se isso não funcionar, há instruções sobre como substituir a cadeia de ferramentas dentro do GNUmakefile nos laboratórios.

## GDB corrigido
A versão padrão do gdb não trata corretamente a transição para o modo longo durante a inicialização do JOS, gerando um erro "Packet too long". O patch abaixo irá corrigir isso:

- Versões do GDB até 7.3: [patch](https://www.cs.unc.edu/~porter/courses/comp630/s22/gdb-7.2-arch-remote-change.patch)
- GDB versão 7.7.1 (e posterior): [patch](https://www.cs.unc.edu/~porter/courses/comp630/s22/gdb-7.7.1-mute-message-too-long.patch)

Para o ambiente fornecido, instalamos o pacote gdb corrigido para você. Para depurar código de 64 bits em uma plataforma de 32 bits, você precisa de gdb e gdb-multiarch. Abaixo, publicamos pacotes do Ubuntu corrigidos.

#### AMD64

- [gdb 7.2](https://www.cs.unc.edu/~porter/courses/comp630/s22/gdb_7.2-1ubuntu11jos_amd64.deb).
- [gdb 7.7.1](https://www.cs.unc.edu/~porter/courses/comp630/s22/gdb_7.7.1-0ubuntu5~14.04.2_amd64.deb) (Ubuntu 14.04).
- [gdb 7.11](https://www.cs.unc.edu/~porter/courses/comp630/s22/gdb_7.11-1ubuntu11jos_amd64.deb) (Ubuntu 16.04).
- gdb-multiarch is not required on amd64.

Para compilar o gdb em relação a outra versão no Ubuntu ou Debian, por exemplo, para uso doméstico, siga estas etapas:

```bash
dpkg-source -x gdb-xxxxx.dsc  #(the .dsc you installed)
cd gdb-xxx  #(a newly created directory)
sudo apt-get build-dep gdb
download gdb-7.7.1-mute-message-too-long.patch
patch -p1 < gdb-7.7.1-mute-message-too-long.patch
sudo debian/rules binary
```

## Emulador QEMU
QEMU é um emulador de PC moderno e rápido.

Infelizmente, as facilidades de depuração do QEMU, embora poderosas, são um tanto imaturas, então recomendamos que você use a versão corrigida do QEMU do MIT em vez da versão padrão que pode vir com sua distribuição. A versão instalada em walter.cs.unc.edu já está corrigida. Para construir sua própria versão corrigida do QEMU:

1. Clone o repositório git do MIT QEMU 
```bash
git clone https://github.com/geofft/qemu.git -b 6.828-1.7.0
```

2. No Linux, pode ser necessário instalar as bibliotecas de desenvolvimento SDL para obter uma janela VGA gráfica. No Debian/Ubuntu, este é o pacote libsdl1.2-dev.
3. Configurar o código-fonte

Linux
```bash
./configure --disable-kvm [--prefix=PFX] [--target-list="i386-softmmu x86_64-softmmu"]
```

Mac OS X: 
```C
./configure --disable-kvm --disable-sdl [--prefix=PFX] [--target-list="i386-softmmu x86_64-softmmu"]
```

O argumento prefix especifica onde instalar o QEMU; sem ele, o QEMU será instalado em /usr/local por padrão. O argumento da lista de destino simplesmente reduz as arquiteturas para as quais o QEMU construirá suporte.

4. Execute
```bash
make && make install
```

## Kernel 
GDB é seu amigo. Use o destino [qemu-gdb](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#make-qemu-gdb) (ou sua variante [qemu-gdb-nox](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#make-qemu-gdb-nox)) para fazer o QEMU esperar que o [GDB](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#gdb) seja anexado. Veja a referência do GDB abaixo para alguns comandos que são úteis ao depurar kernels.

Se você está recebendo interrupções inesperadas, exceções ou falhas triplas, você pode pedir ao QEMU para gerar um log detalhado de interrupções usando o argumento [-d](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#qemu--d).

Para depurar problemas de memória virtual, tente os comandos do monitor QEMU [info mem](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#qemu-info-mem) (para uma visão geral de alto nível) ou [info pg](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#qemu-info-pg) (para muitos detalhes). Observe que esses comandos exibem apenas a tabela de páginas atual.

(Lab 4+) Para depurar várias CPUs, use os comandos relacionados a threads do GDB, como threads e threads de informações.

## Ambientes de usuário (laboratório 3+)
O GDB também permite depurar ambientes de usuário, mas há algumas coisas que você precisa observar, pois o GDB não sabe que há uma distinção entre vários ambientes de usuário ou entre usuário e kernel.

Você pode iniciar o JOS com um ambiente de usuário específico usando [make run-name](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#make-run) (ou você pode editar kern/init.c diretamente). Para fazer o QEMU esperar que o GDB seja anexado, use a variante [run-name-gdb](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#make-run-x).

Você pode depurar simbolicamente o código do usuário, assim como o código do kernel, mas você precisa dizer ao GDB qual [tabela de símbolos](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#obj-elf) usar com o comando [symbol-file](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#gdb-symbol-file), pois ele só pode usar uma tabela de símbolos por vez. O .gdbinit fornecido carrega a tabela de símbolos do kernel, obj/kern/kernel. A tabela de símbolos para um ambiente de usuário está em seu binário ELF, então você pode carregá-la usando:
```bash
symbol-file obj/user/name
```
Não carregue símbolos de nenhum arquivo .o, pois eles não foram realocados pelo vinculador (as bibliotecas são vinculadas estaticamente em binários de usuário JOS, portanto, esses símbolos já estão incluídos em cada binário de usuário). Certifique-se de obter o binário de usuário correto; as funções da biblioteca serão vinculadas em diferentes EIPs em diferentes binários e o GDB não conhecerá nada melhor!

(Lab 4+) Como o GDB está conectado à máquina virtual como um todo, ele vê as interrupções do relógio como apenas mais uma transferência de controle. Isso torna basicamente impossível percorrer o código do usuário porque uma interrupção do relógio é praticamente garantida no momento em que você deixa a VM ser executada novamente. O comando [stepi](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#gdb-si) funciona porque suprime as interrupções, mas apenas executa uma instrução de montagem. [Pontos de interrupção](https://www.cs.unc.edu/~porter/courses/comp630/s22/tools.html#gdb-b) geralmente funcionam, mas cuidado, porque você pode atingir o mesmo EIP em um ambiente diferente (na verdade, um binário totalmente diferente!).


## Parte 1: Bootstrap do PC
O objetivo do primeiro exercício é apresentar a linguagem assembly x86 e o ​​processo de bootstrap do PC, e começar com a depuração QEMU e QEMU/GDB. Você não terá que escrever nenhum código para esta parte do laboratório, mas deve passar por isso de qualquer maneira para seu próprio entendimento e estar preparado para responder às perguntas feitas abaixo.

## Introdução à montagem x86
Se você ainda não estiver familiarizado com a linguagem assembly x86, você se familiarizará rapidamente com ela durante este curso! O PC Assembly Language Book é um excelente lugar para começar. Felizmente, o livro contém uma mistura de material novo e antigo para você.

Aviso: Infelizmente, os exemplos do livro foram escritos para o montador NASM, enquanto nós usaremos o montador GNU. O NASM usa a chamada sintaxe Intel enquanto o GNU usa a sintaxe AT&T. Embora semanticamente equivalente, um arquivo assembly diferirá bastante, pelo menos superficialmente, dependendo de qual sintaxe é usada. Felizmente, a conversão entre os dois é bastante simples e é abordada no Guia de Brennan para Montagem Inline.

> Exercício 1. Leia ou pelo menos examine cuidadosamente todo o livro PC Assembly Language, exceto que você deve pular todas as seções após 1.3.5 no capítulo 1, que falam sobre recursos do montador NASM que não se aplicam diretamente ao montador GNU. Você também pode pular os capítulos 5 e 6 e todas as seções em 7.2, que tratam de recursos de processador e idioma que não usaremos. Essa leitura é útil ao tentar entender o assembly em JOS e escrever seu próprio assembly. Se você nunca viu montagem antes, leia este livro com atenção.
> Leia também a seção "A Sintaxe" no Guia de Brennan para Montagem Inline para se familiarizar com os recursos mais importantes da sintaxe do montador GNU. JOS usa o montador GNU.
> Estaremos desenvolvendo JOS para a versão de 64 bits da arquitetura x86 (também conhecida como amd64). O assembly é muito semelhante ao de 32 bits, com algumas diferenças importantes. Leia este guia, que explica as principais diferenças entre a montagem.
> Familiarize-se com a montagem em linha escrevendo um programa simples. Modifique o programa exercício1.c para incluir a montagem em linha que incrementa o valor de x em 1.

Certamente a referência definitiva para programação em linguagem assembly x86 é a referência de arquitetura do conjunto de instruções da Intel, que você pode encontrar na página de referência em dois sabores: uma edição HTML do antigo 80386 Programmer's Reference Manual, que é muito mais curto e fácil de navegar do que o mais recente manuais, mas descreve todos os recursos do processador x86 que usaremos no COMP 630; e os Manuais do Desenvolvedor de Software Combinado Intel 64 e IA-32 completos, mais recentes e melhores da Intel, cobrindo todos os recursos dos processadores mais recentes que não precisaremos em sala de aula, mas você pode estar interessado em aprender. Um conjunto equivalente (mas ainda mais longo) de manuais está disponível na AMD, que também abrange as novas extensões de 64 bits que agora aparecem nos processadores AMD e Intel.
Você deve ler os capítulos recomendados do livro PC Assembly, a seção "The Syntax" no Brennan's Guide e a introdução suave ao AMD 64 agora. Guarde os manuais de arquitetura Intel/AMD para ler mais tarde ou use-os como referência quando quiser procurar a explicação definitiva de um recurso ou instrução específica do processador.

## Simulando o x86
Em vez de desenvolver o sistema operacional em um computador pessoal (PC) físico real, usamos um programa que emula fielmente um PC completo: o código que você escreve para o emulador também será inicializado em um PC real. Usar um emulador simplifica a depuração; você pode, por exemplo, definir pontos de interrupção dentro do x86 emulado, o que é difícil de fazer com a versão de silício de um x86.
No COMP 630 usaremos o Emulador QEMU, um emulador moderno e relativamente rápido. Embora o monitor integrado do QEMU forneça apenas suporte limitado à depuração, o QEMU pode atuar como um destino de depuração remoto para o depurador GNU (GDB), que usaremos neste laboratório para percorrer o processo de inicialização inicial.
Para começar, extraia os arquivos do Lab 1 em seu próprio diretório conforme descrito acima em "Configuração de software" e digite make no diretório do laboratório para compilar o carregador de inicialização mínimo e o kernel com os quais você começará. (É um pouco generoso chamar o código que estamos executando aqui de "kernel", mas vamos detalhar ao longo do semestre.)

```bash
usuario@maquina:/tmp$cd lab
usuario@maquina:/tmp$make
+ as kern/entry.S
+ cc kern/init.c
+ cc kern/console.c
+ cc kern/monitor.c
+ cc kern/printf.c
+ cc lib/printfmt.c
+ cc lib/readline.c
+ cc lib/string.c
+ ld obj/kern/kernel
+ as boot/boot.S
+ cc -Os boot/main.c
+ ld boot/boot
boot block is 414 bytes (max 510)
+ mk obj/kern/kernel.img
```
(Se você receber erros como "referência indefinida para `__udivdi3'", você provavelmente não tem o gcc multilib de 32 bits. Se você estiver executando Debian ou Ubuntu, tente instalar o pacote gcc-multilib.)
Agora você está pronto para executar o QEMU, fornecendo o arquivo obj/kern/kernel.img, criado acima, como o conteúdo do "disco rígido virtual" do PC emulado. Esta imagem de disco rígido contém nosso carregador de inicialização (obj/boot/boot) e nosso kernel (obj/kern/kernel).

```bash
usuario@maquina:/tmp$make qemu
```

Isso executa o QEMU com as opções necessárias para definir o disco rígido e a saída da porta serial direta para o terminal. (Você também pode usar make qemu-nox para executar o QEMU no terminal atual em vez de abrir um novo.)
Algum texto deve aparecer na janela do QEMU:

```bash
Booting from Hard Disk...
6828 decimal is XXX octal!
entering test_backtrace 5
entering test_backtrace 4
entering test_backtrace 3
entering test_backtrace 2
entering test_backtrace 1
entering test_backtrace 0
leaving test_backtrace 0
leaving test_backtrace 1
leaving test_backtrace 2
leaving test_backtrace 3
leaving test_backtrace 4
leaving test_backtrace 5
Welcome to the JOS kernel monitor!
Type 'help' for a list of commands.
K>
```

O comando help é óbvio, e discutiremos em breve o significado do que o comando kerninfo imprime. Embora simples, é importante notar que este monitor de kernel está sendo executado "diretamente" no "hardware bruto (virtual)" do PC simulado. Isso significa que você deve poder copiar o conteúdo de obj/kern/kernel.img nos primeiros setores de um disco rígido real, inserir esse disco rígido em um PC real, ligá-lo e ver exatamente a mesma coisa no tela real do PC como você fez acima na janela QEMU. (No entanto, não recomendamos que você faça isso em uma máquina real com informações úteis em seu disco rígido, porque copiar kernel.img no início de seu disco rígido destruirá o registro mestre de inicialização e o início da primeira partição, efetivamente fazendo com que tudo anteriormente no disco rígido seja perdido!)

## O Espaço de Endereço Físico do PC
Vamos agora mergulhar em um pouco mais de detalhes sobre como um PC é inicializado. O espaço de endereço físico de um PC é conectado para ter o seguinte layout geral:

```
+------------------+  <- 0xFFFFFFFF (4GB)
|      32-bit      |
|  memory mapped   |
|     devices      |
|                  |
/\/\/\/\/\/\/\/\/\/\

/\/\/\/\/\/\/\/\/\/\
|                  |
|      Unused      |
|                  |
+------------------+  <- depends on amount of RAM
|                  |
|                  |
| Extended Memory  |
|                  |
|                  |
+------------------+  <- 0x00100000 (1MB)
|     BIOS ROM     |
+------------------+  <- 0x000F0000 (960KB)
|  16-bit devices, |
|  expansion ROMs  |
+------------------+  <- 0x000C0000 (768KB)
|   VGA Display    |
+------------------+  <- 0x000A0000 (640KB)
|                  |
|    Low Memory    |
|                  |
+------------------+  <- 0x00000000
```

Os primeiros PCs, que eram baseados no processador Intel 8088 de 16 bits, eram capazes de endereçar apenas 1 MB de memória física. O espaço de endereço físico de um PC inicial, portanto, começaria em 0x00000000, mas terminaria em 0x000FFFFFF em vez de 0xFFFFFFFF. A área de 640 KB marcada como "Memória Baixa" era a única memória de acesso aleatório (RAM) que um PC inicial podia usar; na verdade, os PCs mais antigos só podiam ser configurados com 16 KB, 32 KB ou 64 KB de RAM!

A área de 384 KB de 0x000A0000 a 0x000FFFFF foi reservada pelo hardware para usos especiais, como buffers de exibição de vídeo e firmware mantidos em memória não volátil. A parte mais importante dessa área reservada é o BIOS (Basic Input/Output System), que ocupa a região de 64 KB de 0x000F0000 a 0x000FFFFF. Nos primeiros PCs, o BIOS era mantido na verdadeira memória somente leitura (ROM), mas os PCs atuais armazenam o BIOS em memória flash atualizável. O BIOS é responsável por realizar a inicialização básica do sistema, como ativar a placa de vídeo e verificar a quantidade de memória instalada. Após realizar essa inicialização, o BIOS carrega o sistema operacional de algum local apropriado, como disquete, disco rígido, CD-ROM ou a rede, e passa o controle da máquina para o sistema operacional.

Quando a Intel finalmente "quebrou a barreira de um megabyte" com os processadores 80286 e 80386, que suportavam espaços de endereço físico de 16 MB e 4 GB, respectivamente, os arquitetos do PC preservaram o layout original para o baixo 1 MB de espaço de endereço físico para garantir compatibilidade com versões anteriores com softwares existentes. Os PCs modernos, portanto, têm um "buraco" na memória física de 0x000A0000 a 0x00100000, dividindo a RAM em "memória baixa" ou "memória convencional" (os primeiros 640KB) e "memória estendida" (todo o resto). Além disso, algum espaço no topo do espaço de endereço físico de 32 bits do PC, principalmente a RAM física, agora é normalmente reservado pelo BIOS para uso por dispositivos PCI de 32 bits.

Processadores x86 recentes podem suportar mais de 4 GB de RAM física, portanto, a RAM pode se estender além de 0xFFFFFFFF. Nesse caso, o BIOS deve providenciar para deixar um segundo orifício na RAM do sistema na parte superior da região endereçável de 32 bits, para deixar espaço para que esses dispositivos de 32 bits sejam mapeados. Por causa das limitações de projeto, o JOS usará apenas os primeiros 256 MB da memória física de um PC, então, por enquanto, vamos fingir que todos os PCs têm "apenas" um espaço de endereço físico de 32 bits. Mas lidar com espaços de endereços físicos complicados e outros aspectos da organização de hardware que evoluíram ao longo de muitos anos é um dos importantes desafios práticos do desenvolvimento de SO.

## BIOS ROM
Nesta parte do laboratório, você usará os recursos de depuração do QEMU para investigar como um computador compatível com IA-32 é inicializado.

Abra duas janelas de terminal. Em um, digite make qemu-gdb (ou make qemu-nox-gdb). Isso inicia o QEMU, mas o QEMU para logo antes do processador executar a primeira instrução e aguarda uma conexão de depuração do GDB. No segundo terminal, no mesmo diretório que você executou make, execute gdb. Você deve ver algo assim,

```bash 
usuario@maquina:/tmp$gdb
GNU gdb (GDB) 6.8-debian
Copyright (C) 2008 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i486-linux-gnu".
+ target remote localhost:1234
The target architecture is assumed to be i8086
[f000:fff0] 0xffff0:	ljmp   $0xf000,$0xe05b
0x0000fff0 in ?? ()
(gdb)
```

Fornecemos um arquivo .gdbinit que configura o GDB para depurar o código de 16 bits usado durante a inicialização inicial e o direcionamos para anexar ao QEMU de escuta.
A seguinte linha:

```gdb
[f000:fff0] 0xffff0:	ljmp   $0xf000,$0xe05b
```

é a desmontagem do GDB da primeira instrução a ser executada. A partir desta saída, você pode concluir algumas coisas:

- O IBM PC começa a ser executado no endereço físico 0x000ffff0, que está no topo da área de 64 KB reservada para o ROM BIOS.
- O PC inicia a execução com CS = 0xf000 e IP = 0xfff0.
- A primeira instrução a ser executada é uma instrução jmp, que salta para o endereço segmentado CS = 0xf000 e IP = 0xe05b.

Por que o QEMU começa assim? Foi assim que a Intel projetou o processador 8088, que a IBM usou em seu PC original. Como o BIOS em um PC é "conectado" ao intervalo de endereço físico 0x000f0000-0x000fffff, esse design garante que o BIOS sempre obtenha o controle da máquina primeiro após a inicialização ou qualquer reinicialização do sistema - o que é crucial porque, ao ligar não há nenhum outro software em qualquer lugar na RAM da máquina que o processador possa executar. O emulador QEMU vem com seu próprio BIOS, que é colocado neste local no espaço de endereço físico simulado do processador. Na reinicialização do processador, o processador (simulado) entra no modo real e define CS como 0xf000 e o IP como 0xfff0, para que a execução comece nesse endereço de segmento (CS:IP). Como o endereço segmentado 0xf000:fff0 se transforma em um endereço físico?

Para responder isso, precisamos saber um pouco sobre endereçamento em modo real. No modo real (o modo em que o PC inicia), a tradução de endereços funciona de acordo com a fórmula: endereço físico = 16 * segmento + deslocamento. Assim, quando o PC configura CS para 0xf000 e IP para 0xfff0, o endereço físico referenciado é:

```
 16 * 0xf000 + 0xfff0   # in hex multiplication by 16 is
   = 0xf0000 + 0xfff0     # easy--just append a 0.
   = 0xffff0
```
0xffff0 é 16 bytes antes do final do BIOS (0x100000). Portanto, não deveríamos nos surpreender que a primeira coisa que o BIOS faz seja voltar para um local anterior no BIOS; afinal, quanto poderia realizar em apenas 16 bytes?

> Exercício 2. Use o comando si (Step Instruction) do GDB para rastrear no BIOS da ROM para obter mais algumas instruções e tente adivinhar o que ele pode estar fazendo. Você pode querer ver Phil Storrs I/O Ports Description, bem > como outros materiais na página de materiais de referência. Não há necessidade de descobrir todos os detalhes - apenas a ideia geral do que o BIOS está fazendo primeiro.

Quando o BIOS é executado, ele configura uma tabela de descritor de interrupção e inicializa vários dispositivos, como a tela VGA. É daí que vêm as mensagens "Iniciando o SeaBIOS" que você vê na janela do QEMU.

Depois de inicializar o barramento PCI e todos os dispositivos importantes que o BIOS conhece, ele procura um dispositivo inicializável, como um disquete, disco rígido ou CD-ROM. Eventualmente, quando encontra um disco inicializável, o BIOS lê o carregador de inicialização do disco e transfere o controle para ele.

## Parte 2: O carregador de inicialização
Disquetes e discos rígidos para PCs são divididos em regiões de 512 bytes chamadas setores. Um setor é a granularidade de transferência mínima do disco: cada operação de leitura ou gravação deve ter um ou mais setores de tamanho e alinhadas em um limite de setor. Se o disco for inicializável, o primeiro setor é chamado de setor de inicialização, pois é onde reside o código do carregador de inicialização. Quando o BIOS encontra um disquete ou disco rígido inicializável, ele carrega o setor de inicialização de 512 bytes na memória nos endereços físicos 0x7c00 a 0x7dff e, em seguida, usa uma instrução jmp para definir o CS:IP para 0000:7c00, passando o controle para o boot carregador. Como o endereço de carregamento do BIOS, esses endereços são bastante arbitrários - mas são fixos e padronizados para PCs.

A capacidade de inicializar a partir de um CD-ROM veio muito mais tarde durante a evolução do PC e, como resultado, os arquitetos do PC aproveitaram a oportunidade para repensar um pouco o processo de inicialização. Como resultado, a maneira como um BIOS moderno inicializa a partir de um CD-ROM é um pouco mais complicado (e mais poderoso). Os CD-ROMs usam um tamanho de setor de 2048 bytes em vez de 512, e o BIOS pode carregar uma imagem de inicialização muito maior do disco para a memória (não apenas um setor) antes de transferir o controle para ele. Para obter mais informações, consulte a especificação de formato de CD-ROM inicializável "El Torito".

Para esta classe, no entanto, usaremos o mecanismo convencional de inicialização do disco rígido, o que significa que o BIOS carregará apenas os primeiros 512 bytes. O bootloader (boot/boot.S e boot/main.c) faz o trabalho de inicialização. Examine esses arquivos de origem com cuidado e certifique-se de entender o que está acontecendo. O carregador de inicialização deve executar as seguintes funções principais:

1. Primeiro, o carregador de inicialização obtém do BIOS um mapa da memória física presente no sistema. Isso é feito usando uma chamada de sistema para o BIOS (int 0x15), que retorna uma estrutura chamada mapa e820. Esta chamada só é possível enquanto o processador ainda está em modo real. O bootloader do JOS constrói uma estrutura de inicialização múltipla, que passa para o kernel. A inicialização múltipla é um padrão para passar informações de inicialização do carregador de inicialização para um kernel.
2. O carregador de inicialização então muda o processador do modo real para o modo protegido de 32 bits, porque é somente nesse modo que o software pode acessar toda a memória acima de 1 MB no espaço de endereço físico do processador. O modo protegido é descrito brevemente nas seções 1.2.7 e 1.2.8 do PC Assembly Language e em detalhes nos manuais de arquitetura Intel. Neste ponto, você só precisa entender que a tradução de endereços segmentados (segmento: pares de deslocamento) em endereços físicos acontece de maneira diferente no modo protegido e que após a transição os deslocamentos são de 32 bits em vez de 16.
Um dos recursos arcanos do x86 que manipula o bootloader é configurar corretamente o endereço bit 20. Para entender melhor isso, dê uma olhada neste artigo.
3. O bootloader configura a pilha e começa a executar o código C do kernel em boot/main.c.
4. Finalmente, o carregador de inicialização lê o kernel do disco rígido acessando diretamente os registros do dispositivo de disco IDE através das instruções especiais de E/S do x86. Se você quiser entender melhor o que significam as instruções de E/S específicas aqui, confira a seção "Controlador de disco rígido IDE" na página de referência. Você não precisará aprender muito sobre programação de dispositivos específicos nesta aula: escrever drivers de dispositivo é, na prática, uma parte muito importante do desenvolvimento do SO, mas do ponto de vista conceitual ou arquitetônico também é um dos menos interessantes.

Depois de entender o código-fonte do carregador de inicialização, examine os arquivos obj/boot/boot.asm. Este arquivo é uma desmontagem do carregador de inicialização que nosso GNUmakefile cria após compilar o carregador de inicialização. Esse arquivo de desmontagem facilita ver exatamente onde na memória física reside todo o código do carregador de inicialização e facilita o rastreamento do que está acontecendo ao percorrer o carregador de inicialização no GDB.

Você pode definir pontos de interrupção de endereço no GDB com o comando b. Você precisa iniciar os números hexadecimais com 0x, então diga algo como hb *0x7c00 define um ponto de interrupção no endereço 0x7C00. Nota: um artefato de nossa infraestrutura faz com que b não funcione corretamente; use hb em vez disso. Uma vez em um ponto de interrupção, você pode continuar a execução usando os comandos c e si: c faz com que o QEMU continue a execução até o próximo ponto de interrupção (ou até que você pressione Ctrl-C no GDB), e si N percorre as instruções N de cada vez.

Para examinar as instruções na memória (além da próxima a ser executada, que o GDB imprime automaticamente), você usa o comando x/i. Este comando tem a sintaxe x/Ni ADDR, onde N é o número de instruções consecutivas para desmontar e ADDR é o endereço de memória no qual iniciar a desmontagem.

> Exercício 3. Dê uma olhada no guia de ferramentas do laboratório, especialmente a seção sobre comandos GDB. Mesmo se você estiver familiarizado com o GDB, isso inclui alguns comandos esotéricos do GDB que são úteis para o  trabalho do sistema operacional.
> Defina um ponto de interrupção no endereço 0x7c00, que é onde o setor de inicialização será carregado. Continue a execução até esse ponto de interrupção. Rastreie o código em boot/boot.S, usando o código-fonte e o arquivo de desmontagem obj/boot/boot.asm para acompanhar onde você está. Use também o comando x/i no GDB para desmontar sequências de instruções no carregador de inicialização e compare o código-fonte do carregador de inicialização original com a desmontagem em obj/boot/boot.asm e GDB.
> Trace em bootmain() em boot/main.c, e então em readsect(). Identifique as instruções de montagem exatas que correspondem a cada uma das instruções em readsect(). Rastreie o restante de readsect() e volte para bootmain(), e identifique o início e o fim do loop for que lê os setores restantes do kernel do disco. Descubra qual código será executado quando o loop for concluído, defina um ponto de interrupção e continue até esse ponto de interrupção. Em seguida, percorra o restante do carregador de inicialização.

Ser capaz de responder às seguintes perguntas:

- Em que ponto o processador começa a executar o código de 32 bits? O que exatamente causa a mudança do modo de 16 para 32 bits?
- Qual é a última instrução do carregador de inicialização executada e qual é a primeira instrução do kernel que ele acabou de carregar?
- Como o carregador de inicialização decide quantos setores ele deve ler para buscar todo o kernel do disco? Onde encontra esta informação?

## Carregando o Kernel

Agora veremos com mais detalhes a parte da linguagem C do carregador de inicialização, em boot/main.c.

Para entender boot/main.c você precisa saber o que é um binário ELF. Quando você compila e vincula um programa C como o kernel JOS, o compilador transforma cada arquivo de origem C ('.c') em um arquivo objeto ('.o') contendo instruções em linguagem assembly codificadas no formato binário esperado pelo hardware . O vinculador então combina todos os arquivos de objeto compilados em uma única imagem binária, como obj/kern/kernel, que neste caso é um binário no formato ELF, que significa "Formato executável e vinculável".

Informações completas sobre esse formato estão disponíveis na especificação ELF em nossa página de referência, mas você não precisará se aprofundar nos detalhes desse formato nesta aula. Embora como um todo o formato seja bastante poderoso e complexo, a maioria das partes complexas são para suportar o carregamento dinâmico de bibliotecas compartilhadas, o que não faremos nesta aula.

Para fins desta classe, você pode considerar um executável ELF como um cabeçalho com informações de carregamento, seguido por várias seções de programa, cada uma das quais é um pedaço contíguo de código ou dados destinados a serem carregados na memória em um endereço especificado. O carregador de inicialização não modifica o código ou os dados; ele o carrega na memória e começa a executá-lo.

Um binário ELF começa com um cabeçalho ELF de comprimento fixo, seguido por um cabeçalho de programa de comprimento variável listando cada uma das seções do programa a serem carregadas. As definições C para esses cabeçalhos ELF estão em inc/elf.h. As seções do programa em que estamos interessados ​​são:

- .text: As instruções executáveis do programa.
- .rodata: Dados atentos ao compilador C. No entanto, nos lembramos em configurar o hardware para proibir a gravação.)
- .data: A seção de dados contém os dados inicializados do programa, como globais globais declaradas com inicializadores int x = 5;

Quando o vinculador calcula o layout de memória de um programa, ele reserva espaço para variáveis globais não inicializadas, como int x;, em uma seção chamada .bss que segue imediatamente .data na memória. C requer que as variáveis globais "não inicializadas" comecem com um valor de zero. Assim, não há necessidade de armazenar conteúdo para .bss no binário ELF; em vez disso, o vinculador registra apenas o endereço e o tamanho da seção .bss. O carregador ou o próprio programa deve providenciar para zerar a seção .bss.

Você pode exibir uma lista completa dos nomes, tamanhos e endereços de link de todas as seções no executável do kernel digitando:

```bash
usuario@maquina:/tmp$ objdump -h obj/kern/kernel
```
Você verá muito mais seções do que as listadas acima, mas as outras não são importantes para nossos propósitos. A maioria dos outros contém informações de depuração, que normalmente são incluídas no arquivo executável do programa, mas não são carregadas na memória pelo carregador do programa.

Tome nota especial do "VMA" (ou endereço de link) e do "LMA" (ou endereço de carga) da seção .text. O endereço de carregamento de uma seção é o endereço de memória no qual essa seção deve ser carregada na memória. No objeto ELF, isso é armazenado no campo ph->p_pa (neste caso, é realmente um endereço físico, embora a especificação ELF seja vaga no significado real desse campo).

O endereço de link de uma seção é o endereço de memória do qual a seção espera executar. O vinculador codifica o endereço do link no binário de várias maneiras, como quando o código precisa do endereço de uma variável global, com o resultado de que um binário geralmente não funcionará se estiver sendo executado a partir de um endereço para o qual não está vinculado . (É possível gerar código independente de posição que não contenha nenhum desses endereços absolutos. Isso é usado extensivamente por bibliotecas compartilhadas modernas, mas tem custos de desempenho e complexidade, portanto, não o usaremos nesta classe.)

Normalmente, o link e os endereços de carregamento são os mesmos. Por exemplo, veja a seção .text do carregador de inicialização:

```bash
usuario@maquina:/tmp$ objdump -h obj/boot/boot.out
```
O BIOS carrega o setor de inicialização na memória começando no endereço 0x7c00, portanto, este é o endereço de carregamento do setor de inicialização. Este também é o local de onde o setor de inicialização é executado, portanto, esse também é seu endereço de link. Definimos o endereço do link passando -Ttext 0x7C00 para o vinculador em boot/Makefrag, para que o vinculador produza os endereços de memória corretos no código gerado.

> Exercício 4. Rastreie as primeiras instruções do carregador de inicialização novamente e identifique a primeira instrução que "quebraria" ou faria a coisa errada se você obtivesse o endereço de link do carregador de inicialização errado. Em seguida, altere o endereço do link em boot/Makefrag para algo errado, execute make clean, recompile o laboratório com make e rastreie o carregador de inicialização novamente para ver o que acontece. Não se esqueça de alterar o endereço do link de volta e limpar novamente depois!

Reveja os endereços de carregamento e link para o kernel. Ao contrário do carregador de inicialização, esses dois endereços não são os mesmos: o kernel está dizendo ao carregador de inicialização para carregá-lo na memória em um endereço baixo (1 megabyte), mas espera executar a partir de um endereço alto. Vamos nos aprofundar em como fazemos isso funcionar na próxima seção.

Além das informações da seção, há mais um campo no cabeçalho ELF que é importante para nós, chamado e_entry. Este campo contém o endereço do link do ponto de entrada no programa: o endereço de memória na seção de texto do programa no qual o programa deve começar a ser executado. Você pode ver o ponto de entrada:

```bash
usuario@maquina:/tmp$ objdump -f obj/kern/kernel
```

Agora você deve ser capaz de entender o carregador ELF mínimo em boot/main.c. Ele lê cada seção do kernel do disco para a memória no endereço de carga da seção e então salta para o ponto de entrada do kernel.

> Exercício 5. Podemos examinar a memória usando o comando x do GDB. O manual do GDB tem todos os detalhes, mas por enquanto, basta saber que o comando x/Nx ADDR imprime N palavras de memória no ADDR. (Observe que ambos os 'x's no comando são minúsculos.) Aviso: O tamanho de uma palavra não é um padrão universal. No assembly GNU, uma palavra tem dois bytes (o 'w' em xorw, que significa palavra, significa 2 bytes).
> Reinicie a máquina (saia do QEMU/GDB e inicie-os novamente). Examine as 8 palavras de memória em 0x00100000 no ponto em que o BIOS entra no carregador de inicialização e, novamente, no ponto em que o carregador de inicialização entra no kernel. Por que eles são diferentes? O que há no segundo ponto de interrupção? (Você realmente não precisa usar o QEMU para responder a esta pergunta. Apenas pense.)

## Link vs. Endereço de carregamento
O endereço de carregamento de um binário é o endereço de memória no qual um binário é realmente carregado. Por exemplo, o BIOS é carregado pelo hardware do PC no endereço 0xf0000. Portanto, este é o endereço de carregamento do BIOS. Da mesma forma, o BIOS carrega o setor de inicialização no endereço 0x7c00. Portanto, este é o endereço de carga do setor de inicialização.

O endereço de link de um binário é o endereço de memória para o qual o binário está vinculado. Vincular um binário para um determinado endereço de link o prepara para ser carregado nesse endereço. O vinculador codifica o endereço do link no binário de várias maneiras, por exemplo, quando o código precisa do endereço de uma variável global, com o resultado de que um binário geralmente não funcionará se não for carregado no endereço para o qual está vinculado .

Em uma frase: o endereço do link é o local onde um binário assume que será carregado, enquanto o endereço de carregamento é o local onde um binário é carregado. Cabe a nós garantir que eles sejam os mesmos.

Veja o comando -Ttext linker em boot/Makefrag e o endereço mencionado no início do script do linker em kern/kernel.ld. Eles definem o endereço do link para o carregador de inicialização e o kernel, respectivamente.

Quando o código objeto não contém endereços absolutos que codificam o endereço do link dessa maneira, dizemos que o código é independente da posição: ele se comportará corretamente, não importa onde seja carregado. O GCC pode gerar código independente de posição usando a opção -fpic, e esse recurso é usado extensivamente em bibliotecas compartilhadas modernas que usam o formato executável ELF. A independência de posição normalmente tem algum custo de desempenho, no entanto, porque restringe as maneiras pelas quais o compilador pode escolher instruções para acessar os dados do programa. Não usaremos -fpic nesta classe.

## Parte 3: O Kernel
Vamos agora começar a examinar o kernel JOS mínimo com um pouco mais de detalhes. (E você finalmente conseguirá escrever algum código!). Como o carregador de inicialização, o kernel começa com algum código em linguagem assembly que configura as coisas para que o código em linguagem C possa ser executado corretamente.

O código assembly inicial do kernel faz o seguinte:

1. Quando o kernel começa a ser executado, o processador está no modo protegido de 32 bits. A primeira coisa que o kernel faz é testar se a CPU suporta o modo longo (64 bits).
2. O kernel inicializa um conjunto simples de tabelas de páginas para os primeiros 4 GB de memória. Essas páginas mapeiam endereços virtuais nos 3 GB mais baixos para os mesmos endereços físicos e, em seguida, mapeiam os 256 MB superiores de volta para os 256 MB de memória mais baixos. Neste ponto, o kernel coloca a CPU em modo longo. O kernel pode determinar dinamicamente se deve ser executado no modo de 64 ou 32 bits com base no suporte da CPU ao modo longo. Claro, isso complicaria substancialmente o kernel.
3. Finalmente, o kernel configura a pilha e algumas outras coisas para começar a executar o código C.

Examine os arquivos de origem do kernel kern/entry.S e kern/bootstrap.S e seja capaz de responder à seguinte pergunta

- Em que ponto o processador começa a executar o código de 64 bits? O que exatamente causa a mudança do modo de 32 para 64 bits?

## Usando a segmentação para contornar a dependência da posição
Você notou acima que, embora o link do carregador de inicialização e os endereços de carregamento correspondam perfeitamente, parece haver uma disparidade (bastante grande) entre o link do kernel e os endereços de carregamento? Volte e verifique ambos e certifique-se de que você pode ver do que estamos falando.

Os kernels do sistema operacional geralmente gostam de ser vinculados e executados em um endereço virtual muito alto, como 0x8004100000, para deixar a parte inferior do espaço de endereço virtual do processador para os programas do usuário usarem. A razão para este arranjo ficará mais clara no próximo laboratório.

Muitas máquinas não possuem memória física no endereço 0x8004100000, então não podemos contar com a possibilidade de armazenar o kernel lá. Em vez disso, usaremos o hardware de gerenciamento de memória do processador para mapear o endereço virtual 0x8004100000 - o endereço do link no qual o código do kernel espera ser executado - para o endereço físico 0x100000 --- onde o carregador de inicialização carregou o kernel. Dessa forma, embora o endereço virtual do kernel seja alto o suficiente para deixar bastante espaço de endereçamento para os processos do usuário, ele será carregado na memória física no ponto de 1 MB da RAM do PC, logo acima da ROM do BIOS. Essa abordagem requer que o PC tenha pelo menos alguns megabytes de memória física (para que o endereço 0x00100000 funcione), mas é provável que isso seja verdade para qualquer PC construído depois de 1990.

Eventualmente, mapeamos todos os 256 MB inferiores do espaço de endereço físico do PC, de 0x00000000 a 0x0fffffff, para endereços virtuais 0x8004000000 a 0x8013FFFFFF, respectivamente.
Por enquanto, o kernel irá configurar inicialmente tabelas de páginas escritas à mão e inicializadas estaticamente em kern/bootstrap.S. Por enquanto, você não precisa entender os detalhes de como isso funciona, apenas o efeito que ele produz. Até kern/bootstrap.S definir o sinalizador CR0_PG, as referências de memória são tratadas como endereços físicos (estritamente falando, são endereços lineares, mas boot/boot.S configura um mapeamento de identidade de endereços lineares para endereços físicos e nós estamos nunca vai mudar isso). Depois que CR0_PG é definido, as referências de memória são endereços virtuais que são traduzidos pelo hardware de memória virtual para endereços físicos. pml4 traduz endereços virtuais no intervalo de 0x8004000000 a 0x8013ffffff para endereços físicos de 0x00000000 a 0x0ffffff, bem como endereços virtuais de 0x00000000 a 0xefffffff para endereços físicos de 0x00000000 a 0xeffffff.

> Exercício 6. Use QEMU e GDB para rastrear o código de inicialização do kernel JOS inicial (no diretório kern/boostrap.S) e descobrir onde o novo mapeamento virtual para físico entra em vigor. Em seguida, examine a Global Descriptor Table (GDT) que o código usa para obter esse efeito e certifique-se de entender o que está acontecendo.
> Qual é a primeira instrução após o estabelecimento do novo mapeamento que não funcionaria corretamente se o mapeamento antigo ainda estivesse em vigor? Comente ou quebre intencionalmente o código de configuração de segmentação em kern/entry.S, rastreie-o e veja se você estava certo.

## Impressão formatada no console
A maioria das pessoas aceita funções como printf() como garantidas, às vezes até pensando nelas como "primitivas" da linguagem C. Mas em um kernel do sistema operacional, temos que implementar todas as E/S por conta própria.

Leia kern/printf.c, lib/printfmt.c e kern/console.c, e certifique-se de entender o relacionamento deles. Ficará claro em laboratórios posteriores porque printfmt.c está localizado no diretório lib separado.

> Exercício 7. Omitimos um pequeno fragmento de código - o código necessário para imprimir números octais usando padrões da forma "%o". Encontre e preencha este fragmento de código.

Ser capaz de responder às seguintes perguntas:

1. Explique a interface entre printf.c e console.c. Especificamente, qual função o console.c exporta? Como esta função é usada por printf.c?
2. Explique o seguinte de console.c:

```C
if (crt_pos >= CRT_SIZE)
{
    int i;
    memmove(crt_buf, crt_buf + CRT_COLS, (CRT_SIZE - CRT_COLS) * sizeof(uint16_t));
    for (i = CRT_SIZE - CRT_COLS; i < CRT_SIZE; i++)
            crt_buf[i] = 0x0700 | ' ';
    crt_pos -= CRT_COLS;
}
```

3. Rastreie a execução do seguinte código passo a passo:

```C
int x = 1, y = 3, z = 4;
cprintf("x %d, y %x, z %d\n", x, y, z);
````

- Na chamada para cprintf(), para que aponta fmt? Para o que ap aponta?
- Liste (em ordem de execução) cada chamada para cons_putc, va_arg e vcprintf. Para cons_putc, liste seu argumento também. Para va_arg, liste o que ap aponta antes e depois da chamada. Para vcprintf liste os valores de seus dois argumentos.

```C
	unsigned int i = 0x00646c72;
	cprintf("H%x Wo%s", 57616, &i);
```

Qual é a saída? Explique como essa saída é obtida na maneira passo a passo do exercício anterior. Aqui está uma tabela ASCII que mapeia bytes para caracteres.
A saída depende do fato de que o x86 é little-endian. Se o x86 fosse big-endian, o que você definiria para i para produzir a mesma saída? Você precisaria alterar 57616 para um valor diferente?

Aqui está uma descrição de little e big-endian e uma descrição mais caprichosa.

5. No código a seguir, o que será impresso após 'y='? (nota: a resposta não é um valor específico.) Por que isso acontece?

```C
	cprintf("x=%d y=%d", 3);
```

6. Digamos que o GCC mudou sua convenção de chamada para que ele empurre argumentos na pilha na ordem de declaração, de modo que o último argumento seja enviado por último. Como você teria que alterar o cprintf ou sua interface para que ainda fosse possível passar um número variável de argumentos?

> Desafio 1 (2 pontos de bônus) Melhore o console para permitir que o texto seja impresso em cores diferentes. A maneira tradicional de fazer isso é fazê-lo interpretar sequências de escape ANSI incorporadas nas strings de texto impressas no console, mas você pode usar qualquer mecanismo que desejar. Há muitas informações na página de referência e em outros lugares da web sobre como programar o hardware de exibição VGA. Se você estiver se sentindo realmente aventureiro, você pode tentar mudar o hardware VGA para um modo gráfico e fazer o console desenhar texto no buffer de quadros gráfico.

## A pilha (de apontamentos)
No exercício final deste laboratório, exploraremos com mais detalhes a maneira como a linguagem C usa a pilha no x86 e, no processo, escreveremos uma nova função de monitor de kernel útil que imprime um backtrace da pilha: uma lista dos arquivos salvos Valores do Ponteiro de Instrução (IP) das instruções de chamada aninhadas que levaram ao ponto de execução atual.

> Exercício 8. Determine onde o kernel inicializa sua pilha e exatamente onde na memória sua pilha está localizada. Como o kernel reserva espaço para sua pilha? E para qual "final" dessa área reservada o ponteiro da pilha é inicializado para apontar?

O ponteiro de pilha x86-64 (registro rsp) aponta para o local mais baixo na pilha que está em uso no momento. Tudo abaixo desse local na região reservada para a pilha é gratuito. Empurrar um valor para a pilha envolve diminuir o ponteiro da pilha e, em seguida, escrever o valor no local para o qual o ponteiro da pilha aponta. Retirar um valor da pilha envolve ler o valor para o qual o ponteiro da pilha aponta e, em seguida, aumentar o ponteiro da pilha. No modo de 64 bits, a pilha só pode conter valores de 64 bits e rsp é sempre divisível por oito. Várias instruções x86-64, como call, são "hard-wired" para usar o registrador de ponteiro de pilha.

O registrador rbp (ponteiro base), em contraste, está associado à pilha principalmente por convenção de software. Ao entrar em uma função C, o código de prólogo da função normalmente salva o ponteiro base da função anterior empurrando-o para a pilha e, em seguida, copia o valor rsp atual em rbp durante a duração da função. Se todas as funções em um programa obedecerem a esta convenção, então, em qualquer ponto durante a execução do programa, é possível rastrear de volta pela pilha seguindo a cadeia de ponteiros rbp salvos e determinando exatamente qual sequência aninhada de chamadas de função causou esse problema específico. ponto do programa a ser alcançado. Esse recurso pode ser particularmente útil, por exemplo, quando uma função específica causa uma falha de declaração ou pânico porque argumentos incorretos foram passados ​​para ela, mas você não tem certeza de quem passou os argumentos incorretos. Um backtrace de pilha permite que você encontre a função incorreta.

> Exercício 9. Para se familiarizar com as convenções de chamada C no x86-64, encontre o endereço da função test_backtrace em obj/kern/kernel.asm, defina um ponto de interrupção e examine o que acontece cada vez que é chamado após o kernel começa. Quantas palavras de 64 bits cada nível de aninhamento recursivo de test_backtrace empurra na pilha e quais são essas palavras?
> Observe que, para que este exercício funcione corretamente, você deve usar a versão corrigida do QEMU disponível na página de ferramentas ou na máquina virtual do seu curso. Caso contrário, você terá que traduzir manualmente todos os pontos de interrupção e endereços de memória para endereços lineares.

O exercício acima deve fornecer as informações necessárias para implementar uma função de backtrace de pilha, que você deve chamar mon_backtrace(). Um protótipo para esta função já está esperando por você em kern/monitor.c. Você pode fazer isso inteiramente em C, mas pode achar útil a função read_rbp() em inc/x86.h. Você também terá que conectar essa nova função à lista de comandos do monitor do kernel para que ela possa ser invocada interativamente pelo usuário.

A função backtrace deve exibir uma lista de quadros de chamada de função no seguinte formato:

```bash
Stack backtrace:
  rbp 00000000f0111f20  rip 00000000f01000be
  rbp 00000000f0111f40  rip 00000000f01000a1
  ...
```

A primeira linha impressa reflete a função atualmente em execução, ou seja, o próprio mon_backtrace, a segunda linha reflete a função que chamou mon_backtrace, a terceira linha reflete a função que chamou aquela e assim por diante. Você deve imprimir todos os quadros de pilha pendentes. Ao estudar kern/entry.S, você descobrirá que existe uma maneira fácil de saber quando parar.

Dentro de cada linha, o valor rbp indica o ponteiro base na pilha usada por essa função: ou seja, a posição do ponteiro da pilha logo após a entrada da função e o código do prólogo da função configurar o ponteiro base. O valor rip listado é o ponteiro de instrução de retorno da função: o endereço de instrução para o qual o controle retornará quando a função retornar. O ponteiro de instrução de retorno normalmente aponta para a instrução após a instrução de chamada (por quê?).

Leia [este artigo](http://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64.html) sobre como os argumentos são mapeados para registros na arquitetura x86-64. Isso é chamado de convenção de chamada, pois os desenvolvedores de software concordam com esse mapeamento por convenção. Como o artigo explica, compiladores diferentes podem ter padrões diferentes. Por exemplo, a convenção de chamada do Windows x86-64 difere da convenção de chamada do Linux x86-64.

Aqui estão alguns pontos específicos sobre os quais você leu no Capítulo 5 de K&R que vale a pena lembrar para o exercício a seguir e para laboratórios futuros.

- Se int *p = (int*)100, então (int)p + 1 e (int)(p + 1) são números diferentes: o primeiro é 101, mas o segundo é 104. Ao adicionar um inteiro a um ponteiro, como no segundo caso, o inteiro é implicitamente multiplicado pelo tamanho do objeto para o qual o ponteiro aponta.
-  p[i] é definido como o mesmo que *(p+i), referindo-se ao i'ésimo objeto na memória apontada por p. A regra de adição acima ajuda essa definição a funcionar quando os objetos são maiores que um byte.
- &p[i] é o mesmo que (p+i), produzindo o endereço do i'ésimo objeto na memória apontada por p.

Embora a maioria dos programas C nunca precise converter ponteiros e inteiros, os sistemas operacionais frequentemente precisam. Sempre que você vir uma adição envolvendo um endereço de memória, pergunte a si mesmo se é uma adição de inteiro ou de ponteiro e certifique-se de que o valor que está sendo adicionado seja multiplicado adequadamente ou não.

> Exercício 10. Implemente a função backtrace conforme especificado acima. Use o mesmo formato do exemplo, caso contrário o script de avaliação ficará confuso. Quando você achar que está funcionando corretamente, execute make grade para ver se sua saída está de acordo com o que nosso script de classificação espera e corrija-o se não estiver. Depois de entregar o código do Lab 1, você pode alterar o formato de saída da função backtrace da maneira que desejar.

Neste ponto, sua função backtrace deve fornecer os endereços dos chamadores da função na pilha que levam à execução de mon_backtrace(). No entanto, na prática, muitas vezes você deseja saber os nomes das funções correspondentes a esses endereços. Por exemplo, você pode querer saber quais funções podem conter um bug que está causando o travamento do kernel.

O exercício final fará com que você liste os argumentos para cada função no backtrace, se a função tiver algum valor de entrada.

No x86-64, os argumentos geralmente são passados ​​em registradores. Se a função a chama a função b, a pode salvar alguns valores de registro na pilha e recarregá-los, dependendo se um valor será reutilizado. Essas decisões são feitas pelo compilador. Assim, para encontrar argumentos na pilha, precisamos de alguma ajuda do compilador.

Quando o JOS é compilado com sinalizadores de depuração, o compilador gera uma variedade de informações de depuração no formato DWARF2. Leia este artigo para uma breve introdução ao DWARF e como funcionam os símbolos de depuração. A principal intuição é que o DWARF fornece ao depurador (ou monitora a função backtrace) informações suficientes para identificar programaticamente os argumentos salvos na pilha.

Para ajudá-lo a implementar essa funcionalidade, fornecemos a função debuginfo_rip(), que procura rip na tabela de símbolos e retorna as informações de depuração para esse endereço. Esta função é definida em kern/kdebug.c. Cada valor rip mapeia para uma estrutura Ripdebuginfo. Dentro dessa estrutura há vários campos úteis, incluindo o arquivo e a linha aos quais o rasgo corresponde, e campos que listam o número de entradas e seus deslocamentos na pilha (Dica: confira rip_fn_narg, offset_fn_arg e size_fn_arg).

> Exercício 11. Modifique sua função de backtrace de pilha para exibir, para cada rasgo, os argumentos da função, o nome da função, o nome do arquivo de origem e o número da linha correspondente a esse rasgo.
> Adicione um comando backtrace ao monitor do kernel e estenda sua implementação de mon_backtrace para chamar debuginfo_rip e imprimir uma linha para cada quadro de pilha do formulário:

```
K> backtrace
Stack backtrace:
  rbp 000000800421af00  rip 00000080042010ff
       kern/monitor.c:86: mon_backtrace+0000000000000035  args:3  0000000000000000 000000000421b909 0000000000000080
  rbp 000000800421afb0  rip 000000800420144d
       kern/monitor.c:163: runcmd+00000000000001d3  args:2  0000000000000001 0000000000000002
  rbp 000000800421afe0  rip 0000008004201508
       kern/monitor.c:185: monitor+000000000000007d  args:1  0000000000000080
  rbp 000000800421aff0  rip 0000008004200196
       kern/init.c:172: i386_init+00000000000000ba  args:0
```
> Cada linha fornece o nome do arquivo e a linha dentro desse arquivo do rip do quadro de pilha, seguido pelo nome da função e o deslocamento do rip da primeira instrução da função (por exemplo, monitor+106 significa que o rip de retorno é de 106 bytes passado do início do monitor), seguido pelo número de argumentos da função e, em seguida, os próprios argumentos reais.
> Dica: para os argumentos da função, dê uma olhada na struct Ripdebuginfo em kern/kdebug.h. Essa estrutura é preenchida pela chamada para debuginfo_rip. A convenção de chamada x86_64 afirma que os argumentos da função são enviados para a pilha. Consulte este artigo sobre a convenção de chamada para descobrir como ler os argumentos reais da função na pilha.
> Certifique-se de imprimir os nomes dos arquivos e funções em uma linha separada, para evitar confundir o script de avaliação.
> Dica: as strings de formato printf fornecem uma maneira fácil, embora obscura, de imprimir strings não terminadas em nulo, como as das tabelas DWARF2. printf("%.*s", comprimento, string) imprime no máximo caracteres de comprimento de string. Dê uma olhada na página man do printf para descobrir por que isso funciona.
> Você pode achar que algumas funções estão faltando no backtrace. Por exemplo, você provavelmente verá uma chamada para monitor(), mas não para runcmd(). Isso ocorre porque o compilador in-line algumas chamadas de função. Outras otimizações podem fazer com que você veja números de linha inesperados. Se você se livrar do -O2 do GNUMakefile, os backtraces podem fazer mais sentido (mas seu kernel rodará mais lentamente).


## Labs a desenvolver


### lab2 Memória virtual
* alocador de página física
* operações na tabela de páginas de 2 níveis
* parte do kernel do diretório da página, UPAGES, pilha do kernel, base do kernel


### Lab3 Ambiente do Usuário
Em JOS, usamos _environment_ para denotar o conceito de _process_<br>
Em JOS, ambientes individuais não têm suas próprias pilhas de kernel, pode haver apenas _one_ ambiente JOS ativo no kernel por vez, portanto, o JOS precisa apenas de uma única pilha de kernel.<br>
* criar ambiente de usuário, carregar binário ELF e executá-lo
* set idt com trap handler, a função trap handler assume o controle após cs:eip obter um novo valor.
* configurar falha de página (agora apenas destrua o ambiente que causou a falha)
* configurar syscall com quatro funções sys
* configurar exceção de ponto de interrupção (acionada por panic() e executar monitor() func)


### lab4 Multitarefa preventiva (preemptiva)
* suporte multiprocessador
 + definir pgdir para E/S mapeada em memória
 + configure o código de entrada MP, pilha de kernel e TSS para diferentes CPUs
 + use um grande bloqueio de kernel que apenas um ambiente pode entrar no modo de kernel
 + implementar o agendamento Round-Robin
 + adicionar chamadas de sistema para criação de ambiente
* Fork Copy-on-Write
   + em _fork()_ o kernel apenas copia o espaço de endereço __mappings__ não seu conteúdo e marca as páginas graváveis ​​e COW como páginas COW.
* Interrupção do relógio e IPC
 + implementar interrupções de clock. No JOS, as interrupções de dispositivos externos são sempre _desativadas_ quando no kernel e ativadas quando no espaço do usuário. controlado pelo bit de flag FL_IF do registrador %eflags.
 + implementar a comunicação entre processos
   + duas funções sys sys_ipc_recv e sys_ipc_try_send. Para receber uma mensagem, um processo chama sys_ipc_recv, o processo atual será desprogramado e não será executado novamente até que uma mensagem seja recebida. Agora qualquer outro processo pode enviar uma mensagem chamando sys_ipc_try_send. um int de 32 bits e um mapeamento de página opcional podem ser transferidos entre dois processos.
   

### lab5 Sistema de arquivos, Spawn e Shell
* Sistema de arquivo
A maioria dos sistemas de arquivos UNIX divide o espaço em disco em _regiões de inode_ e _regiões de dados_. No JOS, não temos o conceito de _inode_ o que significa que também não suportamos links físicos, links simbólicos. O JOS simplesmente armazenará todos os metadados de um arquivo dentro da (única e única) entrada de diretório que descreve esse arquivo.
  + habilitar o acesso ao disco para fs env
  + configure um cache de bloco de 3 GB para fs env em uma forma de _demand paging_.
  + adicionar operações de bloco
  + número do bloco de link com endereço de memória virtual
  + implementar interface do sistema de arquivos

 ```
	  Regular env           FS env
       +---------------+   +---------------+
       |      read     |   |   file_read   |
       |   (lib/fd.c)  |   |   (fs/fs.c)   |
    ...|.......|.......|...|.......^.......|...............
       |       v       |   |       |       | RPC mechanism
       |  devfile_read |   |  serve_read   |
       |  (lib/file.c) |   |  (fs/serv.c)  |
       |       |       |   |       ^       |
       |       v       |   |       |       |
       |     fsipc     |   |     serve     |
       |  (lib/file.c) |   |  (fs/serv.c)  |
       |       |       |   |       ^       |
       |       v       |   |       |       |
       |   ipc_send    |   |   ipc_recv    |
       |       |       |   |       ^       |
       +-------|-------+   +-------|-------+
	       |                   |
	       +-------------------+

```
+ implementar processos de spawn e capacidade de compartilhamento entre fork e spawn

