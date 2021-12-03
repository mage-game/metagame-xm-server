#include "xiannvconfig.hpp"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "gameworld/checkresourcecenter.hpp"
#include "skill/skillpool.hpp"

XiannvConfig::XiannvConfig(): m_shengwu_max_id(0), m_shengwu_chou_comsume_count(0), m_chou_ling_consume_cfg_count(0), m_is_open_special_cap_add(false)
{
}

XiannvConfig::~XiannvConfig()
{

}

bool XiannvConfig::Init(std::string configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// ∆‰À˚≈‰÷√
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);

	// œ…≈Æª˘¥°≈‰÷√
	PUGI_XML_LOAD_CONFIG("xiannv", InitBaseCfg);
	// …˝º∂≈‰÷√
	PUGI_XML_LOAD_CONFIG("level_attr", InitLevelCfg);
	// ◊ ÷ ≈‰÷√
	PUGI_XML_LOAD_CONFIG("zizhi_attr", InitZiZhiCfg);
	// ª√ªØ≈‰÷√
	PUGI_XML_LOAD_CONFIG("huanhua", InitHuanHuaCfg);
	// ª√ªØ…˝º∂≈‰÷√
	PUGI_XML_LOAD_CONFIG("huanhua_level_attr", InitHuanHuaLevelCfg);

	// ººƒ‹≈‰÷√
	PUGI_XML_LOAD_CONFIG("skill_cfg", InitSkillCfg);
	// Ω¯Ω◊≈‰÷√
	PUGI_XML_LOAD_CONFIG("shengwu_upgrade_cfg", InitUpGradeCfg);
	//  •ŒÔ≈‰÷√
	PUGI_XML_LOAD_CONFIG("shengwu_cfg", InitShengwuInfoCfg);
	//  •ŒÔœ˚∫ƒ≈‰÷√
	PUGI_XML_LOAD_CONFIG("shengwu_chou_comsume", InitShengwuChouConsumeCfg);
	// æ≠—È≈‰÷√
	PUGI_XML_LOAD_CONFIG("exp_cfg", InitExpCfg);
	// π≤√˘≈‰÷√
	PUGI_XML_LOAD_CONFIG("gongming_cfg", InitGongmingCfg);
	// “¿¿µ≈‰÷√
	PUGI_XML_LOAD_CONFIG("grid_depend_cfg", InitGridDependCfg);
	// √Ÿ¡È≈‰÷√
	PUGI_XML_LOAD_CONFIG("chou_ling_cfg", InitChouLingCfg);
	// √Ÿ¡Èœ˚∫ƒ≈‰÷√
	PUGI_XML_LOAD_CONFIG("chou_ling_consume", InitChouLingConsumeCfg);

	PUGI_XML_LOAD_CONFIG("huanhua_special_cap_add", InitHuanHuaSpecialCapAddCfg);

	return true;
}

const XiannvBaseCfg::ConfigItem * XiannvConfig::GetBaseCfg(int xiannv_id) const
{
	if (xiannv_id < 0 || xiannv_id >= MAX_XIANNV_COUNT)
	{
		return NULL;
	}

	return &m_base_cfg.cfg_list[xiannv_id];
}

const XiannvLevelCfg::ConfigItem * XiannvConfig::GetLevelCfg(int xiannv_id, int level) const
{
	if (xiannv_id < 0 || xiannv_id >= MAX_XIANNV_COUNT || level <= 0 || level > MAX_XIANNV_LEVEL)
	{
		return NULL;
	}

	return &m_level_cfg.cfg_list[xiannv_id][level];
}

const XiannvHuanHuaLevelCfg::ConfigItem * XiannvConfig::GetHuanHuaLevelCfg(int huanhua_id, int level) const
{
	if (huanhua_id < 0 || huanhua_id >= MAX_XIANNV_HUANHUA_COUNT || level <= 0 || level > MAX_XIANNV_HUANHUA_LEVEL)
	{
		return NULL;
	}

	if (m_huanhua_level_cfg.cfg_list[huanhua_id][level].level > 0)
	{
		return &m_huanhua_level_cfg.cfg_list[huanhua_id][level];
	}

	return NULL;
}

