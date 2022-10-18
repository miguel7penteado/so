/*
   mutex.h

*/

#ifndef __condvar_h__
#define __condvar_h__

#include "mutex.h"

class CondVar
{
	private:
	pthread_cond_t  m_cond;
	Mutex&          m_lock;
	CondVar();


	public:
	// just initialize to defaults
	CondVar(Mutex& mutex): m_lock(mutex)
	{
		pthread_cond_init(&m_cond, NULL);
	}
	virtual ~CondVar()
	{
		pthread_cond_destroy(&m_cond);
	}

	int wait()
	{
		return  pthread_cond_wait(&m_cond, &(m_lock.m_mutex));
	}
	
	int signal()
	{
		return pthread_cond_signal(&m_cond);
	} 
	
	int broadcast()
	{
		return pthread_cond_broadcast(&m_cond);
	} 

};

#endif
