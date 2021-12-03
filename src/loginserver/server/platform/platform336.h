#ifndef __PLATFORM336_H__
#define __PLATFORM336_H__

#include "platform.h"

class Platform336 : public Platform
{
public:
	Platform336(){}
	virtual ~Platform336(){}

protected:
	virtual bool Check(NetID netid, PlatName pname, unsigned int login_time, LoginStr str, short server);
};


#endif


