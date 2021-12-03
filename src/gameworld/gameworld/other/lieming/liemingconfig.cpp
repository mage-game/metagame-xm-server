#include "item/itempool.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/attribute.hpp"
#include "servercommon/servercommon.h"
#include "liemingconfig.hpp"
#include "other/lieming/liemingmanager.h"

LieMingConfig::LieMingConfig() : m_max_hunshou_id(0)
{
	memset(m_hunshou_type_attr_map, 0, sizeof(m_hunshou_type_attr_map));

	UNSTD_STATIC_CHECK(8 == LIEMING_HUNSHOU_TYPE_MAX_COUNT);			

	m_hunshou_type_attr_map[0] = CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
	m_hunshou_type_attr_map[1] = CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
	m_hunshou_type_attr_map[2] = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
	m_hunshou_type_attr_map[3] = CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
	m_hunshou_type_attr_map[4] = CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
	m_hunshou_type_attr_map[5] = CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
	m_hunshou_type_attr_map[6] = CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
	m_hunshou_type_attr_map[7] = CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI;
}

LieMingConfig::~LieMingConfig()
{

}

bool LieMingConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "LieMingConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// ÆäËû
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
		// »êÊÞÅäÖÃ
		PugiXmlNode root_element = RootElement.child("hunshou");
		if (root_element.empty())
		{
			*err = configname + ": no [hunshou].";
			return false;
		}

		iRet = this->InitHunshouCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitHunshouCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// »êÊÞ¾­Ñé
		PugiXmlNode root_element = RootElement.child("hunshou_exp");
		if (root_element.empty())
		{
			*err = configname + ": no [hunshou_exp].";
			return false;
		}

		iRet = this->InitHunshouExpCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitHunshouExpCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ³é»êÅäÖÃ
		PugiXmlNode root_element = RootElement.child("chouhun");
		if (root_element.empty())
		{
			*err = configname + ": no [chouhun].";
			return false;
		}

		iRet = this->InitChouhunCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChouhunCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// Ãü¸ñ¼¤»îÅäÖÃ
		PugiXmlNode root_element = RootElement.child("hunge_activity_condition");
		if (root_element.empty())
		{
			*err = configname + ": no [hunge_activity_condition].";
			return false;
		}

		iRet = this->InitLieMingJihuoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLieMingJihuoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

//const LieMingHunshouConfig * LieMingConfig::GetHunshouCfg(int hunshou_id)
//{
//	if (hunshou_id > 0 && hunshou_id <= m_max_hunshou_id)
//	{
//		return &m_hunshou_cfglist[hunshou_id];
//	}
//
//	return NULL;
//}

const LieMingHunshouConfig * LieMingConfig::GetHunshouCfg(ItemID item_id)
{
	for (auto &i : m_hunshou_cfglist)
	{
		if (i.item_id == item_id)
		{
			return &i;
		}
	}

	return NULL;
}

const LieMingHunshouLevelConfigItem * LieMingConfig::GetHunshouLevelCfg(int color, int level)
{
	if (color <= LIEMING_HUNSHOU_COLOR_INVALID || color >= LIEMING_HUNSHOU_COLOR_MAX || level < 0 || level > LIEMING_HUNSHOU_MAX_LEVEL)
	{
		return NULL;
	}

	if (level > m_hunshou_level_cfglist[color].max_level)
	{
		return NULL;
	}

	return &m_hunshou_level_cfglist[color].level_cfglist[level];
}

const LieMingChouhunConfig * LieMingConfig::GetChouhunCfg(int chouhun_color)
{
	if (chouhun_color < 0 || chouhun_color >=  LIEMING_CHOUHUN_COLOR_MAX)
	{
		return NULL;
	}

	return &m_chouhun_cfglist[chouhun_color];
}

