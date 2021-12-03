#include "magiccardconfig.hpp"
#include "obj/character/role.h"
#include "item/itemextern.hpp"
#include "world.h"
#include "item/itempool.h"
#include <servercommon/configcommon.h>

MagicCardConfig::MagicCardConfig() : m_card_suit_cfg_count(0), m_all_card_count(0), m_purple_choucard_cfg_count(0), m_orange_choucard_cfg_count(0), m_red_choucard_cfg_count(0), m_sprite_list_cout(0)
{
	memset(m_sprite_card_id_list, 0, sizeof(m_sprite_card_id_list));
}

MagicCardConfig::~MagicCardConfig()
{

}

bool MagicCardConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "MagicCardConfig", *err);

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
		// ¿¨ÅÆÌ××°ÊôÐÔ
		PugiXmlNode root_element = RootElement.child("card_suit");
		if (root_element.empty())
		{
			*err = configname + ": no [card_suit].";
			return false;
		}

		iRet = this->InitCardSuitCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCardSuitCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¿¨ÅÆÐÅÏ¢
		PugiXmlNode root_element = RootElement.child("card_info");
		if (root_element.empty())
		{
			*err = configname + ": no [card_info].";
			return false;
		}

		iRet = this->InitAllCardInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitAllCardInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¿¨ÅÆÇ¿»¯ÐÅÏ¢
		PugiXmlNode root_element = RootElement.child("strength");
		if (root_element.empty())
		{
			*err = configname + ": no [strength].";
			return false;
		}

		iRet = this->InitStrengthCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitStrengthCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ×Ï¿¨³é½±ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("purple_chou_card");
		if (root_element.empty())
		{
			*err = configname + ": no [purple_chou_card].";
			return false;
		}

		iRet = this->InitPurpleChouCardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPurpleChouCardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ³È¿¨³é½±ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("orange_chou_card");
		if (root_element.empty())
		{
			*err = configname + ": no [orange_chou_card].";
			return false;
		}

		iRet = this->InitOrangeChouCardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOrangeChouCardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ºì¿¨³é½±ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("red_chou_card");
		if (root_element.empty())
		{
			*err = configname + ": no [red_chou_card].";
			return false;
		}

		iRet = this->InitRedChouCardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRedChouCardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int MagicCardConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "open_level", m_other_cfg.open_level) || m_other_cfg.open_level < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "purple_chou_card_consume_gold", m_other_cfg.purple_chou_card_consume_gold) || m_other_cfg.purple_chou_card_consume_gold <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "orange_chou_card_consume_gold", m_other_cfg.orange_chou_card_consume_gold) || m_other_cfg.orange_chou_card_consume_gold <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "orange_chou_card_consume_item_id", m_other_cfg.orange_chou_card_consume_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.orange_chou_card_consume_item_id))
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "day_free_times", m_other_cfg.day_free_times) || m_other_cfg.day_free_times < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "red_chou_card_consume_gold", m_other_cfg.red_chou_card_consume_gold) || m_other_cfg.red_chou_card_consume_gold <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "red_chou_card_consume_item_id", m_other_cfg.red_chou_card_consume_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.red_chou_card_consume_item_id))
	{
		return -7;
	}

	return 0;
}

