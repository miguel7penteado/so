/*
   mutex.h

*/

#ifndef __mutex_h__
#define __mutex_h__

#include <pthread.h>

class Mutex
{
    friend class CondVar;
    pthread_mutex_t  m_mutex;

  public:
    // Inicializaçao padrão
    Mutex()
    {
		pthread_mutex_init(&m_mutex, NULL);
	}
    
    virtual ~Mutex()
    {
		pthread_mutex_destroy(&m_mutex);
	}

    int lock()
    {
		return  pthread_mutex_lock(&m_mutex);
	}
    
    int trylock()
    {
		return  pthread_mutex_trylock(&m_mutex);
	}
    
    int unlock()
    {
		return  pthread_mutex_unlock(&m_mutex);		
	}	
};

#endif
