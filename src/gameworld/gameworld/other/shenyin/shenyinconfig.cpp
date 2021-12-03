#include "shenyinconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "item/itempool.h"

ShenYinConfig::ShenYinConfig() : m_bead_cfg_count(0), m_bead_combine_cfg_count(0), m_bead_combine_attr_cfg_count(0), m_level_limit_cfg_count(0), m_item_list_cfg_count(0),
	m_imprint_base_attr_cfg_count(0), m_imprint_up_start_cfg_count(0), m_imprint_extra_attr_count_cfg_count(0),m_imprint_extra_attr_count_cfg_total_weight(0),
	m_imprint_extra_attr_cfg_count(0), m_imprint_recycle_cfg_count(0), m_imprint_exchange_cfg_count(0), m_imprint_suit_cfg_count(0), m_max_liehun_item_id(0), 
	m_shenyin_wash_attr_total_weight(0)
{
	memset(m_suit_list, 0, sizeof(m_suit_list));
}

ShenYinConfig::~ShenYinConfig()
{
}

bool ShenYinConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	PUGI_XML_LOAD_CONFIG("bead", InitBeadCfg);
	PUGI_XML_LOAD_CONFIG("combine", InitCombineCfg);
	PUGI_XML_LOAD_CONFIG("combine_attr", InitCombineAttrCfg);
	PUGI_XML_LOAD_CONFIG("level_limit_cfg", InitLevelLimitCfg);
	PUGI_XML_LOAD_CONFIG("item_list", InitItemListCfg);
	PUGI_XML_LOAD_CONFIG("imprint_base_attr", InitImprintBaseAttrCfg);
	PUGI_XML_LOAD_CONFIG("imprint_up_star", InitImprintUpStartCfg);
	PUGI_XML_LOAD_CONFIG("imprint_extra_attr_count", InitImprintExtraAttrCountCfg);
	PUGI_XML_LOAD_CONFIG("imprint_extra_attr", InitImprintExtraAttrCfg);
	PUGI_XML_LOAD_CONFIG("imprint_recycle", InitImprintRecycleCfg);
	PUGI_XML_LOAD_CONFIG("imprint_exchange", InitImprintExchangeCfg);
	PUGI_XML_LOAD_CONFIG("suit", InitImprintSuitCfg);
	PUGI_XML_LOAD_CONFIG("hunshou", InitLieHunItemCfg);
	PUGI_XML_LOAD_CONFIG("chouhun", InitChouhunCfg);
	PUGI_XML_LOAD_CONFIG("xilian_attr", InitWashAttrCfg);
	PUGI_XML_LOAD_CONFIG("xilian_add_count", InitWashAddCountCfg);
	PUGI_XML_LOAD_CONFIG("combine_attr_add", InitCombineAttrAddCfg);
	PUGI_XML_LOAD_CONFIG("new_suit", InitNewSuitCfg);
	//PUGI_XML_LOAD_CONFIG("lock_consume", InitLockConsumeCfg);

	return true;
}

const ShenYinBeadCfg * ShenYinConfig::GetBeadCfg(int type) const
{
	if (type <= SHEN_YIN_BEAD_TYPE_INVALID  || type >= SHEN_YIN_BEAD_TYPE_MAX)
	{
		return NULL;
	}

	if (m_bead_cfg[type].type == type)
	{
		return &m_bead_cfg[type];
	}

	return NULL;
}

const ShenYinCombineSeqCfg * ShenYinConfig::GetCombineSeqCfg(int seq) const
{
	if (seq < 0 || seq >= SHEN_YIN_MAX_COMBINE_CFG)
	{
		return NULL;
	}
	const ShenYinCombineSeqCfg &cfg = m_bead_combine_cfg_list[seq];
	if (cfg.bead_list[0].type <= SHEN_YIN_BEAD_TYPE_INVALID)
	{
		return NULL;
	}

	if (cfg.seq == seq)
	{
		return &cfg;
	}

	return NULL;
}

const ShenYinCombineAttrCfg * ShenYinConfig::GetCombineAttrCfg(int seq) const
{
	if (seq < 0 || seq >= m_bead_combine_attr_cfg_count)
	{
		return NULL;
	}

	if (m_bead_combine_attr_cfg_list[seq].seq == seq)
	{
		return &m_bead_combine_attr_cfg_list[seq];
	}

	return NULL;
}

const ShenYinLevelLimitCfg * ShenYinConfig::GetLevelLimitCfg(int level) const
{
	for (int i = m_level_limit_cfg_count - 1; i >= 0 ; -- i )
	{
		if (level >= m_level_limit_cfg_list[i].limit_level )
		{
			return &m_level_limit_cfg_list[i];
		}
	}

	return NULL;
}

