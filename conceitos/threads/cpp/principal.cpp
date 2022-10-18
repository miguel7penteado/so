/*

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread.h"

class MinhaThread : public Thread
{
    public:
    
    // polimorfizando o metodo run
    void *run()
    {
		int contador=0;
		
        for (contador = 0; contador < 5; contador++)
        {
            printf("trabalho %lu rodando - %d\n",  (long unsigned int)self(), contador+1);
            sleep(2);
        }
        printf("trabalho finalizado %lu\n", (long unsigned int)self());
        return NULL;
    }
};

int main(int argc, char** argv)
{
    MinhaThread* forca_trabalho1 = new MinhaThread();
    MinhaThread* forca_trabalho2 = new MinhaThread();
    forca_trabalho1->start();
    forca_trabalho2->start();
    forca_trabalho1->join();
    forca_trabalho2->join();
    printf("Programa principal finalizado\n");
    exit(0);
}