long long LieMingConfig::CalSellExp(int hunshou_id, int level, long long curr_exp)
{
	if (LIEMING_HUNSHOU_EXP_ID == hunshou_id)
	{
		return m_other_cfg.exp_hunshou_exp_val;
	}
	else
	{
		const LieMingHunshouConfig *hunshou_cfg = this->GetHunshouCfg(hunshou_id);
		if (NULL != hunshou_cfg)
		{
			const LieMingHunshouLevelConfigItem *level_cfg_item = this->GetHunshouLevelCfg(hunshou_cfg->hunshou_color, level);
			if (NULL != level_cfg_item)
			{
				long long sell_exp = static_cast<long long>((level_cfg_item->total_exp + curr_exp) * 0.01 * m_other_cfg.hunshou_exp_discount_rate);
				return sell_exp;
			}
		}
	}

	return -1;
}

const LieMingChouhunHunshouWeightCfg * LieMingConfig::GetChouhunHunshouWeightCfg(int hunshou_color)
{
	if (hunshou_color <= LIEMING_HUNSHOU_COLOR_INVALID || hunshou_color >= LIEMING_HUNSHOU_COLOR_MAX)
	{
		return NULL;
	}

	return &m_chouhun_hunshou_weight_cfglist[hunshou_color];
}

int LieMingConfig::GetHunshouMaxLevel(int hunshou_color)
{
	if (hunshou_color <= LIEMING_HUNSHOU_COLOR_INVALID || hunshou_color >= LIEMING_HUNSHOU_COLOR_MAX)
	{
		return -1;
	}

	return m_hunshou_level_cfglist[hunshou_color].max_level;
}

bool LieMingConfig::GetHunshouAddAttr(ItemID item_id, int hunshou_level, int *attr_type, Attribute *attr_val)
{
	if (NULL == attr_type || NULL == attr_val) 
	{
		return false;
	}

	//const LieMingHunshouConfig *hunshou_cfg = this->GetHunshouCfg(hunshou_id);
	const LieMingHunshouConfig *hunshou_cfg = this->GetHunshouCfg(item_id);
	if (NULL != hunshou_cfg)
	{
		const LieMingHunshouLevelConfigItem *level_cfg_item = this->GetHunshouLevelCfg(hunshou_cfg->hunshou_color, hunshou_level);
		if (NULL != level_cfg_item)
		{
			if (hunshou_cfg->hunshou_type >= 0 && hunshou_cfg->hunshou_type < LIEMING_HUNSHOU_TYPE_MAX_COUNT)
			{
				*attr_type = m_hunshou_type_attr_map[hunshou_cfg->hunshou_type];
				*attr_val = level_cfg_item->attr_val_list[hunshou_cfg->hunshou_type];

				return true;
			}
		}
	}

	return false;
}

void LieMingConfig::ActiveSoltByRoleLevel(Role *role, int level)
{
	if (level < 0) return;
	
	for(int i = 0; i < LIEMING_FUHUN_SLOT_COUNT ;i++)
	{
		if (level >= m_jihuo_cfg[i].role_level)
		{
			role->GetLieMingMgr()->ActiveMinggeSolt(m_jihuo_cfg[i].active_mingge_count);
			role->GetLieMingMgr()->SendEquipInfo();
		}
	}
	
}

int LieMingConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_hunshou_exp_val", m_other_cfg.exp_hunshou_exp_val) || m_other_cfg.exp_hunshou_exp_val <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "super_chouhun_price", m_other_cfg.super_chouhun_price) || m_other_cfg.super_chouhun_price <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "hunshou_exp_discount_rate", m_other_cfg.hunshou_exp_discount_rate) || m_other_cfg.hunshou_exp_discount_rate <= 0)
	{
		return -4;
	}

	return 0;
}

