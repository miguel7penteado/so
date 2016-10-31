/*
   principal.cpp

   
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "thread.h"
#include "FilaTrabalho.h"
 
class ItemTrabalho
{
	private:
		string minha_mensagem;
		int    meu_numero;
 
	public:
		// construtor
		ItemTrabalho(const char* message, int number) : minha_mensagem(message), meu_numero(number)
		{
			
		}

		// destrutor
		~ItemTrabalho()
		{
			
		}

		// getters and setters
		const char* getMensagem() 
		{
			 return this->minha_mensagem.c_str();
		}

		int getNumero()
		{
			return this->meu_numero;
		}
	
};

class ConsumidorThread : public Thread
{
	private:	
		FilaTrabalho<ItemTrabalho*>& minha_fila;

	public:
		ConsumidorThread(FilaTrabalho<ItemTrabalho*>& parametro_fila) : minha_fila(parametro_fila)
		{

		}

		void* run()
		{
			// Remove 1 item por vez e processa o bichinho. Bloqueia se não 
			// há itens disponíveis para processar.
			for (int meu_contador = 0;; meu_contador++)
			{
				printf("tarefa %lu, laco %d - aguardando por item...\n",(long unsigned int)self(), meu_contador);
				ItemTrabalho* item = minha_fila.remove();
				
				printf("thread %lu, laço %d - peguei um item\n", (long unsigned int)self(), meu_contador);
				printf("tarefa %lu, laço %d - item: mensagem - %s, numero - %d\n", (long unsigned int)self(), meu_contador, item->getMensagem(), item->getNumero());
				delete item;
			}
			return NULL;
		}
};

int main(int quantidade_argumentos, char** vetor_argumentos)
{
	// Processa argumentos de linha de comando
	if ( quantidade_argumentos != 2 )
	{
		printf("uso: %s <interacoes>\n", vetor_argumentos[0]);
		exit(-1);
	}

	int interacoes = atoi(vetor_argumentos[1]);

	// Cria a fila e consome as filas de trabalho.
	FilaTrabalho<ItemTrabalho*>  minha_fila;

	ConsumidorThread* trabalho1 = new ConsumidorThread(minha_fila);
	ConsumidorThread* trabalho2 = new ConsumidorThread(minha_fila);

	trabalho1->start();
	trabalho2->start();

	// Adiciona itens na fila
	ItemTrabalho* item_trabalho;    
	for (int contador = 0; contador < interacoes; contador++)
	{
		item_trabalho = new ItemTrabalho("abc", 123);
		minha_fila.add(item_trabalho);

		item_trabalho = new ItemTrabalho("def", 456);
		minha_fila.add(item_trabalho);

		item_trabalho = new ItemTrabalho("ghi", 789);
		minha_fila.add(item_trabalho);

		sleep(1);
	}

	// Espera a fila estar vazia
	while (minha_fila.size() > 0);
	printf("terminado\n");
	exit(0);
}
