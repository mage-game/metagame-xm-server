
#ifndef PLATFORM_H
#define PLATFORM_H

#include "inetworkmodule.h"
#include "servercommon/logindef.h"

class Platform
{
public:
	Platform() {}
	virtual ~Platform(){}

	static Platform * Create();
	bool CheckAccount(NetID netid, PlatName pname, unsigned int login_time, LoginStr str, short server);

	virtual void CreateLoginStr(LoginStr login_str, PlatName pname, unsigned int login_time, char cm, short server);

protected:
	virtual bool Check(NetID netid, PlatName pname, unsigned int login_time, LoginStr str, short server);
};

#endif


