

#ifndef PLATFORM91WAN_H
#define PLATFORM91WAN_H

#include "platform.h"

class Platform91wan : public Platform
{
public:
	Platform91wan(){}
	virtual ~Platform91wan(){}

protected:
	virtual bool Check(NetID netid, PlatName pname, unsigned int login_time, LoginStr str, short server);
};


#endif


