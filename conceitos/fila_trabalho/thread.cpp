/*
   thread.cpp

	Trabalho original de http://vichargrave.com/java-style-thread-class-in-c/
	* Implementação dos métodos da classe Thread existente em Java.
	* Arquivo de implementação dos métodos da classe.
*/

#include "thread.h"

static void* runThread(void* arg)
{
    return ((Thread*)arg)->run();
}

Thread::Thread() : m_tid(0), m_running(0), m_detached(0)
{}

Thread::~Thread()
{
    if (this->m_running == 1 && this->m_detached == 0)
    {
        pthread_detach(this->m_tid);
    }
    if (this->m_running == 1)
    {
        pthread_cancel(this->m_tid);
    }
}

int Thread::start()
{
    int resultado_ativacao = 0;
     
    resultado_ativacao = pthread_create(&this->m_tid, NULL, runThread, this);
    
    if (resultado_ativacao == 0)
    {
        this->m_running = 1;
    }
    return resultado_ativacao;
}

int Thread::join()
{
    int resultado_juncao = -1;
    if (this->m_running == 1)
    {
        resultado_juncao = pthread_join(this->m_tid, NULL);
        if (resultado_juncao == 0)
        {
            this->m_detached = 0;
        }
    }
    return resultado_juncao;
}

int Thread::detach()
{
    int resultado_descolamento = -1;
    if (this->m_running == 1 && this->m_detached == 0)
    {
        resultado_descolamento = pthread_detach(m_tid);
        if (resultado_descolamento == 0)
        {
            this->m_detached = 1;
        }
    }
    return resultado_descolamento;
}

pthread_t Thread::self()
{
    return this->m_tid;
}
