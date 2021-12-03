#include <memory.h>
#include <stdlib.h>

#include "upgradeconfigmanager.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "servercommon/struct/upgradeparam.hpp"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"
#include "task/taskpool.h"

#include <string>
#include <algorithm>

UpgradeconfigManager::UpgradeconfigManager()
{
	memset(m_upgrade_config_list, 0, sizeof(m_upgrade_config_list));

	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type++)
	{
		m_upgrade_config_list[upgrade_type] = new UpgradeConfig(this, upgrade_type);
	}
}

UpgradeconfigManager::~UpgradeconfigManager()
{
	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type++)
	{
		if (m_upgrade_config_list[upgrade_type])
		{
			delete m_upgrade_config_list[upgrade_type];
			m_upgrade_config_list[upgrade_type] = nullptr;
		}
	}
}

bool UpgradeconfigManager::Init(const std::string &path, std::string *err)
{
	std::string configname = path + "upgrade_manager.xml";

	PUGI_XML_CONFIG_PRE_LOAD;

	// 进阶管理
	PUGI_XML_LOAD_CONFIG("upgrade_manager", InitUpgradeMgrCfg);

	// 初始化进阶配置
	if (!CheckInitUpgradeCfg(path, err))
	{
		return false;
	}

	return true;
}

bool UpgradeconfigManager::CheckInitUpgradeCfg(const std::string &path, std::string *err)
{
	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type ++)
	{
		std::string configname = path + "upgrade_sys" + std::to_string(upgrade_type) + ".xml";

		if (!m_upgrade_config_list[upgrade_type]->Init(configname, err))
		{
			return false;
		}	
	}

	return true;
}

const UpgradeMgrCfg * UpgradeconfigManager::GetUpgradeMgrCfg(int upgrade_type)
{
	if (upgrade_type < 0 || upgrade_type >= UPGRADE_TYPE_MAX)
	{
		return nullptr;
	}

	auto it = std::find_if(m_upgrade_mgr_vec.begin(), m_upgrade_mgr_vec.end(), [upgrade_type](const UpgradeMgrCfg &cmp_itm) {
		if (upgrade_type == cmp_itm.upgrade_type)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_upgrade_mgr_vec.end())
	{
		return &(*it);
	}

	return nullptr;
}

bool UpgradeconfigManager::IsUpgradeModuleOpen(int upgrade_type, int module_type)
{
	if (upgrade_type < 0 || upgrade_type >= UPGRADE_TYPE_MAX || module_type < 0 || module_type >= UPGRADE_MODULE_TYPE_MAX)
	{
		return false;
	}

	const UpgradeMgrCfg *mgr_cfg = this->GetUpgradeMgrCfg(upgrade_type);
	if (nullptr == mgr_cfg)
	{
		return false;
	}

	bool is_open = (mgr_cfg->module_list[module_type] == 1) ? true : false;

	return is_open;
}

bool UpgradeconfigManager::IsUseUpgradeTemplate(int upgrade_type)
{
	const UpgradeMgrCfg *mgr_cfg = this->GetUpgradeMgrCfg(upgrade_type);
	if (nullptr == mgr_cfg)
	{
		return false;
	}

	bool is_use = (mgr_cfg->is_use_template == 1) ? true : false;

	return is_use;
}

UpgradeConfig * UpgradeconfigManager::GetUpgradeCfg(int upgrade_type)
{
	if (upgrade_type < 0 || upgrade_type >= UPGRADE_TYPE_MAX)
	{
		return nullptr;
	}

	return m_upgrade_config_list[upgrade_type];
}

int UpgradeconfigManager::InitUpgradeMgrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		UpgradeMgrCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "upgrade_type", cfg.upgrade_type) || cfg.upgrade_type < 0 || cfg.upgrade_type >= UPGRADE_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "opengame_day", cfg.opengame_day) || cfg.opengame_day <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "task_id", cfg.task_id))
		{
			return -3;
		}
		if (cfg.task_id > 0)
		{
			if (NULL == TASKPOOL->GetCfgByTaskID(cfg.task_id))
			{
				return -4;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "role_level", cfg.role_level) || cfg.role_level < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_use_template", cfg.is_use_template) || cfg.is_use_template < 0)
		{
			return -6;
		}

		std::string tmp_module;
		for (int i = 0; i < UPGRADE_MODULE_TYPE_MAX; i++)
		{
			tmp_module = "module_" + std::to_string(i);
			if (!PugiGetSubNodeValue(dataElement, tmp_module, cfg.module_list[i]) || cfg.module_list[i] < 0)
			{
				return -10 - i;
			}
		}

		m_upgrade_mgr_vec.emplace_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}