int MagicCardConfig::InitCardSuitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_card_suit_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_card_suit_cfg_count < 0 || m_card_suit_cfg_count >= MAGIC_CARD_COLOR_TYPE_COLOR_COUNT)
		{
			return -1;
		}

		MagicCardSuitConfig &suit_cfg = m_card_suit_cfg_list[m_card_suit_cfg_count];
		if (!PugiGetSubNodeValue(data_element, "color", suit_cfg.color) || suit_cfg.color != m_card_suit_cfg_count)
		{
			return -2;
		}

		//if (!PugiGetSubNodeValue(data_element, "skill_need_level", suit_cfg.skill_need_level) || suit_cfg.skill_need_level <= 0)
		//{
		//	return -3;
		//}

		//if (!PugiGetSubNodeValue(data_element, "skill_id", suit_cfg.skill_id) || NULL == SKILLPOOL->GetSkill(suit_cfg.skill_id))
		//{
		//	return -4;
		//}

		if (!PugiGetSubNodeValue(data_element, "attr_need_level", suit_cfg.attr_need_level) || suit_cfg.attr_need_level <= 0)
		{
			return -5;
		}

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

		++ m_card_suit_cfg_count;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int MagicCardConfig::InitAllCardInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_all_card_count = 0;

	while (!data_element.empty())
	{
		int card_id = 0;
		if (!PugiGetSubNodeValue(data_element, "card_id", card_id) || card_id <= 0 || card_id > MAGIC_CARD_MAX_LIMIT_COUNT)
		{
			return -1;
		}

		MagicCardInfoConfig &card_cfg = m_all_card_list[card_id];
		card_cfg.card_id = card_id;

		if (!PugiGetSubNodeValue(data_element, "type", card_cfg.type) || card_cfg.type < 0 || card_cfg.type >= MAGIC_CARD_CARD_TYPE_MAX_NUM)
		{
			return -5;
		}
		bool is_normal_card = (MAGIC_CARD_CARD_TYPE_NORMAL_CARD == card_cfg.type);
		bool is_sprite_card = (MAGIC_CARD_CARD_TYPE_SPRITE == card_cfg.type);
		bool is_exp_card = (MAGIC_CARD_CARD_TYPE_EXP_CARD == card_cfg.type);

		if (!PugiGetSubNodeValue(data_element, "item_id", card_cfg.item_id) || (!is_sprite_card && NULL == ITEMPOOL->GetItem(card_cfg.item_id)))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "color", card_cfg.color) || card_cfg.color < 0 || card_cfg.color >= MAGIC_CARD_COLOR_TYPE_COLOR_COUNT)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "slot_index", card_cfg.slot_index) || card_cfg.slot_index < 0 || card_cfg.slot_index >= MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT)
		{
			return -4;
		}

		if (is_sprite_card)
		{
			if (card_cfg.color != m_sprite_list_cout)
			{
				return -101;
			}

			m_sprite_card_id_list[card_cfg.color] = card_cfg.card_id;
			++ m_sprite_list_cout;
		}
		else if (is_normal_card || is_exp_card)
		{
			if (m_item_id_to_card_id_map.find(card_cfg.item_id) != m_item_id_to_card_id_map.end()) // ´ËÎïÆ·ÒÑÅä¹ý
			{
				return -102;
			}
			m_item_id_to_card_id_map[card_cfg.item_id] = card_cfg.card_id;
		}

		if (!PugiGetSubNodeValue(data_element, "need_sprit_num", card_cfg.need_sprit_num) || card_cfg.need_sprit_num < 0)
		{
			return -6;
		}

		if (is_normal_card && card_cfg.need_sprit_num <= 0)
		{
			return -7;
		}

		++ m_all_card_count;
		data_element = data_element.next_sibling();
	}
	
	return 0;
}

int MagicCardConfig::InitStrengthCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int card_id = 0;
		if (!PugiGetSubNodeValue(data_element, "card_id", card_id) || card_id < 0 || card_id > m_all_card_count || card_id > MAGIC_CARD_MAX_LIMIT_COUNT)
		{
			return -1;
		}
		MagicCardInfoConfig &card_cfg = m_all_card_list[card_id];

		bool is_normal_card = (MAGIC_CARD_CARD_TYPE_NORMAL_CARD == card_cfg.type);

		if (card_cfg.strength_max_level < 0 || card_cfg.strength_max_level > MAGIC_CARD_LIMIT_STRENGTH_LEVEL_MAX)
		{
			return -11;
		}

		int strength_level = -1;
		if (!PugiGetSubNodeValue(data_element, "strength_level", strength_level) || strength_level != card_cfg.strength_max_level + 1)
		{
			return -2;
		}
		card_cfg.strength_max_level = strength_level;

		MagicCardInfoConfig::StrengthAttr &card_strength_cfg = card_cfg.strength_attr_list[strength_level];
		card_strength_cfg.strength_level = strength_level;

		if (!PugiGetSubNodeValue(data_element, "up_level_need_exp", card_strength_cfg.up_level_need_exp) || (is_normal_card && card_strength_cfg.up_level_need_exp < 0))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "contain_exp", card_strength_cfg.contain_exp) || card_strength_cfg.contain_exp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", card_strength_cfg.maxhp) || (is_normal_card && card_strength_cfg.maxhp < 0))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", card_strength_cfg.gongji) || (is_normal_card && card_strength_cfg.gongji < 0))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", card_strength_cfg.fangyu) || (is_normal_card && card_strength_cfg.fangyu < 0))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", card_strength_cfg.mingzhong) || (is_normal_card && card_strength_cfg.mingzhong < 0))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", card_strength_cfg.shanbi) || (is_normal_card && card_strength_cfg.shanbi < 0))
		{
			return -9;
		}

		data_element = data_element.next_sibling();
	}
	
	return 0;
}

