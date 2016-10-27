#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*              Que função chama qual
 * 
 ****************************************************************
 | criar_novo_produtor -> fornecedor -> produzir_novo_produto  |
 ****************************************************************
 
 ****************************************************************
 | chamat_novo_consumidor -> cliente ->  consumir_produto       |
 ****************************************************************

*/

//define os lados de leitura e escrita do vetor pipe...
#define LEITURA 0
#define ESCRITA 1

//define a quantidade de produtos produzidos pelos produtores 
//e a quantidade de pacotes que um pipe suporta
#define QUANTIDADE_MAX_PRODUTOS  100
#define LIMITE_CARGA_PIPE  30

//define a quantidade de produtores e de consumidores
#define N_PRODUTORES   2
#define N_CONSUMIDORES   2


//definimos uma estrutura para os produtos a serem produzidos,
// armazenando o id do pai
typedef struct prod prod;

struct prod
{
	int numero_serie_produto;
	pid_t cnpj_produtor;
};

int memoria_compartilhada_pipe[2];
int n;

//Prototipos...
void consumir_produto(prod*);
prod produzir_produto(int);
void fornecedor();
void cliente();
void criar_novo_consumidor();
void criar_novo_produtor();


void fornecedor()
{
	int codigo_do_item;
	prod novo_produto;

	// Fecha o lado de leitura. Lado nao utilizado
	close(memoria_compartilhada_pipe[LEITURA]);

	for(codigo_do_item = 0; codigo_do_item < QUANTIDADE_MAX_PRODUTOS; codigo_do_item++)
	{
		//produz novo item
		novo_produto = produzir_produto(codigo_do_item);

		printf("Novo item produzido pelo processo %d! \n", getpid());

		 // insere novo item no pipe
		write(memoria_compartilhada_pipe[ESCRITA],&novo_produto,sizeof(prod));    
	}

	 // Fecha o lado utilizado
	close(memoria_compartilhada_pipe[ESCRITA]);
}

void cliente()
{
	int quantidade;
	int quantidade_itens;
	prod consumeprod;
	/* Fecha o lado de leitura que nao eh utilizado*/
	close (memoria_compartilhada_pipe[ESCRITA]); 

	while( 1 )
	{

	   quantidade_itens = read(memoria_compartilhada_pipe[LEITURA],&consumeprod,sizeof(prod));
	   if(quantidade_itens==-1)
	   {        
		   printf("Erro na leitura do pipe\n");
	   }
	   else
	   {
		   if(quantidade_itens==0)
		   {
			   printf("Pipe estah vazio... \n");
		   }
		   else
		   {

			   consumir_produto(&consumeprod);
		   }
	   }
	}
	/* Fecha o lado utilizado*/
	close (memoria_compartilhada_pipe[LEITURA]);

}

prod produzir_produto(int numero_serie)
{
	prod novo_produto;
	int tempo_de_producao;

	tempo_de_producao = rand()%7;
	novo_produto.numero_serie_produto = numero_serie;

	novo_produto.cnpj_produtor = getpid();
	printf("Processo %d produzindo um novo produto\n",getpid());

	sleep(tempo_de_producao);
	
	return novo_produto;
}

void consumir_produto(prod * t)
{

	int tempo_de_consumo;
	tempo_de_consumo=rand()%5;
	printf("Processo %d consumindo produto %d do Produtor %d\n",getpid(),t->numero_serie_produto,t->cnpj_produtor);

	sleep(tempo_de_consumo);
}

void criar_novo_produtor()
{
	int numero_novo_processo;
	
	// lembre-se que, depois do fork, embora as variáveis
	// internas de pai e filho sejam distintas
	// os descritores de arquivo de pai e filho
	// continuam sendo os mesmos (ou apontam para o mesmo lugar)
	// "implementando" um semáfaro...
	
	
	numero_novo_processo = fork(); // mágica !
	
	if( numero_novo_processo == -1 )
	{
	   printf("Erro na criacao do processo produtor\n");
	}
	else
	{
		if( numero_novo_processo == 0)
		{
		   fornecedor();
		   exit(0);
		}
	}

	return;
}

void criar_novo_consumidor()
{
	int numero_novo_processo;
	
	// lembre-se que, depois do fork, embora as variáveis
	// internas de pai e filho sejam distintas
	// os descritores de arquivo de pai e filho
	// continuam sendo os mesmos (ou apontam para o mesmo lugar)
	// "implementando" um semáfaro...

	numero_novo_processo = fork(); // mágica

	if( numero_novo_processo == -1 )
	{
	   printf("Erro na criacao do processo consumidor\n");
	}
	else
	{

	   cliente();
	   exit(0);
	}

}

int main()
{

	//cria o pipe
	pipe(memoria_compartilhada_pipe);

	int quantidade_de_produtores;
	int quantidade_de_consumidores;
	
	quantidade_de_produtores=quantidade_de_consumidores=0;

	// cria processos produtores
	for( quantidade_de_produtores = 0; quantidade_de_produtores < N_PRODUTORES; quantidade_de_produtores++)
	{

	   criar_novo_produtor(); 
	}
	
	// cria processos consumidores
	for(quantidade_de_consumidores = 0; quantidade_de_consumidores < N_CONSUMIDORES; quantidade_de_consumidores++)
	{

	   criar_novo_consumidor();
	}
	
	//o processo pai será um processo consumidor
	  
	exit(0);
}
