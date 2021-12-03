
#ifndef SEMAPHORE_H
#define SEMAPHORE_H


/*
  need -lrt in linux
*/

#ifdef __linux__
#include <pthread.h>
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

class Event
{
 public:
	enum
	{	
		Fail = -1,
		Succeed = 0,
		Timeout = 1,
	};
	
	Event();
	~Event();

	bool Wait();
	//return 0 if wait complete, return 1 if timeout, return -1 if error
	int Wait(unsigned long millsecond);
	bool Signal();
private:
	Event(const Event&);
	Event& operator=(const Event&);
	
#ifdef __linux__
	pthread_cond_t m_Cond;
	pthread_mutex_t m_Mutex;
#endif

#ifdef WIN32
	HANDLE m_Event;
#endif
};


#endif /* SEMAPHORE_H */
