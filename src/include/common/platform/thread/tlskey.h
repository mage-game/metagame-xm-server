
#ifndef TLSKEY_H
#define TLSKEY_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef __linux__
#include <pthread.h>
#endif


class TLSKey
{
public:
	TLSKey();
	~TLSKey();
	
	void *GetKey();
	void SetKey(const void *value);
private:
	#ifdef WIN32
	DWORD m_key;
	#endif

	#ifdef __linux__
	pthread_key_t m_key;
	#endif
};

#endif