const NvshenShengwuSkillCfg::SkillItem * XiannvConfig::GetShengwuSkillCfg(int skill_id, int skill_level) const
{
	if (skill_id <= 0 || skill_id >= XIANNV_SHENGWU_SKILL_TYPE_COUNT)
	{
		return NULL;
	}

	const NvshenShengwuSkillCfg &skill_cfg = m_skill_cfg_list[skill_id];
	if (skill_level <= 0 || skill_level > skill_cfg.max_level || skill_level > NvshenShengwuSkillCfg::MAX_SKILL_LEVEL)
	{
		return NULL;
	}

	return &skill_cfg.level_cfg_list[skill_level];
}

const NvshenShengwuExpChouCfg * XiannvConfig::GetShengwuExpChouCfg(int chou_type) const
{
	if (chou_type < NvshenShengwuExpChouCfg::CHOU_TYPE_NORMAL || chou_type >= NvshenShengwuExpChouCfg::CHOU_TYPE_COUNT)
	{
		return NULL;
	}

	return &m_chou_exp_cfg_list[chou_type];
}

const NvshenShengwuUpGradeCfg * XiannvConfig::GetShengwuUpgradeCfg(int shengwu_id) const
{
	if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID || shengwu_id > m_shengwu_max_id)
	{
		return nullptr;
	}
	const NvshenShengwuUpGradeCfg &shengwu_cfg = m_shengwu_upgrade_cfg_list[shengwu_id];
	return &shengwu_cfg;
}

const NvshenShengwuInfoCfg::ShengwuItem * XiannvConfig::GetShengwuCfg(int shengwu_id, int shengwu_level) const
{
	if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID || shengwu_id > m_shengwu_max_id)
	{
		return NULL;
	}

	const NvshenShengwuInfoCfg &shengwu_cfg = m_shengwu_info_cfg_list[shengwu_id];
	if (shengwu_level < 0 || shengwu_level > shengwu_cfg.max_level)
	{
		return NULL;
	}

	return &shengwu_cfg.level_cfg_list[shengwu_level];
}

const NvshenShengwuChouConsumeCfg* XiannvConfig::GetShengwuChouConsumeCfg(int level) const
{
	if (level < 0 || level > NvshenShengwuInfoCfg::MAX_LEVEL) return NULL;

	for (int i = 0; i < m_shengwu_chou_comsume_count && i < XIANNV_SHENGWU_CHOU_COMSUEM_MAX_COUNT; i++)
	{
		const NvshenShengwuChouConsumeCfg &cfg = m_shengwu_chou_comsuem_list[i];
		if (cfg.min_level <= level && cfg.max_level >= level)
		{
			return &cfg;
		}
	}

	return NULL;
}

const NvshenShengwuGongmingCfg::ItemCfg * XiannvConfig::GetGongmingGridCfg(int grid_id, int level) const
{
	if (grid_id < 0 || grid_id > XIANNV_SHENGWU_GONGMING_MAX_GRID_ID)
	{
		return NULL;
	}

	const NvshenShengwuGongmingCfg &grid_cfg = m_grid_cfg_list[grid_id];
	if (level < 0 || level > NvshenShengwuGongmingCfg::MAX_LEVEL || level > grid_cfg.max_level)
	{
		return NULL;
	}

	return &grid_cfg.level_cfg_list[level];
}

const std::set<int> * XiannvConfig::GetGridDependCfg(int grid) const
{
	std::map<int, std::set<int> >::const_iterator it = m_grid_depend_list_map.find(grid);
	if (m_grid_depend_list_map.end() == it)
	{
		return NULL;
	}

	return &it->second;
}