const ShenYinPastureSpiritItemCfg * ShenYinConfig::GetItemByVid(int v_item_id) const
{
	for (int i = 0; i < PASTURE_SPIRIT_ITEM_LIST_MAX_CFG_COUNT; ++i)
	{
		if (m_item_list_cfg_list[i].v_item_id == v_item_id)
		{
			return &m_item_list_cfg_list[i];
		}
	}
	return NULL;
}

const ShenYinPastureSpiritItemCfg * ShenYinConfig::GetItemByid(ItemID item_id) const
{
	for (int i = 0; i < PASTURE_SPIRIT_ITEM_LIST_MAX_CFG_COUNT; ++i)
	{
		if (m_item_list_cfg_list[i].item_id == item_id)
		{
			return &m_item_list_cfg_list[i];
		}
	}

	return NULL;
}

const ItemID ShenYinConfig::GetItemIdByVid(int v_item_id) const
{
	for (int i = 0; i < PASTURE_SPIRIT_ITEM_LIST_MAX_CFG_COUNT; ++i)
	{
		if (m_item_list_cfg_list[i].v_item_id == v_item_id)
		{
			return m_item_list_cfg_list[i].item_id;
		}
	}
	return 0;
}

const ShenYinPastureSpiritImprintBaseAttrCfg::SlotParam * ShenYinConfig::GetImprintBaseAttr(int quality, int slot_type) const
{
	if (quality < 0 || quality > m_imprint_base_attr_cfg_count)
	{
		return NULL;
	}

	if (slot_type < 0 || slot_type >= SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT)
	{
		return NULL;
	}

	const ShenYinPastureSpiritImprintBaseAttrCfg &cfg = m_imprint_base_attr_cfg_list[quality];
	if (cfg.quality != quality)
	{
		return NULL;
	}

	if (cfg.slot_list[slot_type].slot_type != slot_type)
	{
		return NULL;
	}

	return &cfg.slot_list[slot_type];

}

const ShenYinPastureSpiritImprintUpStartCfg * ShenYinConfig::GetImprintUpLevel(int slot_type, int grade, int level) const
{
	for (int i = 0; i < (int)m_imprint_up_start_cfg_list.size(); ++ i)
	{
		if (slot_type == m_imprint_up_start_cfg_list[i].slot_type &&
			grade == m_imprint_up_start_cfg_list[i].grade &&
			level == m_imprint_up_start_cfg_list[i].level)
		{
			return &m_imprint_up_start_cfg_list[i];
		}
	}

	return NULL;
}

int ShenYinConfig::GetImprintExtraAttrCount() const
{
	int rand_weight = RandomNum(m_imprint_extra_attr_count_cfg_total_weight);
	int index = 0;
	for (; index < m_imprint_extra_attr_count_cfg_count; ++index)
	{
		if (rand_weight < m_imprint_extra_attr_count_cfg_list[index].weight)
		{
			break;
		}

		rand_weight -= m_imprint_extra_attr_count_cfg_list[index].weight;
	}

	return m_imprint_extra_attr_count_cfg_list[index].attr_count;
}

AttrConfig::ATTR_TYPE ShenYinConfig::GetRandImprintExtraAttrType() const
{
	int total_weight = RandomNum(m_shenyin_wash_attr_total_weight);
	for (int i = 0; i < SHEN_YIN_MAX_ATTR_NUM; ++ i)
	{
		if (total_weight < m_shenyin_wash_attr_list[i].weight)
		{
			return static_cast<AttrConfig::ATTR_TYPE>(m_shenyin_wash_attr_list[i].attr_id);
		}

		total_weight -= m_shenyin_wash_attr_list[i].weight;
	}
	return static_cast<AttrConfig::ATTR_TYPE>(-1);
}

const ShenYinPastureSpiritImprintExtraAttrCfg * ShenYinConfig::GetRandImprintExtraAttrByType(int slot_type, int attr_tyoe) const
{
	for (int i = 0; i < (int)m_imprint_extra_attr_cfg_list.size(); ++i)
	{
		if (m_imprint_extra_attr_cfg_list[i].slot_type == slot_type && m_imprint_extra_attr_cfg_list[i].attr_type == attr_tyoe)
		{
			return &m_imprint_extra_attr_cfg_list[i];
		}
	}

	return NULL;
}

const ShenYinPastureSpiritImprintRecycleCfg * ShenYinConfig::GetImprintRecycle(int quality, bool is_suit) const
{
	for (int i = 0; i < PASTURE_SPIRIT_IMPRINT_RECYCLE_MAX_CFG_COUNT; ++i)
	{
		if (m_imprint_recycle_cfg_list[i].quanlity == quality && m_imprint_recycle_cfg_list[i].is_suit == is_suit)
		{
			return &m_imprint_recycle_cfg_list[i];
		}
	}

	return NULL;
}

