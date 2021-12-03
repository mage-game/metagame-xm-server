#include "dynamicadjustconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "scene/sceneregister/sceneregister.hpp"
#include "monster/monsterpool.h"

DynamicAdjustConfig::DynamicAdjustConfig()
{
	
}

DynamicAdjustConfig::~DynamicAdjustConfig()
{

}

bool DynamicAdjustConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD

	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg)

	// 刷新调整
	PUGI_XML_LOAD_CONFIG("refresh_adjust", InitRefreshAdjustCfg)

	// 玩家数刷新调整
	PUGI_XML_LOAD_CONFIG("role_num_refresh_adjust", InitServerRoleNumRefreshAdjustCfg)

	// 渠道默认玩家数配置
	PUGI_XML_LOAD_CONFIG("agent_default_num", InitAgentServerRoleNumDefaultCfg)

	return true;
}

bool DynamicAdjustConfig::IsAdjustRefreshTime(const RefreshAdjustKeyObj &obj)
{
	return m_refresh_adjust_cfg_map.find(obj) != m_refresh_adjust_cfg_map.end();
}

int DynamicAdjustConfig::GetRefreshAdjustInterval(const RefreshAdjustKeyObj &obj, int role_num)
{
	auto map_iter = m_refresh_adjust_cfg_map.find(obj);
	if (map_iter == m_refresh_adjust_cfg_map.end())
	{
		return 0;
	}

	auto &vec = map_iter->second;
	for (auto vec_it = vec.rbegin(); vec_it != vec.rend(); ++vec_it)
	{
		if (role_num >= vec_it->scene_role_num)
		{
			return vec_it->refresh_interval_ms;
		}
	}

	return 0;
}

float DynamicAdjustConfig::GetRefreshAdjustTimeRate(int server_role_num)
{
	if (server_role_num <= 0 || m_server_adjust_refresh_vec.empty())
	{
		return 1.0f;
	}

	for (auto vec_it = m_server_adjust_refresh_vec.rbegin(); vec_it != m_server_adjust_refresh_vec.rend(); ++vec_it)
	{
		if (server_role_num >= vec_it->server_role_num)
		{
			return vec_it->refresh_time_rate / 10000.0f;
		}
	}
	
	return m_server_adjust_refresh_vec[0].refresh_time_rate / 10000.0f;
}

const AgentDefaultServerRoleNumCfg * DynamicAdjustConfig::GetAgentDefalutNumCfg(std::string plat_name)
{
	if (m_plat_default_role_num_map.find(plat_name) == m_plat_default_role_num_map.end())
	{
		return nullptr;
	}
	return &m_plat_default_role_num_map[plat_name];
}

int DynamicAdjustConfig::InitOtherCfg(PugiXmlNode root_element)
{
	PugiXmlNode dataElement = root_element.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}


	if (!PugiGetSubNodeValue(dataElement, "default_server_role_num", m_other_cfg.default_server_role_num) || m_other_cfg.default_server_role_num <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "min_server_role_num", m_other_cfg.min_server_role_num) || m_other_cfg.min_server_role_num <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "max_server_role_num", m_other_cfg.max_server_role_num) || m_other_cfg.max_server_role_num <= 0 || m_other_cfg.min_server_role_num >= m_other_cfg.max_server_role_num)
	{
		return -3;
	}

	return 0;
}