const NvshenShengwuChouLingCfg * XiannvConfig::GetChouLingCfg(int cur_ling_count) const
{
	if (cur_ling_count < 0 || cur_ling_count >= NvshenShengwuChouLingCfg::MAX_LING_CFG_COUNT)
	{
		return NULL;
	}

	return &m_chou_ling_cfg_list[cur_ling_count];
}

const XiannvHuanHuaSpecialCapAddCfgItem* XiannvConfig::GetHuanHuaSpecialCapAddCfg(int huanhua_id)const
{
	if (huanhua_id <= 0 || huanhua_id > MAX_XIANNV_HUANHUA_COUNT)
	{
		return nullptr;
	}

	auto iter = m_huanhua_special_cap_cfg.find(huanhua_id);
	if (iter != m_huanhua_special_cap_cfg.end())
	{
		return &(iter->second);
	}

	return nullptr;
}

bool XiannvConfig::IsAddSpecialCap()
{
	return m_is_open_special_cap_add;
}

const int XiannvConfig::GetChouLingConsumeGold(int chou_ling_times) const
{
	for (int i = m_chou_ling_consume_cfg_count - 1; i >= 0; --i)
	{
		if (chou_ling_times >= m_chou_ling_consume_cfg_list[i].chou_ling_times)
		{
			return m_chou_ling_consume_cfg_list[i].consume_gold;
		}
	}

	return INT_MAX;
}

const XiannvZiZhiCfg::ConfigItem * XiannvConfig::GetZiZhiCfg(int xiannv_id, int level) const
{
	if (xiannv_id < 0 || xiannv_id >= MAX_XIANNV_COUNT || level <= 0 || level > MAX_XIANNV_ZIZHI)
	{
		return NULL;
	}

	return &m_zizhi_cfg.cfg_list[xiannv_id][level];
}

const XiannvHuanHuaCfg::ConfigItem * XiannvConfig::GetHuanHuaCfg(int huanhua_id) const
{
	if (huanhua_id < 0 || huanhua_id > m_huanhua_cfg.huanhua_count || huanhua_id >= MAX_XIANNV_HUANHUA_COUNT)
	{
		return NULL;
	}

	return &m_huanhua_cfg.cfg_list[huanhua_id];
}

int XiannvConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	if (!PugiGetSubNodeValue(dataElement, "main_percent", m_other_cfg.main_percent) || m_other_cfg.main_percent < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "aid_percent", m_other_cfg.aid_percent) || m_other_cfg.aid_percent < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "idle_percent", m_other_cfg.idle_percent) || m_other_cfg.idle_percent < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "rename_consume_gold", m_other_cfg.rename_consume_gold) || m_other_cfg.rename_consume_gold < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "chou_ling_free_times", m_other_cfg.chou_ling_free_times) || m_other_cfg.chou_ling_free_times < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "double_ling_gold", m_other_cfg.double_ling_gold) || m_other_cfg.double_ling_gold <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "fetch_ling_time", m_other_cfg.fetch_ling_time) || m_other_cfg.fetch_ling_time < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "is_open_special_cap_add", m_is_open_special_cap_add))
	{
		return -10;
	}
	
	return 0; 
}

