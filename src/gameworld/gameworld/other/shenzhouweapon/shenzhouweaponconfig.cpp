#include "shenzhouweaponconfig.hpp"
#include "obj/character/role.h"
#include "item/itemextern.hpp"
#include "world.h"
#include "item/itempool.h"
#include "scene/sceneregister/sceneregister.hpp"
#include <servercommon/configcommon.h>

ShenzhouWeaponConfig::ShenzhouWeaponConfig() : m_gather_reward_count_max(0), m_exchange_identify_exp_cfg_count(0), m_identify_level_cfg_count(0), 
m_identify_item_cfg_count(0), m_box_reward_total_weight(0)
{
	memset(m_xilian_consuem_total_weight, 0, sizeof(m_xilian_consuem_total_weight));
	memset(m_xilian_shuxing_total_weight, 0, sizeof(m_xilian_shuxing_total_weight));
	memset(m_xilian_suit_count, 0, sizeof(m_xilian_suit_count));
}

ShenzhouWeaponConfig::~ShenzhouWeaponConfig()
{

}

bool ShenzhouWeaponConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ShenzhouWeaponConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// ÆäËûÅäÖÃÐÅÏ¢
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ²É¼¯Îï
		PugiXmlNode root_element = RootElement.child("gather_poins");
		if (root_element.empty())
		{
			*err = configname + ": no [gather_poins].";
			return false;
		}

		iRet = this->InitGatherPointsCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGatherPointsCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ²É¼¯½±Àø
		PugiXmlNode root_element = RootElement.child("gather_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [gather_reward].";
			return false;
		}

		iRet = this->InitGatherRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGatherRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¶Ò»»¼ø¶¨¾­Ñé
		PugiXmlNode root_element = RootElement.child("exchange_identify_exp");
		if (root_element.empty())
		{
			*err = configname + ": no [exchange_identify_exp].";
			return false;
		}

		iRet = this->InitExchangeIdentifyExpCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitExchangeIdentifyExpCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¼ø¶¨µÈ¼¶
		PugiXmlNode root_element = RootElement.child("identify_level");
		if (root_element.empty())
		{
			*err = configname + ": no [identify_level].";
			return false;
		}

		iRet = this->InitIdentifyLevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitIdentifyLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¼ø¶¨ÎïÆ·ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("identify_item_cfg");
		if (root_element.empty())
		{
			*err = configname + ": no [identify_suc_pro].";
			return false;
		}

		iRet = this->InitIdentifyItemCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitIdentifySucWeightCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¼ø¶¨ÎïÆ·³É¹¦½±Àø¸ÅÂÊ
		PugiXmlNode root_element = RootElement.child("identify_reward_pro");
		if (root_element.empty())
		{
			*err = configname + ": no [identify_reward_pro].";
			return false;
		}

		iRet = this->InitIdentifyRewardWeightCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitIdentifyRewardWeightCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ²¿Î»ÊôÐÔ
		PugiXmlNode root_element = RootElement.child("hunqi_slot_level_attr");
		if (root_element.empty())
		{
			*err = configname + ": no [hunqi_slot_level_attr].";
			return false;
		}

		iRet = this->InitSlotAttrCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSlotAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¼¼ÄÜÊôÐÔ
		PugiXmlNode root_element = RootElement.child("hunqi_skill");
		if (root_element.empty())
		{
			*err = configname + ": no [hunqi_skill].";
			return false;
		}

		iRet = this->InitWeaponSkillCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitWeaponSkillCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ±¦Ïä
		PugiXmlNode root_element = RootElement.child("box");
		if (root_element.empty())
		{
			*err = configname + ": no [box].";
			return false;
		}

		iRet = this->InitBoxCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBoxCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ±¦Ïä½±Àø
		PugiXmlNode root_element = RootElement.child("box_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [box_reward].";
			return false;
		}

		iRet = this->InitBoxRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBoxRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// »êÆ÷Á¶»êÅäÖÃ
		PugiXmlNode root_element = RootElement.child("element_cfg");
		if (root_element.empty())
		{
			*err = configname + ": no [element_cfg].";
			return false;
		}

		iRet = this->InitElementLevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitElementLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// »êÆ÷»êÓ¡ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("hunyin");
		if (root_element.empty())
		{
			*err = configname + ": no [hunyin].";
			return false;
		}

		iRet = this->InitHunYinCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitHunYinCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// »êÆ÷»êÓ¡ÁéÊàµÈ¼¶ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("lingshu");
		if (root_element.empty())
		{
			*err = configname + ": no [lingshu].";
			return false;
		}

		iRet = this->InitHunyinLingshuLevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitHunyinLingshuLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// »êÆ÷»êÓ¡Ì××°ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("hunyin_suit");
		if (root_element.empty())
		{
			*err = configname + ": no [hunyin_suit].";
			return false;
		}

		//iRet = this->InitHunyinSuitCfg(root_element);
		iRet = this->InitNewHunyinSuitCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitHunyinSuitCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// »êÆ÷»êÓ¡²Û¿ªÆôÅäÖÃ
		PugiXmlNode root_element = RootElement.child("hunyin_slot_open");
		if (root_element.empty())
		{
			*err = configname + ": no [hunyin_slot_open].";
			return false;
		}

		iRet = this->InitHunyinSlotOpenCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitHunyinSlotOpenCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}


	{
		PugiXmlNode root_element = RootElement.child("box_reward_count_cfg");
		if (root_element.empty())
		{
			*err = configname + ": no [box_reward_count_cfg].";
			return false;
		}

		iRet = this->InitBoxRewardCountCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBoxRewardCountCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("xilian_open");
		if (root_element.empty())
		{
			*err = configname + ": no [xilian_open].";
			return false;
		}

		iRet = this->InitXilianOpenCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitXilianOpenCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("lock_comsume");
		if (root_element.empty())
		{
			*err = configname + ": no [lock_comsume].";
			return false;
		}

		iRet = this->InitXilianLockCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitXilianLockCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("xilian_comsume");
		if (root_element.empty())
		{
			*err = configname + ": no [xilian_comsume].";
			return false;
		}

		iRet = this->InitXilianConsumeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitXilianConsumeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("xilian_shuxing_type");
		if (root_element.empty())
		{
			*err = configname + ": no [xilian_shuxing_type].";
			return false;
		}

		iRet = this->InitXilianShuxingCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitXilianShuxingCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

ShenzhouGatherRewardCfg * ShenzhouWeaponConfig::GetGatherRandReward()
{
	int total_pro = 0;

	for (int i = 0; i < m_gather_reward_count_max && i < SHENZHOU_MAX_GATHER_REWARD_COUNT; i++)
	{
		total_pro += m_gather_reward_list[i].gain_pro;
	}

	if (total_pro <= 0) return NULL;

	int rand_pro = RandomNum(total_pro);

	for(int j = 0; j < m_gather_reward_count_max && j < SHENZHOU_MAX_GATHER_REWARD_COUNT; j++)
	{
		if (rand_pro < m_gather_reward_list[j].gain_pro)
		{
			return &m_gather_reward_list[j];
		}
		else
		{
			rand_pro -= m_gather_reward_list[j].gain_pro;
		}
	}

	return NULL;
}

const ShenzhouSlotAttrCfg::LevelItemConfig * ShenzhouWeaponConfig::GetSlotLevelAttrCfg(int hunqi_id, int slot, int level)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return NULL;

	if (slot < 0 || slot >= SHENZHOU_WEAPON_SLOT_COUNT) return NULL;

	if (level < 0 || level > ShenzhouSlotAttrCfg::CFG_MAX_COUNT) return NULL;

	ShenzhouSlotAttrCfg::LevelItemConfig &slot_cfg = m_slot_attr_cfglist[hunqi_id][slot].cfg_list[level];

	if (slot_cfg.level != level) return NULL;

	return &slot_cfg;
}

