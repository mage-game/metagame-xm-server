#ifndef _UPGRADE_MGR_CONFIG_HPP
#define _UPGRADE_MGR_CONFIG_HPP

#include "gamedef.h"
#include "servercommon/basedef.h"
#include "servercommon/struct/upgradeparam.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "upgradeconfig.hpp"

#include "obj/character/attribute.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <map>
#include <vector>

#define UPGRADE_CFG_MGR(upgrade_type) LOGIC_CONFIG->GetUpgradeCfgManager().GetUpgradeCfg(upgrade_type)

struct UpgradeMgrCfg
{
	UpgradeMgrCfg(): upgrade_type(0), opengame_day(0), task_id(0), role_level(0), is_use_template(0), module_count(0)
	{
		memset(module_list, 0, sizeof(module_list));
	}

	int upgrade_type;
	int opengame_day;
	TaskID task_id;
	int role_level;
	int is_use_template;
	
	int module_count;
	int module_list[UPGRADE_MODULE_TYPE_MAX];
};


class UpgradeconfigManager : public ConfigBase
{
public:

	UpgradeconfigManager();
	~UpgradeconfigManager();

	bool Init(const std::string &path, std::string *err);
	bool CheckInitUpgradeCfg(const std::string &path, std::string *err);

	const UpgradeMgrCfg * GetUpgradeMgrCfg(int upgrade_type);
	bool IsUpgradeModuleOpen(int upgrade_type, int module_type);
	bool IsUseUpgradeTemplate(int upgrade_type);

	int GetUpgradeTypeCount() { return static_cast<int>(m_upgrade_mgr_vec.size()); }

	UpgradeConfig * GetUpgradeCfg(int upgrade_type);

private:

	// -----------------------------------------------------------------------------
	int InitUpgradeMgrCfg(PugiXmlNode RootElement);         

	std::vector<UpgradeMgrCfg> m_upgrade_mgr_vec;

	UpgradeConfig *m_upgrade_config_list[UPGRADE_TYPE_MAX];  // ½ø½×ÅäÖÃÁÐ±í
};

#endif




