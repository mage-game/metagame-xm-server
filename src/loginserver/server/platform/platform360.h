#ifndef __PLATFORM360_H__
#define __PLATFORM360_H__

#include "platform.h"

class Platform360 : public Platform
{
public:
	Platform360(){}
	virtual ~Platform360(){}

protected:
	virtual bool Check(NetID netid, PlatName pname, unsigned int login_time, LoginStr str, short server);
};


#endif