const ShenzhouElementCfg::ShenzhouElementLevelCfg * ShenzhouWeaponConfig::GetElemetLevelAttrCfg(int hunqi_id, int element_type, int elemet_level)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return NULL;

	if (element_type < 0 || element_type >= SHENZHOU_ELEMET_MAX_TYPE) return NULL;

	if (elemet_level < 0 || elemet_level > SHENZHOU_ELEMET_MAX_LEVEL) return NULL;

	ShenzhouElementCfg::ShenzhouElementLevelCfg &element_cfg = m_element_level_cfg[hunqi_id].element_level_cfg[element_type][elemet_level];

	if (element_cfg.element_level != elemet_level) return NULL;

	return &element_cfg;
}

ShenzhouSkillCfg * ShenzhouWeaponConfig::GetWeaponSkillCfg(int hunqi_id, int hunqi_level)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return NULL;

	if (hunqi_level <= 0 || hunqi_level > ShenzhouSlotAttrCfg::CFG_MAX_COUNT) return NULL;

	ShenzhouSkillCfg &skill_cfg = m_weapon_skill_cfg[hunqi_id][hunqi_level];
	//if (skill_cfg.level <= 0) return NULL;
	if (skill_cfg.level <= 0)
	{
		for (int i = 0; i < 50; i++)
		{
			hunqi_level--;
			if (hunqi_level <= 0 || hunqi_level > ShenzhouSlotAttrCfg::CFG_MAX_COUNT) return NULL;
			
			if (m_weapon_skill_cfg[hunqi_id][hunqi_level].level > 0)
			{
				skill_cfg = m_weapon_skill_cfg[hunqi_id][hunqi_level];
				break;
			}
		}
	}

	return &skill_cfg;
}

const ShenzhouExchangeIdentifyExpCfg * ShenzhouWeaponConfig::GetExchangeIdentifyExpCfg(int seq)
{
	if (seq < 0 || seq >= m_exchange_identify_exp_cfg_count)
	{
		return NULL;
	}

	return &m_exchange_identify_exp_list[seq];
}

const ShenzhouIdentifyLevelCfg * ShenzhouWeaponConfig::GetIdentifyLevelCfg(int level)
{
	if (level < 0 || level >= m_identify_level_cfg_count || level >= SHENZHOU_IDENTIFY_LEVEL_CFG_COUNT)
	{
		return NULL;
	}

	return &m_identify_level_cfglist[level];
}

const ShenzhouIdentifyLevelCfg::AttributeConfig *ShenzhouWeaponConfig::GetIdentifyAttributeCfg(int level, int star_level)
{
	if (level < 0 || level >= m_identify_level_cfg_count || level >= SHENZHOU_IDENTIFY_LEVEL_CFG_COUNT) return NULL;

	ShenzhouIdentifyLevelCfg &cfg = m_identify_level_cfglist[level];
	if (star_level >= 0 && star_level <= cfg.star_max_level && star_level <= SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT)
	{
		return &cfg.attribute_cfglist[star_level];
	}

	return NULL;
}

const ShenzhouIdentifyLevelCfg::AttributeConfig *ShenzhouWeaponConfig::GetIdentifyNextAttributeCfg(int level, int star_level)
{
	if (level < 0 || level >= m_identify_level_cfg_count || level >= SHENZHOU_IDENTIFY_LEVEL_CFG_COUNT) return NULL;

	ShenzhouIdentifyLevelCfg &cfg = m_identify_level_cfglist[level];
	if (star_level >= 0 && star_level < cfg.star_max_level && star_level < SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT)
	{
		return &cfg.attribute_cfglist[star_level + 1];
	}
	else if (star_level >= 0 && (star_level == cfg.star_max_level || star_level == SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT))
	{
		int next_level = level + 1;

		if (next_level >= 0 && next_level < m_identify_level_cfg_count && next_level < SHENZHOU_IDENTIFY_LEVEL_CFG_COUNT)
		{
			ShenzhouIdentifyLevelCfg &next_cfg = m_identify_level_cfglist[next_level];
			return &next_cfg.attribute_cfglist[0];
		}
	}

	return NULL;
}

ShenzhouIdentifyItemCfg * ShenzhouWeaponConfig::GetIdentifyItemById(int item_id)
{
	for(int i = 0; i < m_identify_item_cfg_count && i < SHENZHOU_IDENTIFY_CONSUME_ITEM_MAX_COUNT; i++)
	{
		if (m_identify_comsume_item_cfglist[i].consume_item_id == item_id)
		{
			return &m_identify_comsume_item_cfglist[i];
		}
	}

	return NULL;
}

const int ShenzhouWeaponConfig::GetIdentifyConsumeItemList(int *item_list, int max_count)
{
	int identify_count = 0;
	for (int i = 0; identify_count < max_count && i < m_identify_item_cfg_count && i < SHENZHOU_IDENTIFY_CONSUME_ITEM_MAX_COUNT; i++)
	{
		if (0 != m_identify_comsume_item_cfglist[i].consume_item_id)
		{
			item_list[identify_count++] = m_identify_comsume_item_cfglist[i].consume_item_id;
		}
	}

	return identify_count;
}

int ShenzhouWeaponConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "gather_scene_id", m_other_cfg.gather_scene_id) || !SceneRegister::Instance().CheckSceneIsExist(m_other_cfg.gather_scene_id))
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "gather_id", m_other_cfg.gather_id) || m_other_cfg.gather_id <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "gather_time_s", m_other_cfg.gather_time_s) || m_other_cfg.gather_time_s <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "gather_live_time_s", m_other_cfg.gather_live_time_s) || m_other_cfg.gather_live_time_s <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "gather_num", m_other_cfg.gather_num) || m_other_cfg.gather_num < 0)
	{
		return -8;
	}

	PugiXmlNode gather_item = data_element.child("gather_item");
	if (NULL == gather_item)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "role_day_gather_num", m_other_cfg.role_day_gather_num) || m_other_cfg.role_day_gather_num <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "refresh_gather_begin_time", m_other_cfg.refresh_gather_begin_time) || m_other_cfg.refresh_gather_begin_time < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "refresh_gather_end_time", m_other_cfg.refresh_gather_end_time) || m_other_cfg.refresh_gather_end_time < 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(data_element, "buy_day_gather_num_cost", m_other_cfg.buy_day_gather_num_cost) || m_other_cfg.buy_day_gather_num_cost <= 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(data_element, "buy_day_gather_num_limit", m_other_cfg.buy_day_gather_num_limit) || m_other_cfg.buy_day_gather_num_limit < 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(data_element, "box_help_num_limit", m_other_cfg.box_help_num_limit) || m_other_cfg.box_help_num_limit < 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(data_element, "box_free_times", m_other_cfg.box_free_times) || m_other_cfg.box_free_times < 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(data_element, "box_free_times_cd", m_other_cfg.box_free_times_cd) || m_other_cfg.box_free_times_cd < 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(data_element, "skip_gather_consume", m_other_cfg.skip_gather_consume) || m_other_cfg.skip_gather_consume < 0)
	{
		return -22;
	}

	if (!PugiGetSubNodeValue(data_element, "skip_gather_level_limit", m_other_cfg.skip_gather_level_limit) || m_other_cfg.skip_gather_level_limit < 0)
	{
		return -23;
	}

	if (!PugiGetSubNodeValue(data_element, "free_xilian_times", m_other_cfg.free_xilian_times) || m_other_cfg.free_xilian_times < 0)
	{
		return -24;
	}

	if (!PugiGetSubNodeValue(data_element, "min_shenzhouweapon_score", m_other_cfg.min_shenzhouweapon_score) || m_other_cfg.min_shenzhouweapon_score < 0)
	{
		return -25;
	}

	if (!PugiGetSubNodeValue(data_element, "max_shenzhouweapon_score", m_other_cfg.max_shenzhouweapon_score) || m_other_cfg.max_shenzhouweapon_score < 0)
	{
		return -26;
	}

	if (!PugiGetSubNodeValue(data_element, "replacement_id", m_other_cfg.replacement_id) || (m_other_cfg.replacement_id > 0 && NULL == ITEMPOOL->GetItem(m_other_cfg.replacement_id)))
	{
		return -27;
	}

	if (!PugiGetSubNodeValue(data_element, "consume_replacement", m_other_cfg.consume_replacement) || m_other_cfg.consume_replacement < 0)
	{
		return -28;
	}

	if (!PugiGetSubNodeValue(data_element, "open_box_10_use_itemid", m_other_cfg.open_box_10_use_itemid) || (m_other_cfg.open_box_10_use_itemid > 0 && NULL == ITEMPOOL->GetItem(m_other_cfg.open_box_10_use_itemid)))
	{
		return -29;
	}

	if (!PugiGetSubNodeValue(data_element, "open_box_10_use_item_num", m_other_cfg.open_box_10_use_item_num) || m_other_cfg.open_box_10_use_item_num < 0)
	{
		return -30;
	}

	if (!PugiGetSubNodeValue(data_element, "can_use_item", m_other_cfg.can_use_item))
	{
		return -31;
	}

	return 0;
}

int ShenzhouWeaponConfig::InitGatherPointsCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (static_cast<int>(m_gather_point_list.size()) >= SHENZHOU_WEAPON_MAX_MELT_GATHER_NUM)
		{
			return -200;
		}

		int x_pos = 0;
		int y_pos = 0;

		if (!PugiGetSubNodeValue(data_element, "x_pos", x_pos) || x_pos <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "y_pos", y_pos) || y_pos <= 0)
		{
			return -2;
		}

		m_gather_point_list.push_back(Posi(x_pos, y_pos));

		data_element = data_element.next_sibling();
	}

	if (m_gather_point_list.size() <= 0)
	{
		return -3;
	}

	return 0;
}

