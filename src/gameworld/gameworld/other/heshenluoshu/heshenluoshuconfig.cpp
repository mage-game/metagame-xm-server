//#include "stdafx.h"

#include "heshenluoshuconfig.hpp"
#include "item/itempool.h"


HeShenLuoShuConfig::HeShenLuoShuConfig()
{
	memset(&m_item_attr_cfg, 0, sizeof(m_item_attr_cfg));
	memset(&m_item_decompose_cfg, 0, sizeof(m_item_decompose_cfg));
}

HeShenLuoShuConfig::~HeShenLuoShuConfig()
{
}

bool HeShenLuoShuConfig::Init(const std::string & configname, std::string * err)
{
	YY_XML_CONFIG_PRE_LOAD;
	YY_XML_LOAD_CONFIG("other", InitOtherCfg);								// ∆‰À˚≈‰÷√
	YY_XML_LOAD_CONFIG("suite_attr", InitHeShenLuoShuSuiteAttr);			// Ã◊◊∞≈‰÷√
	YY_XML_LOAD_CONFIG("upgrade_star", InitHeShenLuoShuItemStar);			// Õºº¯–«–«≈‰÷√
	YY_XML_LOAD_CONFIG("item_decompose", InitHeShenLuoShuItemDecompose);	// Õºº¯…ÒªØ∑÷Ω‚≈‰÷√
	YY_XML_LOAD_CONFIG("shenhua_add", InitHeShenLuoShuShenHuaAdd);	// Õºº¯∑÷Ω‚≈‰÷√

	return true;
}

int HeShenLuoShuConfig::InitOtherCfg(YY_XML_NODE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");

	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "open_level", m_other_cfg.open_level) || m_other_cfg.open_level <= 0)
	{
		return -1;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "max_star_level", m_other_cfg.max_star_level) || m_other_cfg.max_star_level <= 0 || m_other_cfg.max_star_level > HESHENLUOSHU_ITEM_MAX_LEVEL)
	{
		return -2;
	}

	return 0;
}

int HeShenLuoShuConfig::InitHeShenLuoShuSuiteAttr(YY_XML_NODE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");

	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	int type = 0;
	int seq = 0;
	int last_seq = -1;
	int cfg_num = 0;
	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "type", type) || type < 0 || type >= HESHENLUOSHU_MAX_TYPE)
		{
			return -1;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "seq", seq) || seq < 0 || seq >= HESHENLUOSHU_MAX_SEQ)
		{
			return -2;
		}

		if (seq != last_seq)
		{
			cfg_num = 0;
			last_seq = seq;
		}

		if (cfg_num < 0 || cfg_num >= HESHENLUOSHU_SUIT_MAX_NUM)
		{
			return -3;
		}

		HeShenLuoShuSuiteAttr::SuiteAttr &suit_attr = m_suite_attr_list[type][seq].suit_attr[cfg_num];

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "suit_num", suit_attr.suit_num) || suit_attr.suit_num <= 0 || suit_attr.suit_num > HESHENLUOSHU_MAX_INDEX)
		{
			return -4;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "max_hp", suit_attr.maxhp) || suit_attr.maxhp < 0) return -11;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "gong_ji", suit_attr.gongji) || suit_attr.gongji < 0) return -12;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "fang_yu", suit_attr.fangyu) || suit_attr.fangyu < 0) return -13;
		//if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "fa_fangyu", suit_attr.fa_fangyu) || suit_attr.fa_fangyu < 0) return -14;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "per_baoji", suit_attr.per_baoji) || suit_attr.per_baoji < 0) return -15;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "bao_ji", suit_attr.baoji) || suit_attr.baoji < 0) return -16;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "jian_ren", suit_attr.per_kangbao) || suit_attr.per_kangbao < 0) return -17;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "constant_zengshang", suit_attr.fujiashanghai) || suit_attr.fujiashanghai < 0) return -18;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "constant_mianshang", suit_attr.dikangshanghai) || suit_attr.dikangshanghai < 0) return -19;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "per_jingzhun", suit_attr.pojia) || suit_attr.pojia < 0) return -20;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "skill_jianshang_per", suit_attr.skill_jianshang_per) || suit_attr.skill_jianshang_per < 0) return -21;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "per_pofang", suit_attr.per_pofang) || suit_attr.per_pofang < 0) return -22;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "per_mianshang", suit_attr.per_mianshang) || suit_attr.per_mianshang < 0) return -23;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "skill_zengshang_per", suit_attr.skill_zengshang_per) || suit_attr.skill_zengshang_per < 0) return -24;

		++cfg_num;
		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

