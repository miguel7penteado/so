/*
   main.cpp


*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "thread.h"
#include "condvar.h"
 
int variavel_global_valor;

class TesteVarCondicional : public Thread
{
	private:
	CondVar &objeto_cond_var;
	Mutex &objeto_exclusao_mutua;

	public:
	TesteVarCondicional(CondVar& cond, Mutex& mutex) : objeto_cond_var(cond), objeto_exclusao_mutua(mutex)
	{
	
	}
	
	void* run()
	{
		// De ao principal a chance de conseguir a exclusão mutua
		// primeiro
		sleep(2);
		printf("thread esperando para obter area critica\n");
		this->objeto_exclusao_mutua.lock();
		printf("thread obteve a area critica\n");		
		// Mantem travado por um tempo para fazer a thread principal esperar
		sleep(5);
		printf("thread configura valor para 1\n");
		variavel_global_valor = 1;
		this->objeto_exclusao_mutua.unlock();
		this->objeto_cond_var.signal();
		return NULL;
	}
 };

int main(int argc, char** argv)
{
    Mutex   exclusao_mutua;
    CondVar variavel_condicional(exclusao_mutua);
    TesteVarCondicional meu_teste(variavel_condicional,exclusao_mutua);
    meu_teste.start();
 
    printf("main() esperando para obter area critica\n");
    exclusao_mutua.lock();
    printf("main() obteve area critica\n");
    // esperando a thread mudar o valor
    while (variavel_global_valor == 0)
    {
        variavel_condicional.wait();
    }
    printf("main() valor detectado configurado para 1\n");
    exclusao_mutua.unlock();
    meu_teste.join();
    
    exit(0);
}

