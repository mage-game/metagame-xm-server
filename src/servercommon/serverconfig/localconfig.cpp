#include "localconfig.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/serverconfig/serverconfigpool.h"

LocalConfig::LocalConfig()
	:  m_rand_act_zhuanfu_type(0), m_plat_type(0), m_is_check_server_id(true), m_is_ios_platform(false), m_is_dev_platform(false)
{
	memset(m_plat_name, 0, sizeof(m_plat_name));
	memset(m_rand_cfg_name, 0, sizeof(m_rand_cfg_name));
}

LocalConfig::~LocalConfig()
{
}

LocalConfig & LocalConfig::Instance()
{
	static LocalConfig clc;
	return clc;
}

void LocalConfig::GetPlatName(int plat_type, PlatName plat_name)
{
	if (plat_type >= CommonConfig::PLAT_TYPE_YOUKE && plat_type < CommonConfig::PLAT_TYPE_COUNT)
	{
		UNSTD_STATIC_CHECK(CommonConfig::PLAT_TYPE_COUNT == 6);
		const static PlatName plat_list[CommonConfig::PLAT_TYPE_COUNT] = 
		{
			"youke",
			"360",
			"37w",
			"yyg",
			"gts",
			"yyb"
		};

		F_STRNCPY(plat_name, plat_list[plat_type], sizeof(PlatName));
	}
	else
	{
		PlatTypeUnion plat_type_union;

		plat_type_union.id = plat_type;

		SNPRINTF(plat_name, sizeof(PlatName), "%c%c%c", plat_type_union.spid[1], plat_type_union.spid[2], plat_type_union.spid[3]);
	}
}

bool LocalConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "LocalConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	std::string plat_name;
	if (!PugiGetSubNodeValue(RootElement, "plat_name", plat_name) && plat_name.length() > 0 && plat_name.length() < sizeof(m_plat_name))
	{
		*err = path + ": xml node error in [config->plat_name]";
		return false;
	}

	SNPRINTF(m_plat_name, sizeof(m_plat_name), "%s", plat_name.c_str());

	if ('i' == m_plat_name[0])
	{
		m_is_ios_platform = true;
	}
	if (0 == plat_name.compare("dev"))
	{
		m_is_dev_platform = true;
	}

	if (0 == plat_name.compare("360"))
	{
		m_plat_type = CommonConfig::PLAT_TYPE_360;
	}
	else if (0 == plat_name.compare("37w"))
	{
		m_plat_type = CommonConfig::PLAT_TYPE_37W;
	}
	else if (0 == plat_name.compare("yyg"))
	{
		m_plat_type = CommonConfig::PLAT_TYPE_YYG;
	}
	else if (0 == plat_name.compare("gts"))
	{
		m_plat_type = CommonConfig::PLAT_TYPE_GTS;
	}
	else if (0 == plat_name.compare("yyb"))
	{
		m_plat_type = CommonConfig::PLAT_TYPE_YYB;
	}
	else if (!plat_name.empty())
	{
		PlatTypeUnion plat_type_union;

		plat_type_union.id = 0;

		for (int i = 0; i < 3; i++)
		{
			if ('\0' != m_plat_name[i])
			{
				plat_type_union.spid[i + 1] = m_plat_name[i];	// 这样做是为了确保id大于0 字节序的关系
			}
		}

		m_plat_type = plat_type_union.id;
	}
	else
	{
		m_plat_type = CommonConfig::PLAT_TYPE_YOUKE;
	}

	if (!PugiGetSubNodeValue(RootElement, "rand_act_zhuanfu_type", m_rand_act_zhuanfu_type) || m_rand_act_zhuanfu_type <= 0)
	{
		*err = path + ": xml node error in [config->rand_act_zhuanfu_type]";
		return false;
	}

	SNPRINTF(m_rand_cfg_name, sizeof(m_rand_cfg_name), "randactivityconfig_%d", GetRandActZhuanfuType());

	if (!PugiGetSubNodeValue(RootElement, "is_check_server_id", m_is_check_server_id))
	{
		*err = path + ": xml node error in [config->is_check_server_id]";
		return false;
	}

	{
		PugiXmlNode ServerListElement = RootElement.child("allow_server_id_list");
		if (ServerListElement.empty())
		{
			*err = path + ": xml node error in [config->allow_server_id_list]";
			return false;
		}

		int count = 0;
		PugiXmlNode ServerIdElement = ServerListElement.child("server_id");
		while (!ServerIdElement.empty())
		{
			if (count >= MAX_ALLOW_SERVER_ID_COUNT)
			{
				*err = path + ": xml node error in [config->allow_server_id_list->server_id], too many";
				return false;
			}

			int server_id = 0;
			if (!PugiGetNodeValue(ServerIdElement, server_id) || server_id < 0 || server_id >= MAX_DB_IDNEX)
			{
				*err = path + ": xml node error in [config->allow_server_id_list->server_id]";
				return false;
			}

			if (m_allow_server_id_set.find(server_id) != m_allow_server_id_set.end())
			{
				*err = path + ": xml node error in [config->allow_server_id_list->server_id], repeat";
				return false;
			}

			m_allow_server_id_set.insert(server_id);

			++ count;
			ServerIdElement = ServerIdElement.next_sibling("server_id");
		}

		if (m_is_check_server_id && count <= 0)
		{
			*err = path + ": xml node error in [config->allow_server_id_list->server_id], must one server_id at least";
			return false;
		}
	}

	return true;
}

void LocalConfig::GetPlatName(PlatName plat_name)
{
	F_STRNCPY(plat_name, m_plat_name, sizeof(PlatName));
}

bool LocalConfig::IsServerIdAllowed(int server_id)
{
	return (!m_is_check_server_id || (m_allow_server_id_set.find(server_id) != m_allow_server_id_set.end()));
}

bool LocalConfig::IsMergedServer()
{
	return m_allow_server_id_set.size() > 1;
}

int LocalConfig::GetMergeServerId()
{
	if (m_allow_server_id_set.empty())
	{
		return 0;
	}

	std::set<int>::iterator it = m_allow_server_id_set.begin();

	return (*it);
}

bool LocalConfig::IsShenheServer()
{
        return (m_allow_server_id_set.find(1501) != m_allow_server_id_set.end());
}