int HeShenLuoShuConfig::InitHeShenLuoShuItemStar(YY_XML_NODE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");

	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	ItemID item_id = 0;
	ItemID last_item_id = 0;
	int star_level = 0;
	int color = 0;
	HeShenLuoShuItemStar item_star;
	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "star_level", star_level) || star_level < 0 || star_level > HESHENLUOSHU_ITEM_MAX_LEVEL)
		{
			if (star_level < 0)
			{
				data_element = YY_XML_NEXT_SIBLING(data_element);
				continue;
			}
			else
			{
				return -1;
			}
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "item_id", item_id) || NULL == ITEMPOOL->GetItem(item_id))
		{
			return -2;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "color", color) || color < 0)
		{
			return -20;
		}

		if (item_id != last_item_id)
		{
			if (m_item_star_cfg_map.find(last_item_id) != m_item_star_cfg_map.end())
			{
				return -100;
			}

			m_item_star_cfg_map[last_item_id] = item_star;
			last_item_id = item_id;
		}

		if (star_level == 0)
		{
			if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "type", item_star.item_pos.type) || item_star.item_pos.type < 0 || item_star.item_pos.type >= HESHENLUOSHU_MAX_TYPE) return -3;
			if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "seq", item_star.item_pos.seq) || item_star.item_pos.seq < 0 || item_star.item_pos.seq >= HESHENLUOSHU_MAX_SEQ) return -4;
			if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "index", item_star.item_pos.index) || item_star.item_pos.index < 0 || item_star.item_pos.index >= HESHENLUOSHU_MAX_INDEX) return -5;

			m_item_attr_cfg[item_star.item_pos.type][item_star.item_pos.seq][item_star.item_pos.index] = item_id;
		}

		HeShenLuoShuItemStar::ItemStarCfg &item_star_cfg = item_star.shu_item_cfg[star_level];
		item_star_cfg.star_level = star_level;
		item_star_cfg.color = color;

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "item_id_1", item_star_cfg.stuff_1) || NULL == ITEMPOOL->GetItem(item_star_cfg.stuff_1)) return -6;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "item_id_2", item_star_cfg.stuff_2) || NULL == ITEMPOOL->GetItem(item_star_cfg.stuff_2)) return -7;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "item_id_3", item_star_cfg.stuff_3) || NULL == ITEMPOOL->GetItem(item_star_cfg.stuff_3)) return -8;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "item_id_4", item_star_cfg.stuff_4) || NULL == ITEMPOOL->GetItem(item_star_cfg.stuff_4)) return -9;

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "consume_jinghua", item_star_cfg.consume_jinghua) || item_star_cfg.consume_jinghua < 0) return -10;

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "max_hp", item_star_cfg.maxhp) || item_star_cfg.maxhp < 0) return -11;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "gong_ji", item_star_cfg.gongji) || item_star_cfg.gongji < 0) return -12;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "fang_yu", item_star_cfg.fangyu) || item_star_cfg.fangyu < 0) return -13;
		//if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "fa_fangyu", item_star_cfg.fa_fangyu) || item_star_cfg.fa_fangyu < 0) return -14;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "per_baoji", item_star_cfg.per_baoji) || item_star_cfg.per_baoji < 0) return -15;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "bao_ji", item_star_cfg.baoji) || item_star_cfg.baoji < 0) return -16;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "jian_ren", item_star_cfg.per_kangbao) || item_star_cfg.per_kangbao < 0) return -17;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "constant_zengshang", item_star_cfg.fujiashanghai) || item_star_cfg.fujiashanghai < 0) return -18;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "constant_mianshang", item_star_cfg.dikangshanghai) || item_star_cfg.dikangshanghai < 0) return -19;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "per_jingzhun", item_star_cfg.pojia) || item_star_cfg.pojia < 0) return -19;

		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	if (m_item_star_cfg_map.find(item_id) != m_item_star_cfg_map.end())
	{
		return -100;
	}

	m_item_star_cfg_map[item_id] = item_star;

	return 0;
}

int HeShenLuoShuConfig::InitHeShenLuoShuItemDecompose(YY_XML_NODE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");

	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}
	int item_quality = 0;
	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "item_quality", item_quality) || item_quality <= ItemBase::I_COLOR_INVALID || item_quality >= ItemBase::I_COLOR_MAX)
		{
			return -1;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "obtain_num", m_item_decompose_cfg[item_quality]) || m_item_decompose_cfg[item_quality] < 0)
		{
			return -2;
		}

		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