int XiannvConfig::InitBaseCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_id = -1;

	while (!dataElement.empty())
	{
		int id = 0;
		if (!PugiGetSubNodeValue(dataElement, "id", id) || id != last_id + 1 || id >= MAX_XIANNV_COUNT)
		{
			return -1;
		}
		last_id = id;

		XiannvBaseCfg::ConfigItem &cfg = m_base_cfg.cfg_list[id];

		std::string xiannv_name;
		if (!PugiGetSubNodeValue(dataElement, "name", xiannv_name))
		{
			return -11;
		}
		if (xiannv_name.length() >= sizeof(cfg.xiannv_name))
		{
			return -12;
		}
		memcpy(cfg.xiannv_name, xiannv_name.c_str(), (int)xiannv_name.length());
		cfg.xiannv_name[xiannv_name.length()] = 0;

		if (!PugiGetSubNodeValue(dataElement, "active_item", cfg.active_item_id) || NULL == ITEMPOOL->GetItem(cfg.active_item_id)) 
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "skill_id", cfg.skill_id) || cfg.skill_id < 0 || cfg.skill_id >= SkillPool::MAX_SKILL_ID)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_send_system_msg", cfg.is_send_system_msg))
		{
			return -15;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_xiannv_id = 0;
	int last_level = 0;
	while (!dataElement.empty())
	{
		int id = 0;
		if (!PugiGetSubNodeValue(dataElement, "xiannv_id", id) || id < 0 || id >= MAX_XIANNV_COUNT)
		{
			return -1;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > MAX_XIANNV_LEVEL)
		{
			return -2;
		}

		XiannvLevelCfg::ConfigItem &cfg = m_level_cfg.cfg_list[id][level];
		if (0 != cfg.level)
		{
			return -100;
		}

		if (last_xiannv_id != id && last_level != MAX_XIANNV_LEVEL)
		{
			return -1000;
		}

		last_xiannv_id = id;
		last_level = level;

		cfg.id = id;
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) 
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) 
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) 
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) 
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) 
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "xiannv_gongji", cfg.xiannv_gongji) || cfg.xiannv_gongji < 0) 
		{
			return -10;
		}

		dataElement = dataElement.next_sibling();
	}

	if (last_level != MAX_XIANNV_LEVEL)
	{
		return -1001;
	}

	return 0;
}