int LieMingConfig::InitHunshouCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_max_hunshou_id >= LIEMING_HUNSHOU_MAX_ID)
		{
			return -1;
		}

		int hunshou_id = 0;
		if (!PugiGetSubNodeValue(data_element, "hunshou_id", hunshou_id) || hunshou_id != m_max_hunshou_id + 1)
		{
			return -2;
		}
		m_max_hunshou_id = hunshou_id;

		LieMingHunshouConfig &cfg_item = m_hunshou_cfglist[hunshou_id];
		cfg_item.hunshou_id = hunshou_id;

		if (!PugiGetSubNodeValue(data_element, "hunshou_type", cfg_item.hunshou_type) || cfg_item.hunshou_type < 0 || cfg_item.hunshou_type >= LIEMING_HUNSHOU_TYPE_MAX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "hunshou_color", cfg_item.hunshou_color) || cfg_item.hunshou_color <= LIEMING_HUNSHOU_COLOR_INVALID || 
			cfg_item.hunshou_color >= LIEMING_HUNSHOU_COLOR_MAX)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "chouhun_weight", cfg_item.chouhun_weight) || cfg_item.chouhun_weight <= 0)
		{
			return -5;
		}

		LieMingChouhunHunshouWeightCfg &chouhun_weight_cfg = m_chouhun_hunshou_weight_cfglist[cfg_item.hunshou_color];
		if (chouhun_weight_cfg.cfg_count >= LIEMING_HUNSHOU_PER_COLOR_MAX_COUNT)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "hunshou_item", cfg_item.item_id) || (cfg_item.item_id > 0 && NULL == ITEMPOOL->GetItem(cfg_item.item_id)))
		{
			return -6;
		}

		chouhun_weight_cfg.total_weight += cfg_item.chouhun_weight;
		chouhun_weight_cfg.cfg_list[chouhun_weight_cfg.cfg_count].hunshou_id = cfg_item.hunshou_id;
		chouhun_weight_cfg.cfg_list[chouhun_weight_cfg.cfg_count].weight = cfg_item.chouhun_weight;
		++ chouhun_weight_cfg.cfg_count;

		data_element = data_element.next_sibling();
	}

	for (int i = LIEMING_HUNSHOU_COLOR_INVALID + 1; i < LIEMING_HUNSHOU_COLOR_MAX; ++ i)
	{
		if (m_chouhun_hunshou_weight_cfglist[i].total_weight <= 0)
		{
			return -20000;
		}
	}

	return 0;
}

int LieMingConfig::InitHunshouExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int hunshou_color = 0; 
		if (!PugiGetSubNodeValue(data_element, "hunshou_color", hunshou_color) || hunshou_color <= LIEMING_HUNSHOU_COLOR_INVALID || hunshou_color >= LIEMING_HUNSHOU_COLOR_MAX)
		{
			return -1;
		}

		LieMingHunshouLevelConfig &level_cfg = m_hunshou_level_cfglist[hunshou_color];
		level_cfg.color = hunshou_color;

		if (level_cfg.max_level >= LIEMING_HUNSHOU_MAX_LEVEL)
		{
			return -2;
		}

		int hunshou_level = 0;
		if (!PugiGetSubNodeValue(data_element, "hunshou_level", hunshou_level) || hunshou_level != level_cfg.max_level + 1 || hunshou_level > LIEMING_HUNSHOU_MAX_LEVEL)
		{
			return -3;
		}
		level_cfg.max_level = hunshou_level;

		LieMingHunshouLevelConfigItem &level_cfg_item = level_cfg.level_cfglist[hunshou_level];
		level_cfg_item.color = hunshou_color;
		level_cfg_item.level = hunshou_level;

		if (!PugiGetSubNodeValue(data_element, "exp", level_cfg_item.need_exp) || level_cfg_item.need_exp <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_GONGJI]) || level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_GONGJI] < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_FANGYU]) || level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_FANGYU] < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_QIXUE]) || level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_QIXUE] < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_MINGZHONG]) || level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_MINGZHONG] < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_SHANBI]) || level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_SHANBI] < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_BAOJI]) || level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_BAOJI] < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_JIANREN]) || level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_JIANREN] < 0)
		{
			return -11;
		}

		//if (!PugiGetSubNodeValue(data_element, "per_jingzhun", level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_POJIA]) || level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_POJIA] < 0)
		//{
		//	return -12;
		//}

		if (!PugiGetSubNodeValue(data_element, "dikang_shanghai", level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_DIKANG_SHANGHAI]) || level_cfg_item.attr_val_list[LIEMING_HUNSHOU_TYPE_DIKANG_SHANGHAI] < 0)
		{
			return -13;
		}

		data_element = data_element.next_sibling();
	}

	for (int i = LIEMING_HUNSHOU_COLOR_INVALID + 1; i < LIEMING_HUNSHOU_COLOR_MAX; ++ i)
	{
		LieMingHunshouLevelConfig &level_cfg = m_hunshou_level_cfglist[i];

		if (level_cfg.max_level > 0)
		{
			level_cfg.level_cfglist[0].total_exp = level_cfg.level_cfglist[0].need_exp;
		}
		for (int j = 1; j <= level_cfg.max_level ; ++ j)
		{
			level_cfg.level_cfglist[j].total_exp = level_cfg.level_cfglist[j].need_exp + level_cfg.level_cfglist[j -1].total_exp;
		}
	}

	return 0;
}

