

#ifndef PLATFORM_YUJUN_H
#define PLATFORM_YUJUN_H

#include "platform.h"

class PlatformYujun : public Platform
{
public:
	PlatformYujun() {}
	virtual ~PlatformYujun() {}

protected:
	virtual bool Check(NetID netid, PlatName pname, unsigned int login_time, LoginStr str, short server);
};


#endif	// PLATFORM_YUJUN_H


