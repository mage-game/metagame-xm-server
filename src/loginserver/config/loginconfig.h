#ifndef __LOGINCONFIG_H__
#define __LOGINCONFIG_H__

#include "servercommon/serverconfig/openserverconfig.hpp"
#include "server/forbidregisterconfig.hpp"

#define LOGIN_CONFIG LoginConfig::Instance()

class LoginConfig
{
public:
	static LoginConfig * Instance();

	bool Init(const std::string& path, std::string *err);
	bool Reload(std::string* err);

	OpenServerConfig * GetOpenServerConfig() { return &m_open_server_config; }

	bool IsForbidRegister();

private:
	LoginConfig() {}
	~LoginConfig() {}

	std::string m_configpath;

	OpenServerConfig m_open_server_config;
	ForbidRegisterConfig m_forbid_register_config;
};

#endif
