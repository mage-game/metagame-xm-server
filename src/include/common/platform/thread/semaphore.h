

#ifndef LS_SEMAPHORE_H
#define LS_SEMAPHORE_H



#ifdef __linux__
#include <semaphore.h>
#endif


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


class Semaphore
{
public:

	enum
	{
		Fail = -1,
		Succeed = 0,
		Timeout = 1,
	};
	Semaphore(int initcount);
	~Semaphore();

	int TryDown(unsigned long timeout_millsec);
	void Down();
	void Up();
private:
	Semaphore(const Semaphore&);
	Semaphore& operator=(const Semaphore&);

#ifdef WIN32
	HANDLE m_Semaphore;
#endif

#ifdef __linux__
	sem_t m_Semaphore;
#endif
};


#endif