int XiannvConfig::InitZiZhiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		int id = 0;
		if (!PugiGetSubNodeValue(dataElement, "xiannv_id", id) || id < 0 || id >= MAX_XIANNV_COUNT)
		{
			return -1;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > MAX_XIANNV_LEVEL)
		{
			return -2;
		}

		XiannvZiZhiCfg::ConfigItem &cfg = m_zizhi_cfg.cfg_list[id][level];
		if (0 != cfg.level)
		{
			return -100;
		}

		cfg.id = id;
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_id", cfg.uplevel_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.uplevel_stuff_id))
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_num", cfg.uplevel_stuff_num) || cfg.uplevel_stuff_num <= 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "xiannv_gongji", cfg.xiannv_gongji) || cfg.xiannv_gongji < 0)
		{
			return -10;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitHuanHuaCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	m_huanhua_cfg.huanhua_count = 0;

	while (!dataElement.empty())
	{
		int id = 0;
		if (!PugiGetSubNodeValue(dataElement, "id", id) || id < 0 || id >= MAX_XIANNV_HUANHUA_COUNT)
		{
			return -1;
		}

		if (id != m_huanhua_cfg.huanhua_count)
		{
			return -2;
		}

		XiannvHuanHuaCfg::ConfigItem &cfg = m_huanhua_cfg.cfg_list[id];
		cfg.id = id;

		if (!PugiGetSubNodeValue(dataElement, "xiannv_gongji", cfg.xiannv_gongji) || cfg.xiannv_gongji < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_item", cfg.active_item_id) || NULL == ITEMPOOL->GetItem(cfg.active_item_id))
		{
			return -11;
		}

		m_huanhua_cfg.huanhua_count++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitHuanHuaLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		int id = 0;
		if (!PugiGetSubNodeValue(dataElement, "huanhua_id", id) || id < 0 || id >= MAX_XIANNV_HUANHUA_COUNT)
		{
			return -1;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > MAX_XIANNV_HUANHUA_LEVEL)
		{
			return -2;
		}

		XiannvHuanHuaLevelCfg::ConfigItem &cfg = m_huanhua_level_cfg.cfg_list[id][level];
		if (0 != cfg.level)
		{
			return -100;
		}

		cfg.id = id;
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_id", cfg.uplevel_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.uplevel_stuff_id)) 
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_num", cfg.uplevel_stuff_num) || cfg.uplevel_stuff_num <= 0) 
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) 
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) 
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) 
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) 
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) 
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "xiannv_gongji", cfg.xiannv_gongji) || cfg.xiannv_gongji < 0) 
		{
			return -10;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int pre_skill_id = 0;
	while (!data_element.empty())
	{
		int skill_id = 0;
		if (!PugiGetSubNodeValue(data_element, "skill_id", skill_id) || skill_id <= 0 || skill_id >= XIANNV_SHENGWU_SKILL_TYPE_COUNT)
		{
			return -1;
		}

		if (skill_id != pre_skill_id && skill_id != pre_skill_id + 1)
		{
			return -2;
		}
		pre_skill_id = skill_id;

		NvshenShengwuSkillCfg &skill_cfg = m_skill_cfg_list[skill_id];
		skill_cfg.skill_id = skill_id;

		if (skill_cfg.max_level >= NvshenShengwuSkillCfg::MAX_SKILL_LEVEL)
		{
			return -100;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != skill_cfg.max_level + 1)
		{
			return -3;
		}
		skill_cfg.max_level = level;

		NvshenShengwuSkillCfg::SkillItem &item_cfg = skill_cfg.level_cfg_list[level];
		item_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "trigger_rate", item_cfg.trigger_rate) || item_cfg.trigger_rate < 0 || item_cfg.trigger_rate > 10000)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "param_1", item_cfg.param_1) || item_cfg.param_1 < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "param_2", item_cfg.param_2) || item_cfg.param_2 < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "cool_down_ms", item_cfg.cool_down_ms) || item_cfg.cool_down_ms < 0)
		{
			return -7;
		}
		
		if (!PugiGetSubNodeValue(data_element, "capability", item_cfg.capability) || item_cfg.capability < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "capability_per", item_cfg.capability_per) || item_cfg.capability_per < 0)
		{
			return -9;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitUpGradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int pre_shengwu_id = 0;
	while (!data_element.empty())
	{
		int shengwu_id = 0;
		if (!PugiGetSubNodeValue(data_element, "shengwu_id", shengwu_id) || shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID)
		{
			return -1;
		}

		if (shengwu_id != pre_shengwu_id && shengwu_id != pre_shengwu_id + 1)
		{
			return -2;
		}
		pre_shengwu_id = shengwu_id;

		NvshenShengwuUpGradeCfg &upgrade_cfg = m_shengwu_upgrade_cfg_list[shengwu_id];
		upgrade_cfg.shengwu_id = shengwu_id;
		if (!PugiGetSubNodeValue(data_element, "upgrade_stuff_id", upgrade_cfg.upgrade_stuff_id) || nullptr == ITEMPOOL->GetItem(upgrade_cfg.upgrade_stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "exp_reward", upgrade_cfg.exp_reward) || upgrade_cfg.exp_reward < 0 || upgrade_cfg.exp_reward > MAX_XIANNV_UPGRADE_STUFF_EXP)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "max_upgrade_level", upgrade_cfg.max_upgrade_level) || upgrade_cfg.max_upgrade_level < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "active_need_fairy", upgrade_cfg.active_need_fairy) || upgrade_cfg.active_need_fairy < 0 || upgrade_cfg.active_need_fairy > MAX_XIANNV_COUNT)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "consume_type", upgrade_cfg.consume_type) || 
			upgrade_cfg.consume_type <= NvshenShengwuUpGradeCfg::CONSUME_TYPE_INVALID ||
			upgrade_cfg.consume_type >= NvshenShengwuUpGradeCfg::CONSUME_TYPE_MAX)
		{
			return -7;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitShengwuInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int pre_shengwu_id = 0;
	while (!data_element.empty())
	{
		int shengwu_id = 0;
		if (!PugiGetSubNodeValue(data_element, "shengwu_id", shengwu_id) || shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID)
		{
			return -1;
		}

		if (shengwu_id != pre_shengwu_id && shengwu_id != pre_shengwu_id + 1)
		{
			return -2;
		}
		pre_shengwu_id = shengwu_id;
		m_shengwu_max_id = shengwu_id;

		NvshenShengwuInfoCfg &shengwu_info_cfg = m_shengwu_info_cfg_list[shengwu_id];
		shengwu_info_cfg.shengwu_id = shengwu_id;

		if (shengwu_info_cfg.max_level >= NvshenShengwuInfoCfg::MAX_LEVEL)
		{
			return -3;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != shengwu_info_cfg.max_level + 1)
		{
			return -4;
		}
		shengwu_info_cfg.max_level = level;

		NvshenShengwuInfoCfg::ShengwuItem &item_cfg = shengwu_info_cfg.level_cfg_list[level];
		item_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "upgrade_need_exp", item_cfg.upgrade_need_exp) || item_cfg.upgrade_need_exp <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", item_cfg.maxhp) || item_cfg.maxhp < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", item_cfg.gongji) || item_cfg.gongji < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", item_cfg.fangyu) || item_cfg.fangyu < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", item_cfg.mingzhong) || item_cfg.mingzhong < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", item_cfg.shanbi) || item_cfg.shanbi < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", item_cfg.baoji) || item_cfg.baoji < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", item_cfg.jianren) || item_cfg.jianren < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(data_element, "fujia", item_cfg.fujia) || item_cfg.fujia < 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(data_element, "mianshang", item_cfg.mianshang) || item_cfg.mianshang < 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_id", item_cfg.skill_id) || item_cfg.skill_id < 0)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_level", item_cfg.skill_level) || item_cfg.skill_level < 0)
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", item_cfg.broadcast_type) || item_cfg.broadcast_type < NvshenShengwuInfoCfg::BROADCAST_TYPE_NONE || item_cfg.broadcast_type >= NvshenShengwuInfoCfg::BROADCAST_TYPE_COUNT)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(data_element, "upgrade_need_shengwu_essence", item_cfg.upgrade_need_shengwu_essence) || item_cfg.upgrade_need_shengwu_essence < 0)
		{
			return -23;
		}

		if (!PugiGetSubNodeValue(data_element, "upgrade_need_stuff_number", item_cfg.upgrade_need_stuff_number) || item_cfg.upgrade_need_stuff_number < 0)
		{
			return -24;
		}

		data_element = data_element.next_sibling();
	}

	if (pre_shengwu_id != XIANNV_SHENGWU_MAX_ID)
	{
		return -2000;
	}

	return 0;
}

