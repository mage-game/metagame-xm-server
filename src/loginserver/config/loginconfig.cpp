#include "loginconfig.h"

#include "servercommon/configcommon.h"
#include "engineadapter.h"
#include "utility/configpath.h"
#include "servercommon/servercommon.h"
#include "servercommon/serverconfig/localconfig.hpp"

LoginConfig *gs_login_config = NULL;

LoginConfig * LoginConfig::Instance()
{
	if (NULL == gs_login_config) 
	{
		gs_login_config = new LoginConfig();
	}

	return gs_login_config;
}

bool LoginConfig::Reload(std::string* err)
{
	if (m_configpath.empty())
	{
		return false;
	}

	LoginConfig *temp = new LoginConfig();
	if (!temp->Init(m_configpath, err))
	{
		delete temp; temp = NULL;
		return false;
	}

	if (NULL != gs_login_config) delete gs_login_config;

	gs_login_config = temp;

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool LoginConfig::Init(const std::string& path, std::string *err)
{
	m_configpath = path;

	if (!m_open_server_config.Init(m_configpath + "openserver.xml", err))
	{
		return false;
	}

	if (!m_forbid_register_config.Init(m_configpath + "loginserver/agent_adapt.xml", err))
	{
		return false;
	}

	return true;
}

bool LoginConfig::IsForbidRegister()
{
	unsigned int real_openserver_time = (unsigned int)m_open_server_config.GetServerRealStartTime();
	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();

	int forbid_register_hour = m_forbid_register_config.GetForbidRegisterHour();
	if (forbid_register_hour <= 0)
	{
		return false;
	}
	if (LocalConfig::Instance().IsShenheServer())
	{
		return false;
	}

	return now_second >= (real_openserver_time + forbid_register_hour * SECOND_PER_HOUR);
}