int DynamicAdjustConfig::InitRefreshAdjustCfg(PugiXmlNode root_element)
{
	PugiXmlNode dataElement = root_element.child("data");

	while (!dataElement.empty())
	{
		RefreshAdjustKeyObj key;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", key.scene_id) || key.scene_id <= 0 || !SceneRegister::Instance().CheckSceneIsExist(key.scene_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_type", key.refresh_type) ||
			key.refresh_type < RefreshAdjustKeyObj::REFRESH_OBJ_TYPE_GATHER || key.refresh_type >= RefreshAdjustKeyObj::REFRESH_OBJ_TYPE_MAX_COUNT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_obj_id", key.refresh_id) || key.refresh_id <= 0 ||
			(RefreshAdjustKeyObj::REFRESH_OBJ_TYPE_COMMON_MONSTER == key.refresh_type && !MONSTERPOOL->IsMonsterExist(key.refresh_id)))
		{
			return -3;
		}

		RefreshAdjustRoleNumToTimeCfg time_cfg;
		if (!PugiGetSubNodeValue(dataElement, "scene_role_num", time_cfg.scene_role_num) || time_cfg.scene_role_num < 0)
		{
			return -4;
		}

		int refresh_interval_s = 0;
		if (!PugiGetSubNodeValue(dataElement, "refresh_interval", refresh_interval_s) || refresh_interval_s <= 0)
		{
			return -5;
		}
		time_cfg.refresh_interval_ms = refresh_interval_s * 1000;

		auto map_iter = m_refresh_adjust_cfg_map.find(key);
		if (map_iter == m_refresh_adjust_cfg_map.end())
		{
			RefreshAdjustRoleNumToTimeCfgVec time_vec;
			time_vec.push_back(time_cfg);
			m_refresh_adjust_cfg_map[key] = time_vec;
		}
		else
		{
			auto &vec = map_iter->second;

			//保证升序排序
			if (vec.rbegin()->scene_role_num >= time_cfg.scene_role_num)
			{
				return -6;
			}
			else
			{
				vec.push_back(time_cfg);
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int DynamicAdjustConfig::InitServerRoleNumRefreshAdjustCfg(PugiXmlNode root_element)
{
	PugiXmlNode dataElement = root_element.child("data");

	int last_role_num = 0;
	while (!dataElement.empty())
	{
		ServerRoleNumRefreshAdjustCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "server_role_num", cfg.server_role_num) || cfg.server_role_num <= 0)
		{
			return -1;
		}

		if (last_role_num > 0 && last_role_num >= cfg.server_role_num)
		{
			return -2;
		}

		last_role_num = cfg.server_role_num;

		if (!PugiGetSubNodeValue(dataElement, "refresh_time_rate", cfg.refresh_time_rate) || cfg.refresh_time_rate <= 0)
		{
			return -3;
		}

		m_server_adjust_refresh_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int DynamicAdjustConfig::InitAgentServerRoleNumDefaultCfg(PugiXmlNode root_element)
{
	if (root_element.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = root_element.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		std::string spid;
		if (!PugiGetSubNodeValue(dataElement, "spid", spid) || spid.empty())
		{
			return -1;
		}

		if (m_plat_default_role_num_map.find(spid) != m_plat_default_role_num_map.end())
		{
			return -100;
		}

		AgentDefaultServerRoleNumCfg plat_cfg;

		if (!PugiGetSubNodeValue(dataElement, "default_server_role_num", plat_cfg.default_server_role_num) || plat_cfg.default_server_role_num <= 0)
		{
			return -2;
		}

// 		if (!PugiGetSubNodeValue(dataElement, "min_server_role_num", plat_cfg.min_server_role_num) || plat_cfg.min_server_role_num <= 0)
// 		{
// 			return -3;
// 		}
// 
// 		if (!PugiGetSubNodeValue(dataElement, "max_server_role_num", plat_cfg.max_server_role_num) || plat_cfg.max_server_role_num <= 0)
// 		{
// 			return -4;
// 		}

		m_plat_default_role_num_map[spid] = plat_cfg;

		dataElement = dataElement.next_sibling();
	}

// #ifndef _DEBUG
// 	if (!CrossConfig::Instance().IsHiddenServer())
// 	{
// 		PlatName local_plat_name; memset(local_plat_name, 0, sizeof(local_plat_name));
// 		LocalConfig::Instance().GetPlatName(local_plat_name);

// 		auto it = m_plat_default_role_num_map.find(local_plat_name);
// 		if (it == m_plat_default_role_num_map.end())
// 		{
// 			return -101;
// 		}
// 	}
// 
// #endif

	return 0;
}