int ShenzhouWeaponConfig::InitGatherRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_gather_reward_count_max = 0;
	while (!data_element.empty())
	{
		if (m_gather_reward_count_max < 0 || m_gather_reward_count_max >= SHENZHOU_MAX_GATHER_REWARD_COUNT)
		{
			return -200;
		}

		ShenzhouGatherRewardCfg &reward_cfg = m_gather_reward_list[m_gather_reward_count_max];

		if (!PugiGetSubNodeValue(data_element, "gain_pro", reward_cfg.gain_pro) || reward_cfg.gain_pro <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_type", reward_cfg.reward_type) || reward_cfg.reward_type < NORMAL_GAHTER_REWARD || reward_cfg.reward_type > UNIQUE_GATHER_REWARD)
		{
			return -2;
		}

		{
			PugiXmlNode element = data_element.child("gather_reward_list");
			if (element.empty())
			{
				return -3;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("gather_reward");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -4;
				}

				if (!reward_cfg.gather_reward[i].ReadConfig(item_element))
				{
					return 100 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		m_gather_reward_count_max++;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitExchangeIdentifyExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_exchange_identify_exp_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_exchange_identify_exp_cfg_count >= SHENZHOU_EXCHANGE_IDENTIFY_EXP_CFG_COUNT)
		{
			return 1000;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_exchange_identify_exp_cfg_count)
		{
			return -1;
		}

		ShenzhouExchangeIdentifyExpCfg &xiuwei_cfg = m_exchange_identify_exp_list[seq];
		xiuwei_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "consume_gold", xiuwei_cfg.consume_gold) || xiuwei_cfg.consume_gold <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_exp", xiuwei_cfg.reward_exp) || xiuwei_cfg.reward_exp <= 0)
		{
			return -3;
		}

		++ m_exchange_identify_exp_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}
int ShenzhouWeaponConfig::InitIdentifyLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_identify_level_cfg_count = 0;

	while (!data_element.empty())
	{
		int level = 0;
		if (m_identify_level_cfg_count >= SHENZHOU_IDENTIFY_LEVEL_CFG_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "level", level) || level != m_identify_level_cfg_count)
		{
			return -2;
		}

		ShenzhouIdentifyLevelCfg &level_cfg = m_identify_level_cfglist[level];
		level_cfg.level = level;

		if (level_cfg.star_max_level > SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT)
		{
			return -3;
		}

		int star_level = 0;
		if (!PugiGetSubNodeValue(data_element, "star_level", star_level) || star_level != level_cfg.star_max_level)
		{
			return -4;
		}

		ShenzhouIdentifyLevelCfg::AttributeConfig &sub_cfg = level_cfg.attribute_cfglist[star_level];
		sub_cfg.star_level = star_level;

		if (level_cfg.star_max_level < SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT)
		{
			level_cfg.star_max_level++;
		}
		else if (level_cfg.star_max_level == SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT)
		{
			m_identify_level_cfg_count++;
		}

		if (!PugiGetSubNodeValue(data_element, "need_exp", sub_cfg.upgrade_need_exp) || sub_cfg.upgrade_need_exp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", sub_cfg.maxhp) || sub_cfg.maxhp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", sub_cfg.gongji) || sub_cfg.gongji < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", sub_cfg.fangyu) || sub_cfg.fangyu < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", sub_cfg.mingzhong) || sub_cfg.mingzhong < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", sub_cfg.shanbi) || sub_cfg.shanbi < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", sub_cfg.baoji) || sub_cfg.baoji < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", sub_cfg.jianren) || sub_cfg.jianren < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "is_notice", sub_cfg.is_notice) || sub_cfg.is_notice < 0)
		{
			return -13;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitIdentifyItemCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_identify_item_cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_identify_item_cfg_count < 0 || m_identify_item_cfg_count >= SHENZHOU_IDENTIFY_CONSUME_ITEM_MAX_COUNT)
		{
			return -1;
		}

		int item_id = 0;
		if (!PugiGetSubNodeValue(data_element, "consume_item_id", item_id) || item_id < 0 || NULL == ITEMPOOL->GetItem(item_id))
		{
			return -2;
		}

		ShenzhouIdentifyItemCfg &cfg = m_identify_comsume_item_cfglist[m_identify_item_cfg_count];
		cfg.consume_item_id = item_id;

		if (!PugiGetSubNodeValue(data_element, "identify_suc_pro", cfg.identify_suc_pro) || cfg.identify_suc_pro < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "fail_jinghua", cfg.fail_jinghua) || cfg.fail_jinghua < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "identify_exp", cfg.identify_exp) || cfg.identify_exp < 0)
		{
			return -5;
		}

		m_identify_item_cfg_count++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitIdentifyRewardWeightCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int item_id = 0;
		if (!PugiGetSubNodeValue(data_element, "consume_item_id", item_id) || item_id < 0 || NULL == ITEMPOOL->GetItem(item_id))
		{
			return -1;
		}

		ShenzhouIdentifyItemCfg *identify_item_cfg = this->GetIdentifyItemById(item_id);
		if (NULL == identify_item_cfg)
		{
			return -2;
		}

		std::map<int, ShenzhouIdentifyRewardCfg> &cfg_map = identify_item_cfg->reward_pool_map;

		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index <= 0 || cfg_map.find(index) != cfg_map.end())
		{
			return -2;
		}

		ShenzhouIdentifyRewardCfg cfg;
		cfg.index = index;

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "identify_level", cfg.identify_level) || cfg.identify_level < 0 || cfg.identify_level >= SHENZHOU_IDENTIFY_LEVEL_CFG_COUNT)
		{
			return -5;
		}

		{
			PugiXmlNode element = data_element.child("reward_item_list");
			if (element.empty())
			{
				return -6;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -7;
				}

				if (!cfg.reward_item[i].ReadConfig(item_element))
				{
					return 100 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		cfg_map[index] = cfg;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitSlotAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int hunqi = 0;
		int slot = 0;
		if (!PugiGetSubNodeValue(data_element, "hunqi", hunqi) || hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "slot", slot) || slot < 0 || slot >= SHENZHOU_WEAPON_SLOT_COUNT)
		{
			return -2;
		}

		ShenzhouSlotAttrCfg &slot_cfg = m_slot_attr_cfglist[hunqi][slot];
		
		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level > ShenzhouSlotAttrCfg::CFG_MAX_COUNT)
		{
			return -3;
		}

		ShenzhouSlotAttrCfg::LevelItemConfig &level_cfg = slot_cfg.cfg_list[level];
		level_cfg.hunqi_id = hunqi;
		level_cfg.slot = slot;
		level_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "up_level_jinghua", level_cfg.up_level_jinghua) || level_cfg.up_level_jinghua < 0)
		{
			return -4;
		}

		{
			PugiXmlNode item_element = data_element.child("up_level_item");
			if (item_element.empty())
			{
				return -5;
			}

			if (!level_cfg.up_level_item.ReadConfig(item_element) && level_cfg.up_level_item.item_id != 0)
			{
				return -6;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -106;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitWeaponSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int hunqi = 0;
		if (!PugiGetSubNodeValue(data_element, "hunqi", hunqi) || hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT)
		{
			return -1;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level > ShenzhouSlotAttrCfg::CFG_MAX_COUNT)
		{
			return -3;
		}

		ShenzhouSkillCfg &cfg = m_weapon_skill_cfg[hunqi][level];
		if (cfg.level != 0)
		{
			return -4;
		}

		cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "skill_type", cfg.skill_type) || cfg.skill_type < 0 || cfg.skill_type >= WEAPON_SKILL_MAX_TYPE)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "param_1", cfg.param_1) || cfg.param_1 < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "param_2", cfg.param_2) || cfg.param_2 < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "param_3", cfg.param_3) || cfg.param_3 < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "param_4", cfg.param_4) || cfg.param_4 < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "param_5", cfg.param_5) || cfg.param_5 < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "is_notice", cfg.is_notice) || cfg.is_notice < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_cap_per", cfg.skill_cap_per) || cfg.skill_cap_per < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "capbility", cfg.capbility) || cfg.capbility < 0)
		{
			return -13;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitBoxCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "box_id", m_shen_zhou_box_cfg.box_id) || m_shen_zhou_box_cfg.box_id < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "cousume_diamond", m_shen_zhou_box_cfg.cousume_diamond) || m_shen_zhou_box_cfg.cousume_diamond < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "cousume_diamond10", m_shen_zhou_box_cfg.cousume_diamond10) || m_shen_zhou_box_cfg.cousume_diamond10 < 0)
		{
			return -10;
		}

		{
			PugiXmlNode item_element = data_element.child("help_reward_item");
			if (item_element.empty())
			{
				return -3;
			}

			if (!m_shen_zhou_box_cfg.help_reward_item.ReadConfig(item_element) || NULL == ITEMPOOL->GetItem(m_shen_zhou_box_cfg.help_reward_item.item_id))
			{
				return -4;
			}
		}
		
		char temp_str[32] = {0};
		for (int i = 1; i <= SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT + 1; i++)
		{	
			int open_reward_num = 0;
			sprintf(temp_str, "open_reward%d", i);
			if (!PugiGetSubNodeValue(data_element, temp_str, open_reward_num) || open_reward_num <= 0)
			{
				return -4 - i;
			}
			m_shen_zhou_box_cfg.open_reward_num_vec.push_back(open_reward_num);
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::GetBoxRewardNum(int help_num)
{
	if (help_num < 0 || help_num > SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT)
	{
		return 0;
	}
	return m_shen_zhou_box_cfg.open_reward_num_vec[help_num];
}

const ShenzhouHunYinCfg * ShenzhouWeaponConfig::GetHunYinCfg(int hunyin_id)
{
	if (hunyin_id <= 0 || NULL == ITEMPOOL->GetItem(hunyin_id)) return NULL;

	std::map<int, ShenzhouHunYinCfg>::iterator it = m_hunyin_map.find(hunyin_id);
	if (it != m_hunyin_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const ShenzhouLingshuLevelCfg::LingshuLevelCfg * ShenzhouWeaponConfig::GetLingshuLevelAttrCfg(int hunqi_id, int hunyin_slot, int lingshu_level)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return NULL;

	if (hunyin_slot < 0 || hunyin_slot >= SHENZHOU_HUNYIN_MAX_SLOT) return NULL;

	if (lingshu_level < 0 || lingshu_level > SHENZHOU_HUNYIN_LINGSHU_MAX_LEVEL) return NULL;

	ShenzhouLingshuLevelCfg::LingshuLevelCfg &level_cfg = m_hunyin_lingshu_level_cfg[hunqi_id].linshu_level_cfg[hunyin_slot][lingshu_level];

	if (level_cfg.slot_level != lingshu_level) return NULL;

	return &level_cfg;
}

const ShenzhouHunyinSuitCfg * ShenzhouWeaponConfig::GetHunyinSuitCfg(int hunqi_id, int suit_color)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return NULL;

	if (suit_color <= HUNYIN_COLOR_COLOR_INVALID || suit_color >= HUNYIN_COLOR_MAX) return NULL;

	ShenzhouHunyinSuitCfg &suit_cfg = m_hunyin_suit_cfg[hunqi_id][suit_color];
	if (suit_cfg.hunqi_id == hunqi_id && suit_cfg.suit_color == suit_color)
	{
		return &suit_cfg;
	}

	return NULL;
}

const ShenzhouHunyinSoltOpenCfg * ShenzhouWeaponConfig::GetHunyinSlotOpenCfg(int hunqi, int hunyin_slot)
{
	if (hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT) return NULL;

	if (hunyin_slot < 0 || hunyin_slot >= SHENZHOU_HUNYIN_MAX_SLOT) return NULL;

	ShenzhouHunyinSoltOpenCfg &cfg = m_hunyin_slot_open_cfg[hunqi][hunyin_slot];
	if (cfg.hunqi == hunqi && cfg.hunyin_slot == hunyin_slot)
	{
		return &cfg;
	}

	return NULL;
}

const SZXilianOpenCfg * ShenzhouWeaponConfig::GetXilianOpenCfg(int hunqi, int slot)
{
	if (hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT) return NULL;

	if (slot < 0 || slot >= SHENZHOU_HUNYIN_MAX_SLOT) return NULL;

	SZXilianOpenCfg &cfg = m_xilian_open_cfg[hunqi][slot];
	if (cfg.hunqi_id == hunqi && cfg.slot_id == slot)
	{
		return &cfg;
	}

	return NULL;
}

const SZXilianLockCfg * ShenzhouWeaponConfig::GetXilianLockCfg(int lock_num)
{
	if (lock_num < 0 || lock_num >= SHENZHOU_HUNYIN_MAX_SLOT) return NULL;

	SZXilianLockCfg &cfg = m_xilian_lock_cfg[lock_num];
	if (cfg.lock_num == lock_num)
	{
		return &cfg;
	}

	return NULL;
}

const int ShenzhouWeaponConfig::GetXilianConsuemeTotalWeight(int color)
{
	if (color <= XL_COUMME_COLOR_INVALID || color >= XL_COUMME_COLOR_MAX) return 0;

	return m_xilian_consuem_total_weight[color];
}

const SZXilianComsumeCfg * ShenzhouWeaponConfig::GetXilianConsumeCfg(int color)
{
	if (color <= XL_COUMME_COLOR_INVALID || color >= XL_COUMME_COLOR_MAX) return NULL;

	SZXilianComsumeCfg &cfg = m_xilian_consume_cfg[color];
	if (cfg.comsume_color == color)
	{
		return &cfg;
	}

	return NULL;
}

const int ShenzhouWeaponConfig::GetXilianShuxingTotalWeight(int hunqi)
{
	if (hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT) return 0;

	return m_xilian_shuxing_total_weight[hunqi];
}

const SZXilianShuxingCfg * ShenzhouWeaponConfig::GetXilianShuxingCfg(int hunqi, int shuxing_type)
{
	if (hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT) return NULL;

	std::map<int, SZXilianShuxingCfg> &shuxing_map = m_xilian_shuxing_map[hunqi];
	std::map<int, SZXilianShuxingCfg>::iterator iter = shuxing_map.find(shuxing_type);
	if (iter != shuxing_map.end() && iter->second.hunqi_id == hunqi && iter->second.shuxing_type == shuxing_type)
	{
		return &iter->second;
	}

	return NULL;
}

const SZXilianSuitCfg * ShenzhouWeaponConfig::GetXilianSuitCfg(int hunqi, int star_count)
{
	if (hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT) return NULL;

	for (int i = m_xilian_suit_count[hunqi]; i >= 0; i--)
	{
		SZXilianSuitCfg &cfg = m_xilian_suit_cfg[hunqi][i];
		if (star_count >= cfg.need_start_count && cfg.hunqi_id == hunqi)
		{
			return &cfg;
		}
	}

	return NULL;
}

//const SZXilianLockCfg * ShenzhouWeaponConfig::GetXilianLockCfg(int consume_color, int lock_num)
//{
//	if (lock_num < 0 || lock_num > SHENZHOU_HUNYIN_MAX_SLOT) return NULL;
//
//	for (int i = 0; i < static_cast<int>(m_lock_consume_vct.size()); i++)
//	{
//		if (m_lock_consume_vct[i].consume_color == consume_color && m_lock_consume_vct[i].lock_num == lock_num)
//		{
//			return &m_lock_consume_vct[i];
//		}
//	}
//
//	return NULL;
//}


int ShenzhouWeaponConfig::InitBoxRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ShenzhouBoxRewardCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight < 0)
		{
			return -1;
		}

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -2;
			}

			if (!cfg.reward_item.ReadConfig(item_element) || NULL == ITEMPOOL->GetItem(cfg.reward_item.item_id))
			{
				return -3;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || cfg.level < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", cfg.is_broadcast))
		{
			return -5;
		}
		
		m_box_reward_total_weight += cfg.weight;
		m_box_reward_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

bool ShenzhouWeaponConfig::CheckSeq(int seq)
{
	if (seq < 0 || seq >= static_cast<int>(m_box_reward_vec.size()))
	{
		return false;
	}
	return true;
}

void ShenzhouWeaponConfig::GetBoxRandRewardByLevel(int level, std::vector<ShenzhouBoxRewardCfg> &box_reward_vec, int &all_weight)
{
	for (int i = 0; i < static_cast<int>(m_box_reward_vec.size()); i++)
	{
		if (level >= m_box_reward_vec[i].level)
		{
			all_weight += m_box_reward_vec[i].weight;
			box_reward_vec.push_back(m_box_reward_vec[i]);
		}
	}
}

int ShenzhouWeaponConfig::InitElementLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int hunqi = 0;
		int element_type = 0;
		int element_level = 0;
		if (!PugiGetSubNodeValue(data_element, "hunqi", hunqi) || hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "element_type", element_type) || element_type < 0 || element_type >= SHENZHOU_ELEMET_MAX_TYPE)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "element_level", element_level) || element_level < 0 || element_level > SHENZHOU_ELEMET_MAX_LEVEL)
		{
			return -3;
		}

		ShenzhouElementCfg::ShenzhouElementLevelCfg &level_cfg = m_element_level_cfg[hunqi].element_level_cfg[element_type][element_level];
		level_cfg.hunqi = hunqi;
		level_cfg.element_type = element_type;
		level_cfg.element_level = element_level;

		{
			PugiXmlNode item_element = data_element.child("up_level_item");
			if (item_element.empty())
			{
				return -4;
			}

			if (!level_cfg.up_level_item.ReadConfig(item_element) && level_cfg.up_level_item.item_id != 0)
			{
				return -5;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "attr_add_per", level_cfg.attr_add_per) || level_cfg.attr_add_per < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "huqi_level_limit", level_cfg.huqi_level_limit) || level_cfg.huqi_level_limit < 0)
		{
			return -107;
		}

		if (!PugiGetSubNodeValue(data_element, "is_notice", level_cfg.is_notice) || level_cfg.is_notice < 0)
		{
			return -108;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitHunYinCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int hunyin_id = 0;
		if (!PugiGetSubNodeValue(data_element, "hunyin_id", hunyin_id) || hunyin_id < 0 || NULL == ITEMPOOL->GetItem(hunyin_id) || 
			m_hunyin_map.find(hunyin_id) != m_hunyin_map.end())
		{
			return -1;
		}

		ShenzhouHunYinCfg hunyin_cfg;
		hunyin_cfg.hunyin_id = hunyin_id;

		if (!PugiGetSubNodeValue(data_element, "hunyin_color", hunyin_cfg.hunyin_color) || 
			hunyin_cfg.hunyin_color <= HUNYIN_COLOR_COLOR_INVALID || hunyin_cfg.hunyin_color >= HUNYIN_COLOR_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "inlay_slot", hunyin_cfg.inlay_slot) || 
			hunyin_cfg.inlay_slot < 0 || hunyin_cfg.inlay_slot > 8)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "discard_exp", hunyin_cfg.discard_exp) || hunyin_cfg.discard_exp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "blue_lingzhi", hunyin_cfg.blue_lingzhi) || hunyin_cfg.blue_lingzhi < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "purple_lingzhi", hunyin_cfg.purple_lingzhi) || hunyin_cfg.purple_lingzhi < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "orange_lingzhi", hunyin_cfg.orange_lingzhi) || hunyin_cfg.orange_lingzhi < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", hunyin_cfg.maxhp) || hunyin_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", hunyin_cfg.gongji) || hunyin_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", hunyin_cfg.fangyu) || hunyin_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", hunyin_cfg.mingzhong) || hunyin_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", hunyin_cfg.shanbi) || hunyin_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", hunyin_cfg.baoji) || hunyin_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", hunyin_cfg.jianren) || hunyin_cfg.jianren < 0)
		{
			return -106;
		}

		m_hunyin_map[hunyin_id] = hunyin_cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitHunyinLingshuLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int hunqi = 0;
		int hunyin_slot = 0;
		int hunyin_lingshu_level = 0;
		//static int last_hunqi_id = -1;
		if (!PugiGetSubNodeValue(data_element, "hunqi_id", hunqi) || hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "hunyin_slot", hunyin_slot) || hunyin_slot < 0 || hunyin_slot >= SHENZHOU_HUNYIN_MAX_SLOT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "slot_level", hunyin_lingshu_level) || hunyin_lingshu_level < 0 || hunyin_lingshu_level > SHENZHOU_HUNYIN_LINGSHU_MAX_LEVEL)
		{
			return -3;
		}

		ShenzhouLingshuLevelCfg::LingshuLevelCfg &level_cfg = m_hunyin_lingshu_level_cfg[hunqi].linshu_level_cfg[hunyin_slot][hunyin_lingshu_level];
		level_cfg.hunqi_id = hunqi;
		level_cfg.hunyin_slot = hunyin_slot;
		level_cfg.slot_level = hunyin_lingshu_level;

		if (!PugiGetSubNodeValue(data_element, "up_level_exp", level_cfg.up_level_exp) || level_cfg.up_level_exp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "lingshu_color", level_cfg.lingshu_color) || level_cfg.lingshu_color <= HUNYIN_COLOR_COLOR_INVALID || 
			level_cfg.lingshu_color >= HUNYIN_COLOR_MAX)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "add_per", level_cfg.add_per) || level_cfg.add_per < 0)
		{
			return -107;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitHunyinSuitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int hunqi = 0;
		int hunyin_suit_color = 0;
		if (!PugiGetSubNodeValue(data_element, "hunqi_id", hunqi) || hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "suit_color", hunyin_suit_color) || hunyin_suit_color <= HUNYIN_COLOR_COLOR_INVALID || hunyin_suit_color >= HUNYIN_COLOR_MAX)
		{
			return -2;
		}

		ShenzhouHunyinSuitCfg &suit_cfg = m_hunyin_suit_cfg[hunqi][hunyin_suit_color];
		suit_cfg.hunqi_id = hunqi;
		suit_cfg.suit_color = hunyin_suit_color;

		if (!PugiGetSubNodeValue(data_element, "maxhp", suit_cfg.maxhp) || suit_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", suit_cfg.gongji) || suit_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", suit_cfg.fangyu) || suit_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", suit_cfg.mingzhong) || suit_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", suit_cfg.shanbi) || suit_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", suit_cfg.baoji) || suit_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", suit_cfg.jianren) || suit_cfg.jianren < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "per_hunshi", suit_cfg.per_hunshi) || suit_cfg.per_hunshi < 0)
		{
			return -107;
		}

		if (!PugiGetSubNodeValue(data_element, "per_gongji", suit_cfg.per_gongji) || suit_cfg.per_gongji < 0)
		{
			return -108;
		}

		if (!PugiGetSubNodeValue(data_element, "per_maxhp", suit_cfg.per_maxhp) || suit_cfg.per_maxhp < 0)
		{
			return -109;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitNewHunyinSuitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int hunqi = 0;
		int hunyin_suit_color = 0;
		if (!PugiGetSubNodeValue(data_element, "hunqi_id", hunqi) || hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "suit_color", hunyin_suit_color) || hunyin_suit_color <= HUNYIN_COLOR_COLOR_INVALID || hunyin_suit_color >= HUNYIN_COLOR_MAX)
		{
			return -2;
		}

		int cfg_count = m_new_hunyin_suit_cfg[hunqi][hunyin_suit_color].cfg_count;

		if (cfg_count < 0 || cfg_count >= SHENZHOU_NEW_SUIT_MAX_COUNT)
		{
			return -999;
		}

		ShenzhouHunyinSuitCfg &suit_cfg = m_new_hunyin_suit_cfg[hunqi][hunyin_suit_color].attr_cfg[cfg_count];

		suit_cfg.hunqi_id = hunqi;
		suit_cfg.suit_color = hunyin_suit_color;

		if (!PugiGetSubNodeValue(data_element, "maxhp", suit_cfg.maxhp) || suit_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", suit_cfg.gongji) || suit_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", suit_cfg.fangyu) || suit_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", suit_cfg.mingzhong) || suit_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", suit_cfg.shanbi) || suit_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", suit_cfg.baoji) || suit_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", suit_cfg.jianren) || suit_cfg.jianren < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "per_hunshi", suit_cfg.per_hunshi) || suit_cfg.per_hunshi < 0)
		{
			return -107;
		}

		if (!PugiGetSubNodeValue(data_element, "per_gongji", suit_cfg.per_gongji) || suit_cfg.per_gongji < 0)
		{
			return -108;
		}

		if (!PugiGetSubNodeValue(data_element, "per_maxhp", suit_cfg.per_maxhp) || suit_cfg.per_maxhp < 0)
		{
			return -109;
		}

		if (!PugiGetSubNodeValue(data_element, "same_qulitily_count", suit_cfg.same_qulitily_count) || suit_cfg.same_qulitily_count < 0)
		{
			return -110;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi_per", suit_cfg.shanbi_per) || suit_cfg.shanbi_per < 0)
		{
			return -111;
		}

		if (!PugiGetSubNodeValue(data_element, "per_mianshang", suit_cfg.per_mianshang) || suit_cfg.per_mianshang < 0)
		{
			return -112;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_jianshang_per", suit_cfg.skill_jianshang_per) || suit_cfg.skill_jianshang_per < 0)
		{
			return -113;
		}

		if (!PugiGetSubNodeValue(data_element, "per_pofang", suit_cfg.per_pofang) || suit_cfg.per_pofang < 0)
		{
			return -114;
		}

		++m_new_hunyin_suit_cfg[hunqi][hunyin_suit_color].cfg_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitHunyinSlotOpenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int hunyin_slot = -1;
		int hunqi_level = 0;
		int hunqi = -1;
		int glod = 0;
		int limit_1 = 0;
		int limit_2 = 0;
		if (!PugiGetSubNodeValue(data_element, "hunqi", hunqi) || hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "hunyin_slot", hunyin_slot) || hunyin_slot < 0 || hunyin_slot >= SHENZHOU_HUNYIN_MAX_SLOT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "open_hunqi_level", hunqi_level) || hunqi_level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "yuanbao", glod) || glod < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "hunqi_qianzhi", limit_1) || limit_1 < 0 || limit_1 >= SHENZHOU_WEAPON_COUNT)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "hunchao_qianzhi", limit_2) || limit_2 < 0 || limit_2 >= SHENZHOU_HUNYIN_MAX_SLOT)
		{
			return -6;
		}

		ShenzhouHunyinSoltOpenCfg &open_cfg = m_hunyin_slot_open_cfg[hunqi][hunyin_slot];
		open_cfg.hunqi = hunqi;
		open_cfg.hunyin_slot = hunyin_slot;
		open_cfg.open_hunqi_level = hunqi_level;
		open_cfg.gold = glod;
		open_cfg.limit_1 = limit_1;
		open_cfg.limit_2 = limit_2;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitBoxRewardCountCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		BoxRewardCountCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || cfg.level < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "open_reward1", cfg.open_reward1) || cfg.open_reward1 < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "open_reward2", cfg.open_reward2) || cfg.open_reward2 < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "open_reward3", cfg.open_reward3) || cfg.open_reward3 < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "open_reward4", cfg.open_reward4) || cfg.open_reward4 < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "open_reward5", cfg.open_reward5) || cfg.open_reward5 < 0)
		{
			return -6;
		}

		m_box_reward_count_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitXilianOpenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int hunqi = 0;
		int slot = 0;
		if (!PugiGetSubNodeValue(data_element, "hunqi_id", hunqi) || hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "slot_id", slot) || slot < 0 || slot >= SHENZHOU_HUNYIN_MAX_SLOT)
		{
			return -2;
		}

		SZXilianOpenCfg &slot_cfg = m_xilian_open_cfg[hunqi][slot];
		slot_cfg.hunqi_id = hunqi;
		slot_cfg.slot_id = slot;

		if (!PugiGetSubNodeValue(data_element, "lingshu_level_limit", slot_cfg.lingshu_level_limit) || slot_cfg.lingshu_level_limit < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_cost", slot_cfg.gold_cost) || slot_cfg.gold_cost < 0)
		{
			return -101;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitXilianLockCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int lock_num = 0;
		if (!PugiGetSubNodeValue(data_element, "lock_num", lock_num) || lock_num < 0 || lock_num >= SHENZHOU_HUNYIN_MAX_SLOT)
		{
			return -1;
		}

		SZXilianLockCfg &cfg = m_xilian_lock_cfg[lock_num];
		cfg.lock_num = lock_num;

		{
			PugiXmlNode item_element = data_element.child("lock_comsume_item");
			if (item_element.empty())
			{
				return -4;
			}

			if (!cfg.lock_comsume_item.ReadConfig(item_element) && cfg.lock_comsume_item.item_id != 0)
			{
				return -5;
			}
		}

		data_element = data_element.next_sibling();

		//int consume_color = 0;
		//if (!PugiGetSubNodeValue(data_element, "consume_color", consume_color) || consume_color < 0)
		//{
		//	return -1;
		//}

		//SZXilianLockCfg lock_consume_cfg;
		//lock_consume_cfg.consume_color = consume_color;

		////int lock_num = 0;
		//if (!PugiGetSubNodeValue(data_element, "lock_num", lock_consume_cfg.lock_num) || lock_consume_cfg.lock_num < 0 || lock_consume_cfg.lock_num > SHENZHOU_HUNYIN_MAX_SLOT)
		//{
		//	return -2;
		//}

		//{
		//	PugiXmlNode item_element = data_element.child("lock_comsume_item");
		//	if (item_element.empty())
		//	{
		//		return -3;
		//	}

		//	if (!lock_consume_cfg.lock_comsume_item.ReadConfig(item_element) && lock_consume_cfg.lock_comsume_item.item_id != 0)
		//	{
		//		return -4;
		//	}
		//}

		//m_lock_consume_vct.push_back(lock_consume_cfg);

		//data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitXilianConsumeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	memset(m_xilian_consuem_total_weight, 0, sizeof(m_xilian_consuem_total_weight));

	while (!data_element.empty())
	{
		int color = 0;
		if (!PugiGetSubNodeValue(data_element, "comsume_color", color) || color <= XL_COUMME_COLOR_INVALID || color >= XL_COUMME_COLOR_MAX)
		{
			return -1;
		}

		int &xilian_total_weight = m_xilian_consuem_total_weight[color];
		SZXilianComsumeCfg &cfg = m_xilian_consume_cfg[color];
		cfg.comsume_color = color;

		{
			PugiXmlNode item_element = data_element.child("consume_item");
			if (item_element.empty())
			{
				return -4;
			}

			if (!cfg.consume_item.ReadConfig(item_element) && cfg.consume_item.item_id != 0)
			{
				return -5;
			}
		}

		char tem_str[64] = {0};
		for (int i = 0; i < SHENZHOU_XILIAN_MAX_STAR_COUNT; i++)
		{
			sprintf(tem_str, "star_weight_%d", i);
			if (!PugiGetSubNodeValue(data_element, tem_str, cfg.star_weight[i]) || cfg.star_weight[i] < 0)
			{
				return -200;
			}

			xilian_total_weight += cfg.star_weight[i];
		}

		if (xilian_total_weight <= 0)
		{
			return -300;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::InitXilianShuxingCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	memset(m_xilian_shuxing_total_weight, 0, sizeof(m_xilian_shuxing_total_weight));

	while (!data_element.empty())
	{
		int hunqi_id = -1;
		if (!PugiGetSubNodeValue(data_element, "hunqi_id", hunqi_id) || hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT)
		{
			return -1;
		}

		int &shuxing_total_weight = m_xilian_shuxing_total_weight[hunqi_id];
		std::map<int, SZXilianShuxingCfg> &cfg_map = m_xilian_shuxing_map[hunqi_id];

		int shuxing_type = XL_SHUXING_TYPE_INVALID;
		if (!PugiGetSubNodeValue(data_element, "shuxing_type", shuxing_type) || shuxing_type <= XL_SHUXING_TYPE_INVALID || shuxing_type >= XL_SHUXING_TYPE_MAX)
		{
			return -2;
		}

		if (cfg_map.find(shuxing_type) != cfg_map.end())
		{
			return -3;
		}

		SZXilianShuxingCfg &cfg = cfg_map[shuxing_type];
		cfg.hunqi_id = hunqi_id;
		cfg.shuxing_type = shuxing_type;

		if (!PugiGetSubNodeValue(data_element, "type_weight", cfg.type_weight) || cfg.type_weight < 0)
		{
			return -4;
		}
		shuxing_total_weight += cfg.type_weight;

		char tem_str[64] = { 0 };
		for (int i = 0; i < SHENZHOU_XILIAN_MAX_STAR_COUNT; i++)
		{
			sprintf(tem_str, "star_min_%d", i);
			if (!PugiGetSubNodeValue(data_element, tem_str, cfg.star_min[i]) || cfg.star_min[i] < 0)
			{
				return -200;
			}

			sprintf(tem_str, "star_max_%d", i);
			if (!PugiGetSubNodeValue(data_element, tem_str, cfg.star_max[i]) || cfg.star_max[i] < 0)
			{
				return -200;
			}
		}

		data_element = data_element.next_sibling();
	}

	//for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	//{
	//	if (m_xilian_shuxing_total_weight[i] <= 0)
	//	{
	//		return -3000;
	//	}
	//}

	return 0;
}

int ShenzhouWeaponConfig::InitXilianSuitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	memset(m_xilian_suit_count, 0, sizeof(m_xilian_suit_count));

	while (!data_element.empty())
	{
		int hunqi_id = -1;
		if (!PugiGetSubNodeValue(data_element, "hunqi_id", hunqi_id) || hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT)
		{
			return -1;
		}

		int &xilian_suit_count = m_xilian_suit_count[hunqi_id];
		if (xilian_suit_count < 0 || xilian_suit_count >= SHENZHOU_XILIAN_SUIT_MAX_COUNT)
		{
			return -2;
		}

		SZXilianSuitCfg &cfg = m_xilian_suit_cfg[hunqi_id][xilian_suit_count];

		if (!PugiGetSubNodeValue(data_element, "need_start_count", cfg.need_start_count) || cfg.need_start_count < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -200;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -201;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -202;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -203;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -204;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -205;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -200;
		}

		if (!PugiGetSubNodeValue(data_element, "add_per", cfg.add_per) || cfg.add_per < 0)
		{
			return -206;
		}

		++xilian_suit_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenzhouWeaponConfig::GetBoxRewardCountByLevel(int level, int help_num)
{
	for (int i = static_cast<int>(m_box_reward_count_vec.size()) - 1; i >= 0; i--)
	{
		if (level >= m_box_reward_count_vec[i].level)
		{
			if (0 == help_num)
			{
				return m_box_reward_count_vec[i].open_reward1;
			}
			else if (1 == help_num)
			{
				return m_box_reward_count_vec[i].open_reward2;
			}
			else if (2 == help_num)
			{
				return m_box_reward_count_vec[i].open_reward3;
			}
			else if (3 == help_num)
			{
				return m_box_reward_count_vec[i].open_reward4;
			}
			else if (4 == help_num)
			{
				return m_box_reward_count_vec[i].open_reward5;
			}
		}
	}
	return 0;
}

const ShenzhouHunyinSuitCfg * ShenzhouWeaponConfig::GetSuitCfg(int hunqi, int color, int count)
{
	if (hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT) return NULL;
	if (color <= HUNYIN_COLOR_COLOR_INVALID || color >= HUNYIN_COLOR_MAX) return NULL;

	if (m_new_hunyin_suit_cfg[hunqi][color].cfg_count > 0)
	{
		for (int i = m_new_hunyin_suit_cfg[hunqi][color].cfg_count - 1; i >= 0 && i < SHENZHOU_NEW_SUIT_MAX_COUNT; i--)
		{
			if (count >= m_new_hunyin_suit_cfg[hunqi][color].attr_cfg[i].same_qulitily_count)
			{
				return &m_new_hunyin_suit_cfg[hunqi][color].attr_cfg[i];
			}
		}
	}

	return NULL;
}