int XiannvConfig::InitShengwuChouConsumeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_shengwu_chou_comsume_count = 0;
	int last_level = -1;

	while (!data_element.empty())
	{
		if (m_shengwu_chou_comsume_count < 0 || m_shengwu_chou_comsume_count >= XIANNV_SHENGWU_CHOU_COMSUEM_MAX_COUNT)
		{
			return -1;
		}

		NvshenShengwuChouConsumeCfg &cfg = m_shengwu_chou_comsuem_list[m_shengwu_chou_comsume_count];

		if (!PugiGetSubNodeValue(data_element, "min_level", cfg.min_level) || cfg.min_level < 0 || cfg.min_level <= last_level)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "max_level", cfg.max_level) || cfg.max_level < 0 || cfg.max_level < cfg.min_level)
		{
			return -3;
		}
		last_level = cfg.max_level;

		if (!PugiGetSubNodeValue(data_element, "chou_exp_stuff", cfg.chou_exp_stuff) || cfg.chou_exp_stuff <= 0 || NULL == ITEMPOOL->GetItem(cfg.chou_exp_stuff))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "chou_exp_gold", cfg.chou_exp_gold) || cfg.chou_exp_gold < 0)
		{
			return -5;
		}

		++m_shengwu_chou_comsume_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int chou_type = 0;
		if (!PugiGetSubNodeValue(data_element, "chou_type", chou_type) || chou_type < 0 || chou_type >= NvshenShengwuExpChouCfg::CHOU_TYPE_COUNT)
		{
			return -1;
		}
		NvshenShengwuExpChouCfg &chou_cfg = m_chou_exp_cfg_list[chou_type];
		chou_cfg.chou_type = chou_type;

		if (!PugiGetSubNodeValue(data_element, "rate1_exp", chou_cfg.rate1_exp) || chou_cfg.rate1_exp < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "rate2_exp", chou_cfg.rate2_exp) || chou_cfg.rate2_exp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "rate3_exp", chou_cfg.rate3_exp) || chou_cfg.rate3_exp < 0)
		{
			return -4;
		}

		//rate_shengwu_list
		data_element = data_element.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitGongmingCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int pre_grid_id = -1;
	while (!data_element.empty())
	{
		int grid_id = 0;
		if (!PugiGetSubNodeValue(data_element, "grid_id", grid_id) || grid_id < 0 || grid_id > XIANNV_SHENGWU_GONGMING_MAX_GRID_ID)
		{
			return -1;
		}

		if (grid_id != pre_grid_id && grid_id != pre_grid_id + 1)
		{
			return -2;
		}

		pre_grid_id = grid_id;

		NvshenShengwuGongmingCfg &grid_cfg = m_grid_cfg_list[grid_id];
		grid_cfg.grid_id = grid_id;

		if (grid_cfg.max_level >= NvshenShengwuGongmingCfg::MAX_LEVEL)
		{
			return -3;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != grid_cfg.max_level + 1)
		{
			return -4;
		}
		grid_cfg.max_level = level;
		NvshenShengwuGongmingCfg::ItemCfg &item_cfg = grid_cfg.level_cfg_list[level];
		item_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "upgrade_need_ling", item_cfg.upgrade_need_ling) || item_cfg.upgrade_need_ling < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", item_cfg.maxhp) || item_cfg.maxhp < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", item_cfg.gongji) || item_cfg.gongji < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", item_cfg.fangyu) || item_cfg.fangyu < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", item_cfg.mingzhong) || item_cfg.mingzhong < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", item_cfg.shanbi) || item_cfg.shanbi < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", item_cfg.baoji) || item_cfg.baoji < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", item_cfg.jianren) || item_cfg.jianren < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(data_element, "fujia", item_cfg.fujia) || item_cfg.fujia < 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(data_element, "mianshang", item_cfg.mianshang) || item_cfg.mianshang < 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_id", item_cfg.skill_id) || item_cfg.skill_id < 0)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(data_element, "trigger_rate", item_cfg.trigger_rate) || item_cfg.trigger_rate < 0 || item_cfg.trigger_rate > 10000)
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(data_element, "param_1", item_cfg.param_1) || item_cfg.param_1 < 0)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(data_element, "param_2", item_cfg.param_2) || item_cfg.param_2 < 0)
		{
			return -23;
		}

		if (!PugiGetSubNodeValue(data_element, "cool_down_ms", item_cfg.cool_down_ms) || item_cfg.cool_down_ms < 0)
		{
			return -24;
		}
		
		if (!PugiGetSubNodeValue(data_element, "capbility", item_cfg.capbility) || item_cfg.capbility < 0)
		{
			return -25;
		}
		
		if (!PugiGetSubNodeValue(data_element, "attr_percent", item_cfg.attr_percent) || item_cfg.attr_percent < 0)
		{
			return -26;
		}

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0)
		{
			return -27;
		}
		item_cfg.is_broadcast = (is_broadcast > 0);

		data_element = data_element.next_sibling();
	}

	if (pre_grid_id != XIANNV_SHENGWU_GONGMING_MAX_GRID_ID) // —œ∏ÒºÏ≤‚∏Ò◊”≈‰÷√∏ˆ ˝
	{
		return -2000;
	}

	return 0;
}

