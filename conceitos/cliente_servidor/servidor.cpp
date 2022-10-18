/*
   servidor.cpp

*/

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include "thread.h"
#include "FilaTrabalho.h"
#include "tcpacceptor.h"
 
class ItemProcessado
{
	private:
		TCPStream* meu_segmento;

	public:
		
		// construtor
		ItemProcessado(TCPStream* parametro_segmento) : meu_segmento(parametro_segmento)
		{
			
		}
		
		// destrutor
		~ItemProcessado()
		{
			delete meu_segmento;
		}
		
		// getters and setters
		TCPStream* getSegmento()
		{
			return meu_segmento;
		}
		
};

class TarefaConsumidora : public Thread
{
	private:
	FilaTrabalho<ItemProcessado*>& fila_trabalho_interna;

	public:
	TarefaConsumidora(FilaTrabalho<ItemProcessado*>& parametro_fila) : fila_trabalho_interna(parametro_fila)
	{
		
	}

	void* run()
	{
		char entrada[256];
		int tamanho;
		TCPStream* stream ;
		ItemProcessado* item ;

		// Remove 1 item por vez e o processa. Bloqueia se n�o h� itens 
		// para processar.
		for (int contador1 = 0;; contador1++)
		{
			printf("Tarefa %lu, la�o %d - esperando por item...\n", (long unsigned int)self(), contador1);
			item = fila_trabalho_interna.remove();
			
			printf("Tarefa %lu, la�o %d - peguei um item\n", (long unsigned int)self(), contador1);
			stream = item->getSegmento();

			// Propaga mensagens de volta para o cliente at� que se  
			// a conex�o esteja fechada.

			memset(entrada, '\0', sizeof(entrada) );
			tamanho=0;
			
			while ((tamanho = stream->receive(entrada, sizeof(entrada)-1)) > 0 )
			{
				entrada[tamanho] = '\0';
				stream->send(entrada, tamanho);
				printf("tarefa %lu, ecoou '%s' de volta para o cliente\n", (long unsigned int)self(), entrada);
			}
			delete item; 
		}

		// Nunca deve chegar at� aqui.
		return NULL;
	}
};

int main(int quantidade_argumentos, char** vetor_argumentos)
{
	// main - tarefa produtora

    int trabalhos=0;
    int porta=0;
    string ip;
	FilaTrabalho<ItemProcessado*>  fila_de_trabalho;
	TarefaConsumidora* tarefas_de_consumo;
	TCPStream* minha_conexao_ativa;
    ItemProcessado* meu_item_recebido;
    TCPAcceptor* conector_esperando_cliente;

    // Processa a linha de comando
    if ( quantidade_argumentos < 3 || quantidade_argumentos > 4 )
    {
        printf("uso: %s <trabalhos> <porta> <ip>\n", vetor_argumentos[0]);
        exit(-1);
    }
    
    trabalhos = atoi(vetor_argumentos[1]);
    porta = atoi(vetor_argumentos[2]);

    if (quantidade_argumentos == 4)
    { 
        ip = vetor_argumentos[3];
    }
    
    // V�rias consumidoras ser�o enfileiradas
    for (int contador_conexoes = 0; contador_conexoes < trabalhos; contador_conexoes++)
    {
        tarefas_de_consumo = new TarefaConsumidora(fila_de_trabalho);
        if (!tarefas_de_consumo)
        {
            printf("N�o pude criar um Gestor de Conexoes %d\n", contador_conexoes);
            exit(1);
        } 
        tarefas_de_consumo->start();
    }
 
    // Come�ar a ouvir as conexoes no servidor
    if (ip.length() > 0)
    {
        conector_esperando_cliente = new TCPAcceptor(porta, (char*)ip.c_str());
    }
    else
    {
        conector_esperando_cliente = new TCPAcceptor(porta);        
    }                                        
    
    if (!conector_esperando_cliente || conector_esperando_cliente->start() != 0)
    {
        printf("N�o pude criar um conector\n");
        exit(1);
    }

    // adicionar um meu_item_recebido de trabalho na fila_de_trabalho 
    // para cada conexao...
    while (1)
    {
        minha_conexao_ativa = conector_esperando_cliente->accept();
        if (!minha_conexao_ativa)
        {
            printf("N�o pude aceitar a conexao\n");
            continue;
        }
        
        meu_item_recebido = new ItemProcessado(minha_conexao_ativa);
        if (!meu_item_recebido)
        {
            printf("Cliente n�o mandou nada\n");
            continue;
        }
        fila_de_trabalho.add(meu_item_recebido);
    }
 
    // Nunca deve chegar aqui
    exit(0);
}