const ShenYinPastureSpiritImprintExchangeCfg * ShenYinConfig::GetImprintExchange(int index) const
{
	if (index < 0 || index >= m_imprint_exchange_cfg_count)
	{
		return NULL;
	}

	if (m_imprint_exchange_cfg_list[index].index != index)
	{
		return NULL;
	}

	return &m_imprint_exchange_cfg_list[index];
}

const ShenYinPastureSpiritImprintSuitCfg::SuitAttr * ShenYinConfig::GetSuitAttr(int suit_id, int part_count) const
{
	if (suit_id <= 0 || suit_id >= PASTURE_SPIRIT_IMPRINT_SUIT_MAX_CFG_COUNT)
	{
		return NULL;
	}

	const ShenYinPastureSpiritImprintSuitCfg &cfg = m_imprint_suit_cfg_list[suit_id];
	for (int i = 0; i < cfg.cfg_count; ++i)
	{
		if (part_count == cfg.suit_list[i].count)
		{
			return &cfg.suit_list[i];
		}
	}

	return NULL;
}

const ShenYinLieMingItemConfig * ShenYinConfig::GetHunshouCfg(int hunshou_id)
{
	if (hunshou_id > 0 && hunshou_id <= m_max_liehun_item_id)
	{
		return &m_hunshou_cfglist[hunshou_id];
	}

	return NULL;
}

const ShenYinLieMingChouhunConfig * ShenYinConfig::GetChouhunCfg(int chouhun_color)
{
	if (chouhun_color < 0 || chouhun_color >= SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX)
	{
		return NULL;
	}

	return &m_chouhun_cfglist[chouhun_color];
}

const ShenYinLieMingChouhunItemWeightCfg * ShenYinConfig::GetChouhunItemWeightCfg(int hunshou_color)
{
	if (hunshou_color <= SHEN_YIN_LIEMING_CHOUHUN_COLOR_INVALID || hunshou_color >= SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX)
	{
		return NULL;
	}

	return &m_chouhun_weight_cfglist[hunshou_color];
}

const ShenYinWashAttr * ShenYinConfig::GetShenYinWashAttrCfg(int type) const
{
	if (type == m_shenyin_wash_attr_list[type].attr_id)
	{
		return &m_shenyin_wash_attr_list[type];
	}

	return NULL;
}

const ShenYinWashAddCount * ShenYinConfig::GetShenYinWashAddCountCfg(int attr_num) const
{
	if (attr_num <= 0 || attr_num >= SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT) return NULL;

	for (int i = 0; i < (int)m_shenyin_wash_add_count_list.size(); ++ i)
	{
		if (attr_num == m_shenyin_wash_add_count_list[i].add_count)
		{
			return &m_shenyin_wash_add_count_list[i];
		}
	}

	return NULL;
}

const ShenYinCombineAttrAddCfg * ShenYinConfig::GetShenYinCombineAttrAddCfg(int index) const
{
	if (index < 0 || index >= (int)m_combine_attr_add_list.size())
	{
		return NULL;
	}

	return &m_combine_attr_add_list[index];
}

int ShenYinConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	
	if (!PugiGetSubNodeValue(data_element, "limit_level", m_other_cfg.limit_level) || m_other_cfg.limit_level <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "change_bead_type_need_gold", m_other_cfg.change_bead_type_need_gold) || m_other_cfg.change_bead_type_need_gold < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "flush_attr_type_need_v_item_id", m_other_cfg.flush_attr_type_need_v_item_id) || m_other_cfg.flush_attr_type_need_v_item_id < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "flush_attr_type_need_v_item_num", m_other_cfg.flush_attr_type_need_v_item_num) || m_other_cfg.flush_attr_type_need_v_item_num <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "flush_attr_value_need_v_item_id", m_other_cfg.flush_attr_value_need_v_item_id) || m_other_cfg.flush_attr_value_need_v_item_id < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "flush_attr_value_need_v_item_num", m_other_cfg.flush_attr_value_need_v_item_num) || m_other_cfg.flush_attr_value_need_v_item_num <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "add_attr_type_need_v_item_id", m_other_cfg.add_attr_type_need_v_item_id) || m_other_cfg.add_attr_type_need_v_item_id < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "super_chouhun_price", m_other_cfg.super_chouhun_price) || m_other_cfg.super_chouhun_price < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "lock_consume", m_other_cfg.lock_consume) || m_other_cfg.lock_consume < 0)
	{
		return -9;
	}
	
	return 0;
}

