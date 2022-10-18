/*
   thread.h
	
	Trabalho original de http://vichargrave.com/java-style-thread-class-in-c/
	* Imitando a classe Thread existente em Java.
	* Arquivo de definição da classe.

*/

#ifndef __thread_h__
#define __thread_h__

#include <pthread.h>

class Thread
{
	private:
		pthread_t  m_tid;
		int        m_running;
		int        m_detached;

	public:
		Thread();
		virtual ~Thread();

		int start();
		int join();
		int detach();
		pthread_t self();

		virtual void* run() = 0;

};

#endif
