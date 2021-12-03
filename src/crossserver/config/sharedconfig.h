#ifndef __SHAREDCONFIG_H__
#define __SHAREDCONFIG_H__

#include "servercommon/serverconfig/openserverconfig.hpp"

#include <string>

#define SHAREDCONFIG SharedConfig::Instance()

class SharedConfig
{
public:
	static SharedConfig * Instance();

	bool Init(const std::string &path, std::string *err);
	static bool Reload(const std::string &path, std::string *err);

	OpenServerConfig & GetOpenServerConfig() { return m_open_server_cfg; }
public:
	SharedConfig() {}
	~SharedConfig() {}

	OpenServerConfig m_open_server_cfg;
};

#endif