int ShenYinConfig::InitBeadCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_bead_cfg_count = 1;
	while (!data_element.empty())
	{
		if (m_bead_cfg_count >= SHEN_YIN_BEAD_TYPE_MAX)
		{
			return -1001;
		}

		int type = 0;
		if (!PugiGetSubNodeValue(data_element, "type", type) || type != m_bead_cfg_count)
		{
			return -1002;
		}

		ShenYinBeadCfg &cfg = m_bead_cfg[type];
		cfg.type = type;
		int iRet = cfg.attr.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		++m_bead_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenYinConfig::InitCombineAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_bead_combine_attr_cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_bead_combine_attr_cfg_count >= SHEN_YIN_MAX_COMBINE_CFG)
		{
			return -1001;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_bead_combine_attr_cfg_count)
		{
			return -1002;
		}

		ShenYinCombineAttrCfg &cfg = m_bead_combine_attr_cfg_list[seq];
		cfg.seq = seq;
		int iRet = cfg.attr.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		++m_bead_combine_attr_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenYinConfig::InitLevelLimitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_limit_level = 0;
	int last_bead_num = 0;
	m_level_limit_cfg_count = 0;
	while(!data_element.empty())
	{
		if (m_level_limit_cfg_count >= SHEN_YIN_LEVEL_LIMIT_MAX_CFG)
		{
			return -1001;
		}

		ShenYinLevelLimitCfg &cfg = m_level_limit_cfg_list[m_level_limit_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "limit_level", cfg.limit_level) || cfg.limit_level < last_limit_level)
		{
			return -1002;
		}
		last_limit_level = cfg.limit_level;

		if (!PugiGetSubNodeValue(data_element, "bead_num", cfg.bead_num) || cfg.bead_num < last_bead_num)
		{
			return -1002;
		}
		last_bead_num = cfg.bead_num;

		++m_level_limit_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenYinConfig::InitCombineCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	
	m_bead_combine_cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_bead_combine_cfg_count >= SHEN_YIN_MAX_COMBINE_CFG)
		{
			return -1001;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_bead_combine_cfg_count)
		{
			return -1204;
		}

		ShenYinCombineSeqCfg &cfg = m_bead_combine_cfg_list[seq];
		cfg.seq = seq;

		for (int index = 0; index < SHEN_YIN_COMBINE_MAX_BEADS; ++ index)
		{
			ShenYinCombineItem &bead_cfg = cfg.bead_list[index];

			char name[30] = {};
			SNPRINTF(name, sizeof(name), "type_%d", index);
			if (!PugiGetSubNodeValue(data_element, name, bead_cfg.type) || bead_cfg.type < SHEN_YIN_BEAD_TYPE_INVALID || bead_cfg.type > SHEN_YIN_BEAD_TYPE_SAME)
			{
				return -1205;
			}

			memset(name, 0, sizeof(name));
			SNPRINTF(name, sizeof(name), "x_%d", index);
			if (!PugiGetSubNodeValue(data_element, name, bead_cfg.x) || bead_cfg.x < SCHAR_MIN || bead_cfg.x > SCHAR_MAX)
			{
				return -1206;
			}

			memset(name, 0, sizeof(name));
			SNPRINTF(name, sizeof(name), "y_%d", index);
			if (!PugiGetSubNodeValue(data_element, name, bead_cfg.y) || bead_cfg.y < SCHAR_MIN || bead_cfg.y > SCHAR_MAX)
			{
				return -1207;
			}
		}

		++m_bead_combine_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenYinConfig::InitItemListCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_item_list_cfg_count >= PASTURE_SPIRIT_ITEM_LIST_MAX_CFG_COUNT)
		{
			return -10001;
		}

		ShenYinPastureSpiritItemCfg &cfg = m_item_list_cfg_list[m_item_list_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "v_item_id", cfg.v_item_id) || cfg.v_item_id < 0)
		{
			return -10002;
		}

		if (!PugiGetSubNodeValue(data_element, "item_id", cfg.item_id) || !ITEMPOOL->GetItem(cfg.item_id))
		{
			return -10022;
		}

		if (!PugiGetSubNodeValue(data_element, "item_type", cfg.item_type) || cfg.item_type < 0)
		{
			return -10003;
		}

		if (!PugiGetSubNodeValue(data_element, "stack_num", cfg.stack_num) || cfg.stack_num < 0)
		{
			return -10004;
		}

		if (!PugiGetSubNodeValue(data_element, "quanlity", cfg.quanlity) || cfg.quanlity < 0)
		{
			return -10005;
		}

		if ((int)ShenYinPastureSpiritItemCfg::EQUIPMENT == cfg.item_type)
		{
			if (!PugiGetSubNodeValue(data_element, "imprint_slot", cfg.imprint_slot) || cfg.imprint_slot < 0 || cfg.imprint_slot >= SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT)
			{
				return -10006;
			}

			if (!PugiGetSubNodeValue(data_element, "suit_id", cfg.suit_id) || cfg.suit_id < 0)
			{
				return -10007;
			}

			if (!PugiGetSubNodeValue(data_element, "return_item_id", cfg.return_item_id) || !ITEMPOOL->GetItem(cfg.return_item_id))
			{
				return -10008;
			}

			if (!PugiGetSubNodeValue(data_element, "return_item_num", cfg.return_item_num) || cfg.return_item_num < 0)
			{
				return -10009;
			}
		}

		++m_item_list_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}


