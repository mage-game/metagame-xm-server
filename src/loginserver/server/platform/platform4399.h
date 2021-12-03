

#ifndef PLATFORM4399_H
#define PLATFORM4399_H

#include "platform.h"

class Platform4399 : public Platform
{
public:
	Platform4399(){}
	virtual ~Platform4399(){}

protected:
	virtual bool Check(NetID netid, PlatName pname, unsigned int login_time, LoginStr str, short server);
};


#endif


