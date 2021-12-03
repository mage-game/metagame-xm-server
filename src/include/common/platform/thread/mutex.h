
#ifndef MUTEX_H
#define MUTEX_H


#ifdef __linux__
#include <pthread.h>
#endif 

#ifdef WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400 
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


class Mutex
{
 public:
	Mutex();
	~Mutex();

	bool Lock();
	bool TryLock();
	bool Unlock();
 private:
	 Mutex(const Mutex&);
	 Mutex& operator=(const Mutex&);

#ifdef __linux__
	pthread_mutex_t m_Lock;
#endif

#ifdef WIN32
	CRITICAL_SECTION m_CriticalSection;
#endif
};




#endif /* MUTEX_H */
