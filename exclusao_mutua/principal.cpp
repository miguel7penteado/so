/*
   main.cpp


*/

#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "thread.h"
#include "mutex.h"
 
class MutexTest : public Thread
{
	private:
    Mutex &atributo_exclusao_mutua;
    
    public:
    MutexTest(Mutex& parametro_exclusao_mutua) : atributo_exclusao_mutua(parametro_exclusao_mutua)
    {		
	}
	
    void* run()
    {
        printf("thread esperando para obter acesso a mutex\n");
        atributo_exclusao_mutua.lock();
        printf("thread obteve acesso ao mutex\n");
        // fecha por um tempo para fazer a tarefa principal esperar
        sleep(10);
        atributo_exclusao_mutua.unlock();
        return NULL;
    }
};

int main(int argc, char** argv)
{
    Mutex minha_exclusao_mutua;
    MutexTest meu_teste(minha_exclusao_mutua);
    meu_teste.start();

    // Dá à tarefa a chance de obter acesso a minha_exclusao_mutua primeiro
    sleep(1);

    // Agora é a vez da tarefa principal 
    printf("Tarefa principal esperando para obter minha_exclusao_mutua\n");
    minha_exclusao_mutua.lock();

    printf("Tarefa principal obteve acesso a minha_exclusao_mutua \n");
    minha_exclusao_mutua.unlock();

    meu_teste.join();
    
    exit(0);
}

