



#ifndef THREAD_H
#define THREAD_H


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
typedef DWORD TID;
#endif


#ifdef __linux__
#include <pthread.h>

typedef void *DWORD;
typedef pthread_t TID;
#endif


class Thread
{
public:
	typedef DWORD (*Func)(void *);
private:
	struct ThreadParam
	{
		Func func;
		void *param;
	};
public:


	Thread();
	~Thread();

	bool Run(Func func, void *param, unsigned int stack_size=0);

	bool Join();
	bool Terminate();

	void Detach();

	TID GetThreadID()const;

	static TID GetCurrentThreadID();

private:
#ifdef WIN32
	static DWORD CALLBACK ThreadProc(void *param);

	HANDLE m_Thread;
	DWORD m_ID;
#endif

#ifdef __linux__
	static DWORD ThreadProc(void *param);

	pthread_t m_PID;
#endif
};


#endif