int ShenYinConfig::InitImprintBaseAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_imprint_base_attr_cfg_count = 0;
	while (!data_element.empty())
	{
		int quality = 0;
		if (!PugiGetSubNodeValue(data_element, "quanlity", quality) || (quality != m_imprint_base_attr_cfg_count && quality != m_imprint_base_attr_cfg_count + 1))
		{
			return -10002;
		}

		m_imprint_base_attr_cfg_count = quality;
		if (m_imprint_base_attr_cfg_count >= SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX)
		{
			return -10001;
		}

		ShenYinPastureSpiritImprintBaseAttrCfg &cfg = m_imprint_base_attr_cfg_list[m_imprint_base_attr_cfg_count];
		cfg.quality = quality;
		int imprint_slot = 0;

		if (!PugiGetSubNodeValue(data_element, "imprint_slot", imprint_slot) || imprint_slot >= SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT)
		{
			return -10003;
		}
		cfg.slot_list[imprint_slot].slot_type = imprint_slot;

		int iRet = cfg.slot_list[imprint_slot].attr.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}


int ShenYinConfig::InitImprintUpStartCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ShenYinPastureSpiritImprintUpStartCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "slot_ype", cfg.slot_type) || (cfg.slot_type < 0))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "stage", cfg.grade) || (cfg.grade < 0))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || (cfg.level < 0))
		{
			return -3;
		}

		int iRet = cfg.attr_cfg.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		if (!PugiGetSubNodeValue(data_element, "basics_addition", cfg.basics_addition) || (cfg.basics_addition < 0))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "consume_v_item_id", cfg.consume_v_item_id) || (cfg.consume_v_item_id < 0))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "need_num", cfg.need_num) || (cfg.need_num < 0))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "protect_v_item_id", cfg.protect_v_item_id) || (cfg.protect_v_item_id < 0))
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "need_protect_num", cfg.need_protect_num) || (cfg.need_protect_num < 0))
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "rate", cfg.rate) || (cfg.rate < 0))
		{
			return -11;
		}

		m_imprint_up_start_cfg_list.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}


int ShenYinConfig::InitImprintExtraAttrCountCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_imprint_extra_attr_count_cfg_total_weight = 0;
	while (!data_element.empty())
	{
		if (m_imprint_extra_attr_count_cfg_count >= PASTURE_SPIRIT_IMPRINT_EXTRA_ATTR_COUNT_MAX_CFG_COUNT)
		{
			return -10001;
		}

		ShenYinPastureSpiritImprintExtraAttrCountCfg &cfg = m_imprint_extra_attr_count_cfg_list[m_imprint_extra_attr_count_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "attr_count", cfg.attr_count) || cfg.attr_count < 0)
		{
			return -10002;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight < 0)
		{
			return -10003;
		}
		m_imprint_extra_attr_count_cfg_total_weight += cfg.weight;

		++m_imprint_extra_attr_count_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}


int ShenYinConfig::InitImprintExtraAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ShenYinPastureSpiritImprintExtraAttrCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "slot_type", cfg.slot_type) || cfg.slot_type < 0)
		{
			return -10001;
		}

		if (!PugiGetSubNodeValue(data_element, "attr_type", cfg.attr_type) || cfg.attr_type < 0)
		{
			return -10002;
		}

		if (!PugiGetSubNodeValue(data_element, "min_value", cfg.min_value) || cfg.min_value < 0)
		{
			return -10003;
		}

		if (!PugiGetSubNodeValue(data_element, "max_value", cfg.max_value) || cfg.max_value < cfg.min_value)
		{
			return -10004;
		}

		char attr_name[100];
		for (int index = 0; index < SHEN_YIN_XILIAN_BAODI_MAX_COUNT; ++ index)
		{
			ShenYinPastureSpiritImprintExtraAttrCfg::BaodiCfg & baodi_cfg = cfg.baodi_count_cfg[index];

			SNPRINTF(attr_name, 100, "baodi_count_%d", index);
			if (!PugiGetSubNodeValue(data_element, attr_name, baodi_cfg.baodi_count) || baodi_cfg.baodi_count < 0)
			{
				return -(1000 + index);
			}

			SNPRINTF(attr_name, 100, "criti_per_%d", index);
			if (!PugiGetSubNodeValue(data_element, attr_name, baodi_cfg.criti_per) || baodi_cfg.criti_per < 0 || baodi_cfg.criti_per > 100)
			{
				return -(1100 + index);
			}

		}

		m_imprint_extra_attr_cfg_list.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}


