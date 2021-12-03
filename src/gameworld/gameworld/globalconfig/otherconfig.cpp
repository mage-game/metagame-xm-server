#include "otherconfig.h"
#include "servercommon/configcommon.h"
#include "gameworld/gameworld/item/itempool.h"

OtherConfig::OtherConfig()
{
	
}

OtherConfig::~OtherConfig()
{

}

bool OtherConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "OtherConfig", *err);
	
	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 装备技能信息
		PugiXmlNode root_element = RootElement.child("equip_skill");
		if (root_element.empty())
		{
			*err = path + ": no [equip_skill].";
			return false;
		}

		iRet = this->InitEquipSkillCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitEquipSkillCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 组队副本怪物血量修正
		PugiXmlNode root_element = RootElement.child("teamfb_monster_hp");
		if (root_element.empty())
		{
			*err = path + ": no [teamfb_monster_hp].";
			return false;
		}

		iRet = this->InitTeamFBMonsterHpCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitTeamFBMonsterHpCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int OtherConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "cross_relive_gold", m_other_cfg.cross_relive_gold) || m_other_cfg.cross_relive_gold <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "cross_free_relive_time", m_other_cfg.cross_free_relive_time) || m_other_cfg.cross_free_relive_time <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "special_grid_open_level", m_other_cfg.special_grid_open_level) || m_other_cfg.special_grid_open_level <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "today_free_relive_num", m_other_cfg.today_free_relive_num) || m_other_cfg.today_free_relive_num < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "zhuizong_item_id", m_other_cfg.zhuizong_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.zhuizong_item_id))
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "flyshoe_item_id", m_other_cfg.flyshoe_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.flyshoe_item_id))
	{
		return -6;
	}

	return 0;
}

int OtherConfig::InitEquipSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int equip_item_id = 0;
		if (!PugiGetSubNodeValue(data_element, "equip_item_id", equip_item_id) || equip_item_id < 0)
		{
			return -1;
		}

		int skill_id = 0;
		if (!PugiGetSubNodeValue(data_element, "skill_id", skill_id) || skill_id < 0)
		{
			return -2;
		}

		if (m_item_to_skill_map.find(equip_item_id) != m_item_to_skill_map.end())
		{
			return -3;
		}

		m_item_to_skill_map[equip_item_id] = skill_id;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int OtherConfig::GetSkillIDWithItemId(ItemID item_id)
{
	std::map<ItemID, int>::iterator skill_it = m_item_to_skill_map.find(item_id);
	if (skill_it != m_item_to_skill_map.end())
	{
		return skill_it->second;
	}

	return 0;
}

int OtherConfig::InitTeamFBMonsterHpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int team_member_num = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "team_member_num", team_member_num) || team_member_num <= 0 || team_member_num > MAX_TEAM_MEMBER_NUM)
		{
			return -1;
		}

		OtherTeamfbMonsterAttrConfig & cfg = m_teamfb_monster_attr_cfg[team_member_num];

		if (!PugiGetSubNodeValue(data_element, "monster_hp_percent", cfg.monster_hp_percent) || cfg.monster_hp_percent < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_fangyu_percent", cfg.monster_fangyu_percent) || cfg.monster_fangyu_percent < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_gongji_percent", cfg.monster_gongji_percent) || cfg.monster_gongji_percent < 0)
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const OtherTeamfbMonsterAttrConfig* OtherConfig::GetTeamFbMonsterAttrCfg(int team_member_num)
{
	if (team_member_num > MAX_TEAM_MEMBER_NUM || team_member_num <= 0)
	{
		return &m_teamfb_monster_attr_cfg[MAX_TEAM_MEMBER_NUM];
	}

	return &m_teamfb_monster_attr_cfg[team_member_num];
}