int MagicCardConfig::InitPurpleChouCardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_purple_choucard_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_purple_choucard_cfg_count < 0 || m_purple_choucard_cfg_count >= MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT)
		{
			return -1;
		}

		MagicCardChouCardConfig &purple_cfg = m_purple_choucard_cfg_list[m_purple_choucard_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "reward_seq", purple_cfg.reward_seq) || purple_cfg.reward_seq != m_purple_choucard_cfg_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "card_id", purple_cfg.reward_card_id) || purple_cfg.reward_card_id <= 0
			|| purple_cfg.reward_card_id > m_all_card_count	|| purple_cfg.reward_card_id > MAGIC_CARD_MAX_LIMIT_COUNT)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_count", purple_cfg.reward_count) || purple_cfg.reward_count < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", purple_cfg.weight) || purple_cfg.weight < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", purple_cfg.is_broadcast))
		{
			return -6;
		}

		++ m_purple_choucard_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int MagicCardConfig::InitOrangeChouCardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_orange_choucard_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_orange_choucard_cfg_count < 0 || m_orange_choucard_cfg_count >= MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT)
		{
			return -1;
		}

		MagicCardChouCardConfig &orange_cfg = m_orange_choucard_cfg_list[m_orange_choucard_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "reward_seq", orange_cfg.reward_seq) || orange_cfg.reward_seq != m_orange_choucard_cfg_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "card_id", orange_cfg.reward_card_id) || orange_cfg.reward_card_id <= 0
			|| orange_cfg.reward_card_id > m_all_card_count	|| orange_cfg.reward_card_id > MAGIC_CARD_MAX_LIMIT_COUNT)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_count", orange_cfg.reward_count) || orange_cfg.reward_count < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", orange_cfg.weight) || orange_cfg.weight < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", orange_cfg.is_broadcast))
		{
			return -6;
		}

		++ m_orange_choucard_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int MagicCardConfig::InitRedChouCardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_red_choucard_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_red_choucard_cfg_count < 0 || m_red_choucard_cfg_count >= MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT)
		{
			return -1;
		}

		MagicCardChouCardConfig &red_cfg = m_red_choucard_cfg_list[m_red_choucard_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "reward_seq", red_cfg.reward_seq) || red_cfg.reward_seq != m_red_choucard_cfg_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "card_id", red_cfg.reward_card_id) || red_cfg.reward_card_id <= 0
			|| red_cfg.reward_card_id > m_all_card_count	|| red_cfg.reward_card_id > MAGIC_CARD_MAX_LIMIT_COUNT)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_count", red_cfg.reward_count) || red_cfg.reward_count < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", red_cfg.weight) || red_cfg.weight < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", red_cfg.is_broadcast))
		{
			return -6;
		}

		++ m_red_choucard_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const MagicCardInfoConfig::StrengthAttr * MagicCardInfoConfig::GetStrenthAttrCfg(int strength) const
{
	if (strength <= 0 || strength > strength_max_level || strength > MAGIC_CARD_LIMIT_STRENGTH_LEVEL_MAX)
	{
		return NULL;
	}

	return &strength_attr_list[strength];
}

const MagicCardSuitConfig * MagicCardConfig::GetCardSuitCfg(int color)
{
	if (color < 0 || color >= m_card_suit_cfg_count || color >= MAGIC_CARD_COLOR_TYPE_COLOR_COUNT)
	{
		return NULL;
	}

	return &m_card_suit_cfg_list[color];
}


const MagicCardInfoConfig * MagicCardConfig::GetCardCfg(int card_id)
{
	if (card_id <= 0 || card_id > m_all_card_count || card_id > MAGIC_CARD_MAX_LIMIT_COUNT)
	{
		return NULL;
	}

	return &m_all_card_list[card_id];
}


const MagicCardChouCardConfig * MagicCardConfig::GetRandomRewardCfg(int type)
{
	if (type < MAGIC_CARD_CHOU_CARD_TYPE_PURPLE || type >= MAGIC_CARD_CHOU_CARD_TYPE_MAX_NUM)
	{
		return NULL;
	}

	MagicCardChouCardConfig *cfg = NULL;
	int all_weight = 0;
	int cfg_count = 0;

	switch (type)
	{
	case MAGIC_CARD_CHOU_CARD_TYPE_PURPLE:
		{
			cfg = m_purple_choucard_cfg_list;
			cfg_count = m_purple_choucard_cfg_count;
		}
		break;

	case MAGIC_CARD_CHOU_CARD_TYPE_ORAGEN:
		{
			cfg = m_orange_choucard_cfg_list;
			cfg_count = m_orange_choucard_cfg_count;
		}
		break;

	case MAGIC_CARD_CHOU_CARD_TYPE_RED:
		{
			cfg = m_red_choucard_cfg_list;
			cfg_count = m_red_choucard_cfg_count;
		}
		break;
	}

	if (NULL == cfg || cfg_count <= 0)
	{
		return NULL;
	}

	for (int i = 0; i < cfg_count; ++ i)
	{
		all_weight += cfg[i].weight;
	}

	if (all_weight <= 0)
	{
		return NULL;
	}

	int rand_weight = RandomNum(all_weight); 

	for (int index = 0; index < cfg_count; ++ index)
	{
		int cur_weight = cfg[index].weight;
		if (rand_weight < cur_weight)
		{
			return &cfg[index];
		}
		rand_weight -= cur_weight;
	}

	return NULL;
}


int MagicCardConfig::GetSpriteCardId(int color)
{
	if (color < MAGIC_CARD_COLOR_TYPE_BLUE || color >= MAGIC_CARD_COLOR_TYPE_COLOR_COUNT)
	{
		return -1;
	}

	return m_sprite_card_id_list[color];
}

int MagicCardConfig::GetCardIdWithItemID(ItemID item_id)
{
	std::map<ItemID, int>::iterator index_it = m_item_id_to_card_id_map.find(item_id);
	if (index_it != m_item_id_to_card_id_map.end())
	{
		return index_it->second;
	}

	return -1;
}