int ShenYinConfig::InitImprintRecycleCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_imprint_recycle_cfg_count >= PASTURE_SPIRIT_IMPRINT_RECYCLE_MAX_CFG_COUNT)
		{
			return -10001;
		}

		ShenYinPastureSpiritImprintRecycleCfg &cfg = m_imprint_recycle_cfg_list[m_imprint_recycle_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "quanlity", cfg.quanlity) || cfg.quanlity < 0)
		{
			return -10002;
		}

		if (!PugiGetSubNodeValue(data_element, "add_imprint_score", cfg.add_imprint_score) || cfg.add_imprint_score < 0)
		{
			return -10003;
		}

		int is_suit = 0;
		if (!PugiGetSubNodeValue(data_element, "is_suit", is_suit) || is_suit < 0)
		{
			return -10004;
		}
		cfg.is_suit = is_suit != 0;

		++m_imprint_recycle_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}


int ShenYinConfig::InitImprintExchangeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_imprint_exchange_cfg_count >= SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT)
		{
			return -10001;
		}

		ShenYinPastureSpiritImprintExchangeCfg &cfg = m_imprint_exchange_cfg_list[m_imprint_exchange_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "index", cfg.index) || cfg.index != m_imprint_exchange_cfg_count)
		{
			return -10011;
		}

		if (!PugiGetSubNodeValue(data_element, "v_item_id", cfg.v_item_id) || cfg.v_item_id < 0)
		{
			return -10002;
		}

		if (!PugiGetSubNodeValue(data_element, "item_num", cfg.item_num) || cfg.item_num < 0)
		{
			return -10003;
		}

		if (!PugiGetSubNodeValue(data_element, "imprint_score", cfg.imprint_score) || cfg.imprint_score < 0)
		{
			return -10004;
		}

		int is_bind = 0;
		if (!PugiGetSubNodeValue(data_element, "is_bind", is_bind) || is_bind < 0)
		{
			return -10005;
		}
		cfg.is_bind = is_bind != 0;

		if (!PugiGetSubNodeValue(data_element, "buy_count", cfg.buy_count) || cfg.buy_count <= 0)
		{
			return -10006;
		}

		int is_virtual_item = 0;
		if (!PugiGetSubNodeValue(data_element, "is_virtual_item", is_virtual_item) || is_virtual_item < 0 || is_virtual_item > 1)
		{
			return -10007;
		}
		cfg.is_virtual_item = (0 != is_virtual_item);

		if (!PugiGetSubNodeValue(data_element, "is_week_refresh", cfg.is_week_refresh) || cfg.is_week_refresh < 0)
		{
			return -10008;
		}

		if (!PugiGetSubNodeValue(data_element, "day", cfg.day) || cfg.day < 0)
		{
			return -10009;
		}

		++m_imprint_exchange_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenYinConfig::InitImprintSuitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int suit_id = 0;
		if (!PugiGetSubNodeValue(data_element, "suit_id", suit_id) || suit_id <= 0)
		{
			return -10003;
		}
		m_imprint_suit_cfg_count = suit_id;

		if (m_imprint_suit_cfg_count >= PASTURE_SPIRIT_IMPRINT_SUIT_MAX_CFG_COUNT)
		{
			return -10004;
		}

		ShenYinPastureSpiritImprintSuitCfg &base_cfg = m_imprint_suit_cfg_list[m_imprint_suit_cfg_count];
		base_cfg.suit_id = suit_id;

		if (base_cfg.cfg_count >= SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT)
		{
			return -10015;
		}

		ShenYinPastureSpiritImprintSuitCfg::SuitAttr &suit_cfg = base_cfg.suit_list[base_cfg.cfg_count];
		if (!PugiGetSubNodeValue(data_element, "count", suit_cfg.count) || suit_cfg.count <= 0)
		{
			return -10005;
		}

		if (suit_cfg.count >= SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT)
		{
			return -10006;
		}

		int iRet = suit_cfg.attr.ReadConfig(data_element);
		if (iRet < 0)
		{
			return 10007;
		}

		++base_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenYinConfig::InitLieHunItemCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_max_liehun_item_id >= SHEN_YIN_MAX_NUM)
		{
			return -1;
		}

		int hunshou_id = 0;
		if (!PugiGetSubNodeValue(data_element, "index", hunshou_id) || hunshou_id != m_max_liehun_item_id + 1)
		{
			return -2;
		}
		m_max_liehun_item_id = hunshou_id;

		ShenYinLieMingItemConfig &cfg_item = m_hunshou_cfglist[hunshou_id];
		cfg_item.item_index = hunshou_id;

		if (!PugiGetSubNodeValue(data_element, "shenyin_slot", cfg_item.slot_type) || cfg_item.slot_type < 0 || cfg_item.slot_type >= SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "shenyin_color", cfg_item.shenyin_color) || cfg_item.shenyin_color <= SHEN_YIN_LIEMING_CHOUHUN_COLOR_INVALID ||
			cfg_item.shenyin_color >= SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "chouhun_weight", cfg_item.chouhun_weight) || cfg_item.chouhun_weight <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "virtual_item_id", cfg_item.virtual_item_id) || cfg_item.virtual_item_id < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", cfg_item.is_broadcast) || cfg_item.is_broadcast < 0)
		{
			return -6;
		}

		ShenYinLieMingChouhunItemWeightCfg &chouhun_weight_cfg = m_chouhun_weight_cfglist[cfg_item.shenyin_color];
		if (chouhun_weight_cfg.cfg_count >= SHEN_YIN_LIEMING_HUNSHOU_PER_COLOR_MAX_COUNT)
		{
			return -100;
		}

		chouhun_weight_cfg.total_weight += cfg_item.chouhun_weight;
		chouhun_weight_cfg.cfg_list[chouhun_weight_cfg.cfg_count].index = cfg_item.item_index;
		chouhun_weight_cfg.cfg_list[chouhun_weight_cfg.cfg_count].weight = cfg_item.chouhun_weight;
		++chouhun_weight_cfg.cfg_count;

		data_element = data_element.next_sibling();
	}

	for (int i = SHEN_YIN_LIEMING_CHOUHUN_COLOR_INVALID + 1; i < SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX; ++ i)
	{
		if (m_chouhun_weight_cfglist[i].total_weight <= 0)
		{
			return -20000;
		}
	}

	return 0;
}