int LieMingConfig::InitChouhunCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int chouhun_color = 0;
		if (!PugiGetSubNodeValue(data_element, "chouhun_color", chouhun_color) || chouhun_color < 0 || chouhun_color >= LIEMING_CHOUHUN_COLOR_MAX)
		{
			return -1;
		}

		LieMingChouhunConfig &chouhun_cfg = m_chouhun_cfglist[chouhun_color];
		chouhun_cfg.chouhun_color = chouhun_color;

		if (!PugiGetSubNodeValue(data_element, "cost_hun_li", chouhun_cfg.cost_hunli) || chouhun_cfg.cost_hunli <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "advance_rate", chouhun_cfg.advance_rate) || chouhun_cfg.advance_rate < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "exp_card_rate", chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_INVALID]) || chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_INVALID] < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "green_rate", chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_GREEN]) || chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_GREEN] < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "blue_rate", chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_BLUE]) || chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_BLUE] < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "purple_rate", chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_PURPLE]) || chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_PURPLE] < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "orange_rate", chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_ORANGE]) || chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_ORANGE] < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "red_rate", chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_RED]) || chouhun_cfg.rate_list[LIEMING_HUNSHOU_COLOR_RED] < 0)
		{
			return -8;
		}

		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < LIEMING_CHOUHUN_COLOR_MAX; ++ i)
	{
		int total_rate = 0;
		{
			for (int j = LIEMING_HUNSHOU_COLOR_INVALID; j < LIEMING_HUNSHOU_COLOR_MAX; ++ j)
			{
				total_rate += m_chouhun_cfglist[i].rate_list[j];
			}
		}
		if (100 != total_rate)
		{
			return -20000;
		}
	}

	return 0;
}

int LieMingConfig::InitLieMingJihuoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}


	while (!data_element.empty())
	{
		int activity_mingge_count = 0;
		if (!PugiGetSubNodeValue(data_element, "active_mingge_count", activity_mingge_count) || activity_mingge_count < 0 || activity_mingge_count > LIEMING_FUHUN_SLOT_COUNT)
		{
			return -1;
		}

		LieMingJihuoCfg &mingge_cfg = m_jihuo_cfg[activity_mingge_count -1];
		mingge_cfg.active_mingge_count = activity_mingge_count;
		
		int last_pata_level = 0;
		if (!PugiGetSubNodeValue(data_element, "role_level", mingge_cfg.role_level) || mingge_cfg.role_level < 0 || mingge_cfg.role_level > MAX_ROLE_LEVEL)
		{
			return -2;
		}
		
		if (last_pata_level > mingge_cfg.role_level)
		{
			return -3;
		}

		last_pata_level = mingge_cfg.role_level;

		data_element = data_element.next_sibling();
	}

	return 0;
}


