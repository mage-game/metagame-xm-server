#include "sharedconfig.h"

#include "servercommon/configcommon.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SharedConfig *gs_shared_config = NULL;

SharedConfig * SharedConfig::Instance()
{
	if (NULL == gs_shared_config)
	{
		gs_shared_config = new SharedConfig();
	}

	return gs_shared_config;
}

bool SharedConfig::Reload(const std::string& path, std::string* err)
{
	// ���static�ĺ������Ƿ�ֹ����ĺ�������delete�Լ�

	SharedConfig *temp = new SharedConfig();

	if (!temp->Init(path, err))
	{
		delete temp; temp = NULL;
		return false;
	}

	if (NULL != gs_shared_config) delete gs_shared_config;

	gs_shared_config = temp;

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SharedConfig::Init(const std::string &path, std::string *err)
{
	{
		if (!m_open_server_cfg.Init(path + "openserver.xml", err))
		{
			return false;
		}
	}

	return true;
}