int ShenYinConfig::InitChouhunCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int chouhun_color = 0;
		if (!PugiGetSubNodeValue(data_element, "chouhun_color", chouhun_color) || chouhun_color < 0 || chouhun_color >= SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX)
		{
			return -1;
		}

		ShenYinLieMingChouhunConfig &chouhun_cfg = m_chouhun_cfglist[chouhun_color];
		chouhun_cfg.chouhun_color = chouhun_color;

		if (!PugiGetSubNodeValue(data_element, "cost_diamonds", chouhun_cfg.cost_gold) || chouhun_cfg.cost_gold <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "advance_rate", chouhun_cfg.advance_rate) || chouhun_cfg.advance_rate < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "color_rate_0", chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_GREEN]) || chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_GREEN] < 0)
		{
			return -31;
		}

		if (!PugiGetSubNodeValue(data_element, "white_rate", chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_BLUE]) || chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_BLUE] < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "green_rate", chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_PURPLE]) || chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_PURPLE] < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "blue_rate", chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_ORANGE]) || chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_ORANGE] < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "purple_rate", chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_RED]) || chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_RED] < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "integral_1_rate", chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_QIANGHUA]) || chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_QIANGHUA] < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "integral_2_rate", chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_XILIAN_FIRST]) || chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_XILIAN_FIRST] < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "integral_3_rate", chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_XILIAN_SECOND]) || chouhun_cfg.rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_XILIAN_SECOND] < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "cost_hun_li", chouhun_cfg.chouhun_score) || chouhun_cfg.chouhun_score <= 0)
		{
			return -11;
		}

		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < SHEN_YIN_LIEMING_CHOUHUN_COLOR_QIANGHUA; ++ i)
	{
		int total_rate = 0;
		{
			for (int j = SHEN_YIN_LIEMING_CHOUHUN_COLOR_INVALID + 1; j < SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX; ++ j)
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

int ShenYinConfig::InitWashAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ShenYinWashAttr cfg;
		if (!PugiGetSubNodeValue(data_element, "attr_id", cfg.attr_id) || cfg.attr_id < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight < 0)
		{
			return -2;
		}
		m_shenyin_wash_attr_total_weight += cfg.weight;

		m_shenyin_wash_attr_list.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenYinConfig::InitWashAddCountCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ShenYinWashAddCount cfg;
		if (!PugiGetSubNodeValue(data_element, "add_count", cfg.add_count) || cfg.add_count < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "consume_num", cfg.consume_num) || cfg.consume_num < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "rate", cfg.rate) || cfg.rate < 0)
		{
			return -3;
		}

		m_shenyin_wash_add_count_list.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenYinConfig::InitCombineAttrAddCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ShenYinCombineAttrAddCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "combine_seq_1", cfg.combine_seq_1) || cfg.combine_seq_1 < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "combine_seq_2", cfg.combine_seq_2) || cfg.combine_seq_2 < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "attr_add", cfg.attr_add) || cfg.attr_add < 0)
		{
			return -3;
		}

		m_combine_attr_add_list.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

