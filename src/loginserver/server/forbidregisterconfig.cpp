#include "forbidregisterconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/serverconfig/configstruct.h"
#include "servercommon/logindef.h"
#include "servercommon/servercommon.h"
#include <fstream>
#include <string.h>
#include "servercommon/serverconfig/localconfig.hpp"
#include "loginserver/engineadapter.h"
#include "loginserver.h"


ForbidRegisterConfig::ForbidRegisterConfig() : m_forbid_register_time_h(0)
{

}

ForbidRegisterConfig::~ForbidRegisterConfig()
{

}

bool ForbidRegisterConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	TiXmlDocument document;
	if (path == "" || !document.LoadFile(path))
	{
		*err = path + "：Load AgentAdapt Error,\n" + document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 渠道列表配置
		TiXmlElement *OtherElement = RootElement->FirstChildElement("agent_adapt");
		if (NULL == OtherElement)
		{
			*err = path + ": xml node error in [config->agent_adapt]";
			return false;
		}

		iRet = this->InitCloseRegRoleCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCloseRegRoleCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ForbidRegisterConfig::InitCloseRegRoleCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	m_forbid_register_time_h = 0;
	int plat_type = LocalConfig::Instance().GetPlatType();
	while (NULL != dataElement)
	{
		PlatName plat_name; memset(plat_name, 0, sizeof(plat_name));
		std::string spid;
		if (!GetSubNodeValue(dataElement, "spid", spid) && spid.length() > 0 && spid.length() < sizeof(plat_name))
		{
			return -1;
		}

		PlatUnion plat_type_union;
		plat_type_union.id = 0;

		SNPRINTF(plat_name, sizeof(plat_name), "%s", spid.c_str());

		if (!spid.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				if ('\0' != plat_name[i])
				{
					plat_type_union.spid[i + 1] = plat_name[i];	// 这样做是为了确保id大于0 字节序的关系
				}
			}
		}

		int close_new_role_h = 0;
		if (!GetSubNodeValue(dataElement, "close_new_role_h", close_new_role_h) || close_new_role_h < 0)
		{
			return -2;
		}

		std::map<int, int>::iterator iter = m_agentadapt_map.find(plat_type_union.id);
		if (m_agentadapt_map.end() == iter)
		{
			m_agentadapt_map[plat_type_union.id] = close_new_role_h;
		}

		if (plat_type == plat_type_union.id)
		{
			m_forbid_register_time_h = close_new_role_h;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

