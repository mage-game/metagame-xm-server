#include "imgfulingconfig.hpp"

#include "common/tinyxml/tinyxml.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

ImgFulingConfig::ImgFulingConfig()
{

}

ImgFulingConfig::~ImgFulingConfig()
{

}

bool ImgFulingConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// 等级配置
	PUGI_XML_LOAD_CONFIG("fuling_level", InitLevelCfg);
	// 技能配置
	PUGI_XML_LOAD_CONFIG("fuling_skill", InitSkillCfg);
	// 升级物品配置
	PUGI_XML_LOAD_CONFIG("fuling_stuff", InitUpLevelStuffCfg);
	// 进阶装备加成配置
	PUGI_XML_LOAD_CONFIG("jingjie_equip_per_add", InitJineJieEquipPerAddCfg);

	return true;
}

const ImgFulingLevelCfg * ImgFulingConfig::GetLevelCfg(int jingjie_type, int level)
{
	if (jingjie_type < JINGJIE_TYPE_MOUNT || jingjie_type >= JINGJIE_TYPE_COUNT)
	{
		return NULL;
	}

	const std::vector<ImgFulingLevelCfg> &level_vec = m_img_fuling_level_cfg_list[jingjie_type];
	if (level < 0 || level >= static_cast<int>(level_vec.size()) || level != level_vec[level].level)
	{
		return NULL;
	}

	return &level_vec[level];
}

const ImgFulingLevelCfg * ImgFulingConfig::GetAttrCfg(int jingjie_type, int level)
{
	if (jingjie_type < JINGJIE_TYPE_MOUNT || jingjie_type >= JINGJIE_TYPE_COUNT)
	{
		return NULL;
	}

	const std::vector<ImgFulingLevelCfg> &level_vec = m_img_fuling_level_cfg_list[jingjie_type];
	if (level <= 0 || level >= static_cast<int>(level_vec.size()) || level != level_vec[level].level)
	{
		return NULL;
	}

	return &level_vec[level];
}

int ImgFulingConfig::GetJingJieTypeBySkillIndex(int index)
{
	if (index < IMG_FULING_SKILL_0 || index >= IMG_FULING_SKILL_MAX)
	{
		return -1;
	}

	if (!m_img_fuling_skill_cfg_list[index].empty())
	{
		return m_img_fuling_skill_cfg_list[index].at(0).system_type;
	}

	return -1;
}

int ImgFulingConfig::GetSkillIndexByJingJieType(int jingjie_type)
{
	if (jingjie_type < JINGJIE_TYPE_MOUNT || jingjie_type >= JINGJIE_TYPE_COUNT)
	{
		return -1;
	}

	for (int i = 0; i < IMG_FULING_SKILL_MAX; ++i)
	{
		if (!m_img_fuling_skill_cfg_list[i].empty() && m_img_fuling_skill_cfg_list[i].at(0).system_type == jingjie_type)
		{
			return i;
		}
	}

	return -1;
}

const ImgFulingSkillCfg * ImgFulingConfig::GetSkillCfg(int index, int skill_level)
{
	if (index < IMG_FULING_SKILL_0 || index >= IMG_FULING_SKILL_MAX)
	{
		return NULL;
	}

	if (skill_level <= 0 || skill_level > static_cast<int>(m_img_fuling_skill_cfg_list[index].size()))
	{
		return NULL;
	}

	return &m_img_fuling_skill_cfg_list[index].at(skill_level - 1);
}

int ImgFulingConfig::GetSkillLevelByCount(int index, int img_count)
{
	if (index < JINGJIE_TYPE_MOUNT || index >= JINGJIE_TYPE_COUNT)
	{
		return 0;
	}

	const std::vector<ImgFulingSkillCfg> &skill_vec = m_img_fuling_skill_cfg_list[index];

	for (std::vector<ImgFulingSkillCfg>::const_reverse_iterator iter = skill_vec.rbegin(); iter != skill_vec.rend(); ++iter)
	{
		if (img_count >= iter->img_count_limit)
		{
			return iter->level;
		}
	}

	return 0;
}

const ImgFulingStuffCfg * ImgFulingConfig::GetStuffCfg(int jingjie_type, int stuff_index)
{
	if (jingjie_type < JINGJIE_TYPE_MOUNT || jingjie_type >= JINGJIE_TYPE_COUNT)
	{
		return NULL;
	}

	const std::vector<ImgFulingStuffCfg> &stuff_vec = m_img_fuling_stuff_cfg_list[jingjie_type];
	if (stuff_index < 0 || stuff_index >= static_cast<int>(stuff_vec.size()))
	{
		return NULL;
	}

	return &stuff_vec[stuff_index];
}