//int ShenYinConfig::InitLockConsumeCfg(PugiXmlNode RootElement)
//{
//	PugiXmlNode data_element = RootElement.child("data");
//	if (data_element.empty())
//	{
//		return -10000;
//	}
//
//	m_lock_consume_vct.clear();
//
//	while (!data_element.empty())
//	{
//		int xilian_type = 0;
//		if (!PugiGetSubNodeValue(data_element, "xilian_type", xilian_type) || xilian_type < 0)
//		{
//			return -1;
//		}
//
//		ShenYinLockConsumeCfg lock_consume_cfg;
//		lock_consume_cfg.xilian_type = xilian_type;
//
//		int lock_num = 0;
//		if (!PugiGetSubNodeValue(data_element, "lock_num", lock_consume_cfg.lock_num) || lock_consume_cfg.lock_num < 0 || lock_consume_cfg.lock_num > SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT - 1)
//		{
//			return -2;
//		}
//
//		{
//			PugiXmlNode item_element = data_element.child("lock_comsume_item");
//			if (item_element.empty())
//			{
//				return -3;
//			}
//
//			if (!lock_consume_cfg.lock_comsume_item.ReadConfig(item_element) && lock_consume_cfg.lock_comsume_item.item_id != 0)
//			{
//				return -4;
//			}
//		}
//
//		m_lock_consume_vct.push_back(lock_consume_cfg);
//		data_element = data_element.next_sibling();
//	}
//
//	return 0;
//}
//
//const ShenYinLockConsumeCfg * ShenYinConfig::GetXilianLockCfg(int xilian_type, int lock_num)
//{
//	if (lock_num < 0 || lock_num > (SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT - 1)) return NULL;
//
//	for (int i = 0; i < static_cast<int>(m_lock_consume_vct.size()); i++)
//	{
//		if (m_lock_consume_vct[i].xilian_type == xilian_type && m_lock_consume_vct[i].lock_num == lock_num)
//		{
//			return &m_lock_consume_vct[i];
//		}
//	}
//
//	return NULL;
//}

int ShenYinConfig::InitNewSuitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{

		int color = 0;
		if (!PugiGetSubNodeValue(data_element, "quality", color) || color < 0 || color >= SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK + 1)
		{
			return -97;
		}

		ShenYinNewSuitCfg &suit_cfg = m_suit_list[color];

		int need_count = 0;
		if (!PugiGetSubNodeValue(data_element, "need_count", need_count) || need_count < 0 )
		{
			return -98;
		}

		if (suit_cfg.suit_count < 0 || suit_cfg.suit_count >= SHEN_YIN_SUIT_CFG_MAX_COUNT)
		{
			return -99;
		}

		ShenYinNewSuitAttrCfg &attr_cfg = suit_cfg.attr[suit_cfg.suit_count];

		attr_cfg.need_count = need_count;

		if (!PugiGetSubNodeValue(data_element, "maxhp", attr_cfg.maxhp) || attr_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", attr_cfg.gongji) || attr_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", attr_cfg.fangyu) || attr_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", attr_cfg.mingzhong) || attr_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", attr_cfg.shanbi) || attr_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", attr_cfg.baoji) || attr_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", attr_cfg.jianren) || attr_cfg.jianren < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "per_gongji", attr_cfg.per_gongji) || attr_cfg.per_gongji < 0)
		{
			return -108;
		}

		if (!PugiGetSubNodeValue(data_element, "per_maxhp", attr_cfg.per_maxhp) || attr_cfg.per_maxhp < 0)
		{
			return -109;
		}

		if (!PugiGetSubNodeValue(data_element, "per_fangyu", attr_cfg.per_fangyu) || attr_cfg.per_fangyu < 0)
		{
			return -110;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_jianshang_per", attr_cfg.skill_jianshang_per) || attr_cfg.skill_jianshang_per < 0)
		{
			return -111;
		}

		++suit_cfg.suit_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

const ShenYinNewSuitAttrCfg * ShenYinConfig::GetSuitCfg(int color, int count) const
{
	if (color < 0 || color >= SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK + 1)
	{
		return NULL;
	}

	if (m_suit_list[color].suit_count > 0)
	{
		for (int i = m_suit_list[color].suit_count - 1; i >= 0 && i < SHEN_YIN_SUIT_CFG_MAX_COUNT; i--)
		{
			if (count >= m_suit_list[color].attr[i].need_count)
			{
				return &m_suit_list[color].attr[i];
			}
		}
	}

	return NULL;
}