int HeShenLuoShuConfig::InitHeShenLuoShuShenHuaAdd(YY_XML_NODE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");

	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}
	int type = 0;
	int seq = 0;
	int level = 0;
	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "type", type) || type < 0 || type >= HESHENLUOSHU_MAX_TYPE)
		{
			return -1;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "seq", seq) || seq < 0 || seq >= HESHENLUOSHU_MAX_SEQ)
		{
			return -2;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "level", level) || level < 0 || level > HESHENLUOSHU_HUASHEN_MAX_LEVEL)
		{
			return -3;
		}

		HeShenLuoShuShenHuaAdd shen_hua_add_cfg;


		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "per_add", shen_hua_add_cfg.per_add) || shen_hua_add_cfg.per_add < 0 || shen_hua_add_cfg.per_add >= 10000)
		{
			return -4;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "exp", shen_hua_add_cfg.exp) || shen_hua_add_cfg.exp <= 0)
		{
			return -5;
		}

		m_shen_hua_add_cfg[type][seq][level] = shen_hua_add_cfg;

		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}




const HeShenLuoShuSuiteAttr::SuiteAttr * HeShenLuoShuConfig::GetHeShenLuoShuSuiteAttr(int type, int seq, int suit_num) const
{
	if (type < 0 || type >= HESHENLUOSHU_MAX_TYPE || seq < 0 || seq >= HESHENLUOSHU_MAX_SEQ)
	{
		return NULL;
	}

	for (int index = HESHENLUOSHU_SUIT_MAX_NUM - 1; index >= 0; index--)
	{
		if (m_suite_attr_list[type][seq].suit_attr[index].suit_num > 0 && m_suite_attr_list[type][seq].suit_attr[index].suit_num <= suit_num)
		{
			return  &m_suite_attr_list[type][seq].suit_attr[index];
		}
	}

	return NULL;
}

const HeShenLuoShuItemStar::ItemPos * HeShenLuoShuConfig::GetHeShenLuoShuItemPos(ItemID item_id) const
{
	HeShenLuoShuItemStarMapIt it = m_item_star_cfg_map.find(item_id);
	if (it != m_item_star_cfg_map.end())
	{
		return &it->second.item_pos;
	};
	return NULL;
}

const HeShenLuoShuItemStar::ItemStarCfg * HeShenLuoShuConfig::GetHeShenLuoShuItemStarCfg(ItemID item_id, int star_level) const
{
	if (star_level < 0 || star_level > HESHENLUOSHU_ITEM_MAX_LEVEL)
	{
		return NULL;
	}
	HeShenLuoShuItemStarMapIt it = m_item_star_cfg_map.find(item_id);
	if (it != m_item_star_cfg_map.end())
	{
		return &it->second.shu_item_cfg[star_level];
	};
	return NULL;
}

const HeShenLuoShuItemStar::ItemStarCfg * HeShenLuoShuConfig::GetHeShenLuoShuItemStarCfg(int type, int seq, int index, int star_level) const
{
	if (type < 0 || type >= HESHENLUOSHU_MAX_TYPE || seq < 0 || seq >= HESHENLUOSHU_MAX_SEQ ||
		index < 0 || index >= HESHENLUOSHU_MAX_INDEX || star_level < 0 || star_level > HESHENLUOSHU_ITEM_MAX_LEVEL)
	{
		return NULL;
	}

	ItemID item_id = m_item_attr_cfg[type][seq][index];

	HeShenLuoShuItemStarMapIt it = m_item_star_cfg_map.find(item_id);
	if (it != m_item_star_cfg_map.end())
	{
		return &it->second.shu_item_cfg[star_level];
	};
	return NULL;
}

const HeShenLuoShuItemStar * HeShenLuoShuConfig::GetHeShenLuoShuItemStarAllCfg(ItemID item_id) const
{
	HeShenLuoShuItemStarMapIt it = m_item_star_cfg_map.find(item_id);
	if (it != m_item_star_cfg_map.end())
	{
		return &it->second;
	};
	return NULL;
}

int HeShenLuoShuConfig::GetHeShenLuoShuItemDecompose(int item_color)
{
	if (ItemBase::I_COLOR_INVALID < item_color && item_color < ItemBase::I_COLOR_MAX)
	{
		return m_item_decompose_cfg[item_color];
	}

	return  0;
}

const HeShenLuoShuShenHuaAdd * HeShenLuoShuConfig::GetHeShenLuoShuShenHuaAdd(int type, int seq, int level) const
{
	if (type < 0 || type >= HESHENLUOSHU_MAX_TYPE || seq < 0 || seq >= HESHENLUOSHU_MAX_SEQ ||
		level < 0 || level > HESHENLUOSHU_HUASHEN_MAX_LEVEL)
	{
		return NULL;
	}
	return &m_shen_hua_add_cfg[type][seq][level];
}
