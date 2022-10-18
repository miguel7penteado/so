/*
   FilaTrabalho.h


*/

#ifndef __FilaTrabalho_h__
#define __FilaTrabalho_h__

#include <pthread.h>
#include <list>

using namespace std;

template <typename TEMPLATE> class FilaTrabalho
{
	private:
		list<TEMPLATE>	minha_fila;
		pthread_mutex_t	exclusao_mutua;
		pthread_cond_t	condicao; 

	public:
		FilaTrabalho()
		{
			pthread_mutex_init(&exclusao_mutua, NULL);
			pthread_cond_init(&condicao, NULL);
		}

		~FilaTrabalho()
		{
			pthread_mutex_destroy(&exclusao_mutua);
			pthread_cond_destroy(&condicao);
		}

		void add(TEMPLATE item)
		{
			pthread_mutex_lock(&exclusao_mutua);
			minha_fila.push_back(item);
			pthread_cond_signal(&condicao);
			pthread_mutex_unlock(&exclusao_mutua);
		}

		TEMPLATE remove()
		{
			pthread_mutex_lock(&exclusao_mutua);
			while (minha_fila.size() == 0)
			{
				pthread_cond_wait(&condicao, &exclusao_mutua);
			}
			TEMPLATE item = minha_fila.front();
			minha_fila.pop_front();
			pthread_mutex_unlock(&exclusao_mutua);
			return item;
		}

		int size()
		{
			pthread_mutex_lock(&exclusao_mutua);
			int size = minha_fila.size();
			pthread_mutex_unlock(&exclusao_mutua);
			return size;
		}
};

#endif