int XiannvConfig::InitGridDependCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int grid_id = 0;
		if (!PugiGetSubNodeValue(data_element, "grid_id", grid_id) || grid_id < 0 || grid_id > XIANNV_SHENGWU_GONGMING_MAX_GRID_ID)
		{
			return -1;
		}

		std::set<int> &depend_list = m_grid_depend_list_map[grid_id];

		if (!PugiGetSubNodeValue(data_element, "grid_id1", grid_id) || grid_id < 0 || grid_id > XIANNV_SHENGWU_GONGMING_MAX_GRID_ID)
		{
			return -2;
		}
		depend_list.insert(grid_id);

		if (PugiGetSubNodeValue(data_element, "grid_id2", grid_id))
		{
			if (grid_id < 0 || grid_id > XIANNV_SHENGWU_GONGMING_MAX_GRID_ID)
			{
				return -3;
			}

			depend_list.insert(grid_id);
		}

		if (PugiGetSubNodeValue(data_element, "grid_id3", grid_id))
		{
			if (grid_id < 0 || grid_id > XIANNV_SHENGWU_GONGMING_MAX_GRID_ID)
			{
				return -4;
			}

			depend_list.insert(grid_id);
		}

		if (PugiGetSubNodeValue(data_element, "grid_id4", grid_id))
		{
			if (grid_id < 0 || grid_id > XIANNV_SHENGWU_GONGMING_MAX_GRID_ID)
			{
				return -5;
			}

			depend_list.insert(grid_id);
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitChouLingCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int cur_ling_count = 0;
	while (!data_element.empty())
	{
		if (cur_ling_count >= NvshenShengwuChouLingCfg::MAX_LING_CFG_COUNT)
		{
			return -1;
		}

		int ling_count = 0;
		if (!PugiGetSubNodeValue(data_element, "ling_count", ling_count) || ling_count != cur_ling_count)
		{
			return -2;
		}

		NvshenShengwuChouLingCfg &ling_cfg = m_chou_ling_cfg_list[ling_count];
		ling_cfg.ling_count = ling_count;

		if (!PugiGetSubNodeValue(data_element, "ling_value", ling_cfg.ling_value) || ling_cfg.ling_value <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_rate", ling_cfg.gold_rate) || ling_cfg.gold_rate <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "free_rate", ling_cfg.free_rate) || ling_cfg.free_rate <= 0)
		{
			return -6;
		}
		
		++cur_ling_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitChouLingConsumeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_chou_ling_consume_cfg_count = 0;
	int pre_chou_times = -1;

	while (!data_element.empty())
	{
		if (m_chou_ling_consume_cfg_count >= NvshenShengwuChouLingConsumeCfg::MAX_CFG_COUNT)
		{
			return -1;
		}

		NvshenShengwuChouLingConsumeCfg &consume_cfg = m_chou_ling_consume_cfg_list[m_chou_ling_consume_cfg_count];
		if (!PugiGetSubNodeValue(data_element, "chou_ling_times", consume_cfg.chou_ling_times) || consume_cfg.chou_ling_times <= pre_chou_times)
		{
			return -2;
		}
		pre_chou_times = consume_cfg.chou_ling_times;

		if (!PugiGetSubNodeValue(data_element, "consume_gold", consume_cfg.consume_gold) || consume_cfg.consume_gold <= 0)
		{
			return -3;
		}

		++m_chou_ling_consume_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int XiannvConfig::InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		XiannvHuanHuaSpecialCapAddCfgItem cfg_item;
		if (!PugiGetSubNodeValue(dataElement, "huanhua_id", cfg_item.huanhua_id) || cfg_item.huanhua_id < 0 || cfg_item.huanhua_id > MAX_XIANNV_HUANHUA_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "huanhua_level", cfg_item.huanhua_level) || cfg_item.huanhua_level <= 0 || cfg_item.huanhua_level > MAX_XIANNV_HUANHUA_COUNT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg_item.maxhp) || cfg_item.maxhp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg_item.gongji) || cfg_item.gongji < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg_item.fangyu) || cfg_item.fangyu < 0)
		{
			return -5;
		}

		// ≈–∂œ÷ÿ∏¥
		if (m_huanhua_special_cap_cfg.find(cfg_item.huanhua_id) != m_huanhua_special_cap_cfg.end())
		{
			return -6;
		}
		m_huanhua_special_cap_cfg[cfg_item.huanhua_id] = cfg_item;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}