int ImgFulingConfig::GetJingJieEquipPerAdd(int min_level)
{
	if (min_level <= 0 || min_level > static_cast<int>(m_img_fuling_jingjie_equip_per_add_cfg_vec.size()) + 1 || 
		m_img_fuling_jingjie_equip_per_add_cfg_vec[min_level - 1].min_level != min_level)
	{
		return 0;
	}

	return m_img_fuling_jingjie_equip_per_add_cfg_vec[min_level - 1].jingjie_equip_per_add;
}

int ImgFulingConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_level = -1;
	int last_system_type = -1;
	while (!data_element.empty())
	{
		int system_type = -1;
		if (!PugiGetSubNodeValue(data_element, "system_type", system_type) || system_type < 0 || system_type >= JINGJIE_TYPE_COUNT)
		{
			return -1;
		}

		if (last_system_type != system_type)
		{
			last_level = -1;
			last_system_type = system_type;
		}
		
		std::vector<ImgFulingLevelCfg> &level_vec = m_img_fuling_level_cfg_list[system_type];

		int level = -1;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level != last_level + 1)
		{
			return -2;
		}

		last_level = level;

		ImgFulingLevelCfg level_cfg;
		level_cfg.level = level;
		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "per_add", level_cfg.per_add) || level_cfg.per_add < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "exp", level_cfg.exp) || level_cfg.exp < 0)
		{
			return -11;
		}

		int broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", broadcast) || (broadcast != 0 && broadcast != 1))
		{
			return -12;
		}

		level_cfg.broadcast = broadcast > 0;
		level_vec.push_back(level_cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ImgFulingConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int index = -1;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0 || index >= IMG_FULING_SKILL_MAX)
		{
			return -1000;
		}

		std::vector<ImgFulingSkillCfg> &skill_vec = m_img_fuling_skill_cfg_list[index];

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "skill_level", level) || level <= 0 || level != static_cast<int>(skill_vec.size()) + 1)
		{
			return -1;
		}

		ImgFulingSkillCfg skill_cfg;
		skill_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "system_type", skill_cfg.system_type) || skill_cfg.system_type < 0 || skill_cfg.system_type >= JINGJIE_TYPE_COUNT)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "param_a", skill_cfg.param_a) || skill_cfg.param_a < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "param_b", skill_cfg.param_b) || skill_cfg.param_b < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "param_c", skill_cfg.param_c) || skill_cfg.param_c < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "param_d", skill_cfg.param_d) || skill_cfg.param_d < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "img_count_limit", skill_cfg.img_count_limit) || skill_cfg.img_count_limit < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "param_e", skill_cfg.param_e) || skill_cfg.param_e < 0)
		{
			return -7;
		}

		int broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", broadcast) || (broadcast != 0 && broadcast != 1))
		{
			return -8;
		}

		skill_cfg.broadcast = broadcast > 0;
		skill_vec.push_back(skill_cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ImgFulingConfig::InitUpLevelStuffCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int system_type = -1;
		if (!PugiGetSubNodeValue(data_element, "system_type", system_type) || system_type < 0 || system_type >= JINGJIE_TYPE_COUNT)
		{
			return -1;
		}

		std::vector<ImgFulingStuffCfg> &stuff_vec = m_img_fuling_stuff_cfg_list[system_type];

		ImgFulingStuffCfg stuff_cfg;
		if (!PugiGetSubNodeValue(data_element, "stuff_id", stuff_cfg.stuff_id) || stuff_cfg.stuff_id < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "add_exp", stuff_cfg.add_exp) || stuff_cfg.add_exp <= 0)
		{
			return -3;
		}

		stuff_vec.push_back(stuff_cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ImgFulingConfig::InitJineJieEquipPerAddCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ImgFulingJingJieEquipPerAdd equip_add_cfg;
		if (!PugiGetSubNodeValue(data_element, "min_level", equip_add_cfg.min_level) || equip_add_cfg.min_level < 0 || 
			equip_add_cfg.min_level != static_cast<int>(m_img_fuling_jingjie_equip_per_add_cfg_vec.size()) + 1)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "jingjie_equip_per_add", equip_add_cfg.jingjie_equip_per_add) || equip_add_cfg.jingjie_equip_per_add < 0)
		{
			return -2;
		}

		m_img_fuling_jingjie_equip_per_add_cfg_vec.push_back(equip_add_cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}