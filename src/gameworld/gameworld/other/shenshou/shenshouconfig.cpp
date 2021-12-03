#include "shenshouconfig.hpp"
#include "servercommon/shenshoudef.hpp"

#include "servercommon/configcommon.h"
#include "item/itempool.h"

#include "common/tinyxml/tinyxml.h"

#include <vector>

ShenshouConfig::ShenshouConfig(): m_max_equip_id(0), m_max_extend_num(0), m_shenshou_max_id(0), m_compose_cfg_count(0), m_huanling_pool_total_refresh_weight(0)
{
}

ShenshouConfig::~ShenshouConfig()
{
}

bool ShenshouConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// 其他配置
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	// 装配品质配置
	PUGI_XML_LOAD_CONFIG("equip_quality", InitQualityCfg);
	// 品质属性配置
	PUGI_XML_LOAD_CONFIG("equip_attr", InitEquipAttrCfg);
	// 装备配置
	PUGI_XML_LOAD_CONFIG("equip_cfg", InitEquipCfg);
	// 装备基础属性
	PUGI_XML_LOAD_CONFIG("equip_base_attr", InitEquipBaseAttrCfg);
	// 装备等级配置
	PUGI_XML_LOAD_CONFIG("equip_level", InitEquipLevelCfg);
	// 助战位配置
	PUGI_XML_LOAD_CONFIG("extra_num_cfg", InitZhuzhanCfg);
	// 技能配置
	PUGI_XML_LOAD_CONFIG("skill_cfg", InitSkillCfg);
	// 神兽配置
	PUGI_XML_LOAD_CONFIG("shou_cfg", InitShenshouCfg);
	// 部位品质要求配置
	PUGI_XML_LOAD_CONFIG("slot_need_quality_cfg", InitSlotNeedQualityCfg);
	// 神兽合成配置
	PUGI_XML_LOAD_CONFIG("compose_cfg", InitComposeCfg);
	// 唤灵档位配置
	PUGI_XML_LOAD_CONFIG("huanling_pool_cfg", InitHuanlingPoolCfg);
	// 唤灵刷新配置
	PUGI_XML_LOAD_CONFIG("huanling_refresh_cfg", InitHuanlingRefreshCfg);
	// 唤灵抽奖配置
	PUGI_XML_LOAD_CONFIG("huanling_draw_cfg", InitHuanlingDrawCfg);

	return true;
}

const ShenshouItemCfg * ShenshouConfig::GetItemInfo(int item_id) const
{
	if (item_id <= 0 || item_id > SHENSHOU_MAX_EQUIP_ID || item_id > m_max_equip_id)
	{
		return NULL;
	}

	return &m_equip_cfg_list[item_id];
}

const ShenshouItemCfg * ShenshouConfig::GetItemInfo(ItemID real_id, int item_id) const
{
	if (nullptr == ITEMPOOL->GetItem(real_id))
	{
		return nullptr;
	}

	for (int i = 0; i <= m_max_equip_id && i < SHENSHOU_MAX_EQUIP_ID; ++i)
	{
		if (real_id == m_equip_cfg_list[i].real_id)
		{
			return &m_equip_cfg_list[i];
		}
	}
	return nullptr;
}

const ShenshouInfoCfg * ShenshouConfig::GetShenshouInfo(int shou_id) const
{
	if (shou_id <= 0 || shou_id > m_shenshou_max_id || shou_id > SHENSHOU_MAX_ID)
	{
		return NULL;
	}

	return &m_shenshou_cfg_list[shou_id];
}

const ShenshouEquipQualityCfg * ShenshouConfig::GetEquipQualityCfg(int quality) const
{
	if (quality < 0 || quality > SHENSHOU_MAX_QUALITY)
	{
		return NULL;
	}

	return &m_equip_quality_cfg_list[quality];
}

const ShenshouAttrCfg *ShenshouEquipQualityCfg::GetRandomNormalAttr(const std::set<int> &attr_set) const
{
	std::vector<int> tmp_atrr_set;
	for (int i = 0; i < normal_attr_num && i < MAX_NORMAL_ATTR_CFG_COUNT; ++ i)
	{
		if (attr_set.find(normal_attr_list[i].attr_type) != attr_set.end())
		{
			continue;
		}
		
		tmp_atrr_set.push_back(i);
	}

	if (tmp_atrr_set.size() <= 0)
	{
		return NULL;
	}

	int ra_index = RandomNum(static_cast<int>(tmp_atrr_set.size()));
	return &normal_attr_list[tmp_atrr_set[ra_index]];
}

const ShenshouAttrCfg *ShenshouEquipQualityCfg::GetRandomStarAttr(const std::set<int> &attr_set) const
{
	std::vector<int> tmp_index_set;
	for (int i = 0; i < star_attr_num && i < MAX_STAR_ATTR_CFG_COUNT; ++i)
	{
		if (attr_set.find(star_attr_list[i].attr_type) != attr_set.end())
		{
			continue;
		}

		tmp_index_set.push_back(i);
	}

	if (tmp_index_set.size() <= 0)
	{
		return NULL;
	}

	int ra_index = RandomNum(static_cast<int>(tmp_index_set.size()));
	return &star_attr_list[tmp_index_set[ra_index]];
}

int ShenshouEquipQualityCfg::GetRandomStartNum() const
{
	int rand_weight = RandomNum(total_weight);
	for (std::map<int, int>::const_iterator it = weight_list_map.begin(); it != weight_list_map.end(); ++ it)
	{
		if (rand_weight < it->second)
		{
			return it->first;
		}

		rand_weight -= it->second;
	}

	return 0;
}

const ShenshouEquipQualityCfg::LevelShulianduCfg * ShenshouConfig::GetEquipShulianduCfg(int equip_slot_index, int strength_level) const
{
	if (equip_slot_index < 0 || equip_slot_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX || strength_level < 0 || strength_level > SHENSHOU_MAX_EQUIP_STRENGTH_LEVEL)
	{
		return NULL;
	}

	// 后面改成不需要品质了，不同品质的部位等级属性都一样，数据结构不去改了，就这样存吧
	return &m_equip_quality_cfg_list[0].level_cfg_list[equip_slot_index][strength_level];
}

int ShenshouConfig::GetEquipShulianduMax(int equip_slot_index)
{
	if (equip_slot_index < 0 || equip_slot_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX)
	{
		return -1;
	}

	short MAX_LEVEL = m_equip_quality_cfg_list[0].max_equip_strength_level;;
	const ShenshouEquipQualityCfg::LevelShulianduCfg *tmp_shuliandu_cfg = GetEquipShulianduCfg(equip_slot_index, MAX_LEVEL);
	if (NULL == tmp_shuliandu_cfg)
	{
		return -2;
	}

	return tmp_shuliandu_cfg->contain_shulian;;
}

const ShenshouEquipQualityCfg::BaseAttrduCfg * ShenshouConfig::GetEquipBaseAttrCfg(int quality, int equip_slot_index) const
{
	if (quality < 0 || quality > SHENSHOU_MAX_QUALITY || equip_slot_index < 0 || equip_slot_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX)
	{
		return NULL;
	}

	return &m_equip_quality_cfg_list[quality].base_attr_cfg_list[equip_slot_index];
}

const ShenshouSkillCfg::LevelCfg * ShenshouConfig::GetSkillLevelCfg(int skill_type, int skill_level) const
{
	if (skill_type <= SHENSHOU_SKILL_TYPE_MIN_INVALID || skill_type >= SHENSHOU_SKILL_TYPE_COUNT)
	{
		return NULL;
	}

	if (skill_level <= 0 || skill_level > m_skill_cfg_list[skill_type].max_level || skill_level > SHENSHOU_MAX_SKILL_LEVEL)
	{
		return NULL;
	}

	return &m_skill_cfg_list[skill_type].level_cfg_list[skill_level];
}

const ShenshouZhuzhanCfg * ShenshouConfig::GetExtraZhuzhanCfg(int extra_zhuzhan_count) const
{
	if (extra_zhuzhan_count <= 0 || extra_zhuzhan_count > SHENSHOU_MAX_EXTEND_NUM || extra_zhuzhan_count > m_max_extend_num)
	{
		return NULL;
	}

	return &m_zhuzhan_consume_cfg_list[extra_zhuzhan_count];
}

const ShenshouComposeCfg * ShenshouConfig::GetComposeCfg(int v_item_id) const
{
	for (int index = 0; index < m_compose_cfg_count; ++ index)
	{
		if (m_shenshou_compose_cfg_list[index].v_item_id == v_item_id)
		{
			return &m_shenshou_compose_cfg_list[index];
		}
	}

	return NULL;
}

int ShenshouConfig::GetHuanlingDrawConsumeScore(int draw_times)
{
	if (draw_times < 0 || draw_times >= static_cast<int>(m_huanling_draw_cfg_vec.size()))
	{
		return -1;
	}

	return m_huanling_draw_cfg_vec[draw_times].score;
}

bool ShenshouConfig::GetHuanlingRefreshItem(int &refresh_times, HuanlingListInfo seq_list[SHENSHOU_MAX_RERFESH_ITEM_COUNT], int item_count)
{
	if (SHENSHOU_MAX_RERFESH_ITEM_COUNT != item_count)
	{
		return false;
	}

	int pool_list[SHENSHOU_MAX_RERFESH_ITEM_COUNT] = { -1 };
	
	int dupilcate_flag = 0;

	// 得到10个物品的刷新档位
	for (int i = 0; i < item_count; ++i)
	{
		int seq = this->HuanlingRefreshPoolHelper(refresh_times, dupilcate_flag);
		if (seq >= 0)
		{
			pool_list[i] = seq;
		}
		++refresh_times;
	}

	// 遍历pool_list,找到对应档位的物品,进行刷新
	for (int i = 0; i < item_count; ++i)
	{
		int pool_index = pool_list[i];
		if (pool_index < 0 || pool_index >= static_cast<int>(m_huanling_pool_cfg_vec.size()))
		{
			continue;
		}

		if (m_huanling_pool_cfg_vec[pool_index].item_total_refresh_weight < 0)
		{
			continue;
		}

		const std::map<int, HuanlingRefreshItemCfg> &item_map = m_huanling_pool_cfg_vec[pool_index].item_map;
		int rand_weight = RandomNum(m_huanling_pool_cfg_vec[pool_index].item_total_refresh_weight);
		for (std::map<int, HuanlingRefreshItemCfg>::const_iterator iter = item_map.begin(); iter != item_map.end(); ++iter)
		{
			if (rand_weight < iter->second.refresh_weight)
			{
				seq_list[i].huanling_seq = iter->second.seq;
				seq_list[i].pool_index = pool_index;
				seq_list[i].draw = 0;
				break;
			}
			else
			{
				rand_weight -= iter->second.refresh_weight;
			}
		}
	}

	return true;
}

const ItemConfigData * ShenshouConfig::GetHuanlingDrawItem(short &draw_times, HuanlingListInfo cur_list[SHENSHOU_MAX_RERFESH_ITEM_COUNT], int &item_seq, bool &is_broadcast, int list_len)
{
	if (draw_times < 0 || draw_times >= SHENSHOU_MAX_DRAW_TIMES || draw_times >= m_other_cfg.huanling_draw_limit
		|| draw_times >= static_cast<int>(m_huanling_draw_cfg_vec.size()))
	{
		return NULL;
	}

	if (SHENSHOU_MAX_RERFESH_ITEM_COUNT != list_len)
	{
		return NULL;
	}

	//先随机出档位
	const HuanlingDrawCfg &draw_cfg = m_huanling_draw_cfg_vec[draw_times];
	const std::vector<int> pool_weight_vec = draw_cfg.draw_pool_weight_vec;

	std::map<int, int> cur_pool_weight_map;			//当前所有档位的刷新权重

	//把当前刷新列表中没有的档位去掉
	int cur_draw_total_weight = 0;
	for (int i = 0; i < list_len; ++i)
	{
		if (1 == cur_list[i].draw || cur_pool_weight_map.find(cur_list[i].pool_index) != cur_pool_weight_map.end())
		{
			continue;
		}
		else
		{
			cur_pool_weight_map[cur_list[i].pool_index] = pool_weight_vec[cur_list[i].pool_index];
			cur_draw_total_weight += pool_weight_vec[cur_list[i].pool_index];
		}
	}

	if (cur_draw_total_weight <= 0)
	{
		return NULL;
	}

	//根据权重找到对应档位
	int pool_index = -1;
	int pool_rand_weight = RandomNum(cur_draw_total_weight);
	for (std::map<int, int>::iterator iter = cur_pool_weight_map.begin(); iter != cur_pool_weight_map.end(); ++iter)
	{
		if (pool_rand_weight < iter->second)
		{
			pool_index = iter->first;
			break;
		}
		else
		{
			pool_rand_weight -= iter->second;
		}
	}

	if (pool_index < 0 && pool_index >= static_cast<int>(m_huanling_pool_cfg_vec.size()))
	{
		return NULL;
	}

	//再随机出物品
	{
		int cur_list_item_total_weight = 0;										//当前刷新列表物品总权重
		for (int i = 0; i < list_len; ++i)
		{
			if (cur_list[i].pool_index != pool_index || cur_list[i].draw)
			{
				continue;
			}

			const HuanlingPoolCfg &pool_cfg = m_huanling_pool_cfg_vec[pool_index];
			std::map<int, HuanlingRefreshItemCfg>::const_iterator item_map_iter = pool_cfg.item_map.find(cur_list[i].huanling_seq);	//根据seq找到对应物品
			if (item_map_iter != pool_cfg.item_map.end())
			{
				cur_list_item_total_weight += item_map_iter->second.draw_weight;
			}
		}

		if (cur_list_item_total_weight <= 0)
		{
			return NULL;
		}

		int item_rand_weight = RandomNum(cur_list_item_total_weight);
		for (int i = 0; i < list_len; ++i)
		{
			if (cur_list[i].pool_index != pool_index || cur_list[i].draw)
			{
				continue;
			}

			// 根据pool_index和seq找到对应物品
			const HuanlingPoolCfg &pool_cfg = m_huanling_pool_cfg_vec[pool_index];
			std::map<int, HuanlingRefreshItemCfg>::const_iterator item_map_iter = pool_cfg.item_map.find(cur_list[i].huanling_seq);	//根据seq找到对应物品
			if (item_map_iter != pool_cfg.item_map.end())
			{
				if (item_rand_weight < item_map_iter->second.draw_weight)
				{
					cur_list[i].draw = 1;
					item_seq = i;
					is_broadcast = item_map_iter->second.is_broadcast;
					return &item_map_iter->second.item;
				}
				else
				{
					item_rand_weight -= item_map_iter->second.draw_weight;
				}
			}
		}

	}

	return NULL;
}

int ShenshouConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	if (!PugiGetSubNodeValue(data_element, "need_role_level", m_other_cfg.need_role_level) || m_other_cfg.need_role_level <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "equip_double_shulian_per_gold", m_other_cfg.equip_double_shulian_per_gold) || m_other_cfg.equip_double_shulian_per_gold <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "default_zhuzhan_count", m_other_cfg.default_zhuzhan_count) || m_other_cfg.default_zhuzhan_count <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "huanling_draw_limit", m_other_cfg.huanling_draw_limit) || m_other_cfg.huanling_draw_limit <= 0 || m_other_cfg.huanling_draw_limit > SHENSHOU_MAX_DRAW_TIMES)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "huanling_refresh_consume", m_other_cfg.huanling_refresh_consume) || m_other_cfg.huanling_refresh_consume <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "start_limit_level", m_other_cfg.start_limit_level) || m_other_cfg.start_limit_level <= 0)
	{
		return -6;
	}

	//if (!PugiGetSubNodeValue(data_element, "opengame_day", m_other_cfg.opengame_day) || m_other_cfg.opengame_day <= 0)
	//{
	//	return -7;
	//}
	
	return 0;
}

int ShenshouConfig::InitQualityCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int pre_quality = -1;
	while (!data_element.empty())
	{
		int quality = 0;
		if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality != pre_quality + 1 || quality > SHENSHOU_MAX_QUALITY)
		{
			return -1;
		}
		pre_quality = quality;

		ShenshouEquipQualityCfg &quality_cfg = m_equip_quality_cfg_list[quality];

		if (!PugiGetSubNodeValue(data_element, "rand_attr_num", quality_cfg.rand_attr_num) || quality_cfg.rand_attr_num < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "star_attr_max_num", quality_cfg.star_attr_max_num) || quality_cfg.star_attr_max_num < 0)
		{
			return -3;
		}

		quality_cfg.total_weight = 0;
		for (int index = 0; index <= quality_cfg.star_attr_max_num; ++ index)
		{
			int &weight = quality_cfg.weight_list_map[index];

			char name[30] = {};
			SNPRINTF(name, sizeof(name), "weight_%d", index);
			if (!PugiGetSubNodeValue(data_element, name, weight) || weight < 0)
			{
				return - 10000 - index;
			}

			quality_cfg.total_weight += weight;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenshouConfig::InitEquipAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int quality = 0;
		if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality < 0 || quality > SHENSHOU_MAX_QUALITY)
		{
			return -1;
		}

		ShenshouEquipQualityCfg &quality_cfg = m_equip_quality_cfg_list[quality];
		quality_cfg.quality = quality;

		int is_star_attr = 0;
		if (!PugiGetSubNodeValue(data_element, "is_star_attr", is_star_attr) || is_star_attr < 0 || is_star_attr > 1)
		{
			return -2;
		}

		ShenshouAttrCfg *attr_cfg;
		if (0 == is_star_attr) // 普通属性
		{
			if (quality_cfg.normal_attr_num >= ShenshouEquipQualityCfg::MAX_NORMAL_ATTR_CFG_COUNT)
			{
				return -3;
			}

			attr_cfg = &quality_cfg.normal_attr_list[quality_cfg.normal_attr_num++];
		}
		else // 珍稀属性
		{
			if (quality_cfg.star_attr_num >= ShenshouEquipQualityCfg::MAX_STAR_ATTR_CFG_COUNT)
			{
				return -4;
			}

			attr_cfg = &quality_cfg.star_attr_list[quality_cfg.star_attr_num++];
		}

		if (!PugiGetSubNodeValue(data_element, "attr_type", attr_cfg->attr_type) || attr_cfg->attr_type <= SHENSHOU_ATTR_TYPE_INVALID_MIN || attr_cfg->attr_type >= SHENSHOU_ATTR_TYPE_INVALID_MAX)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "attr_value", attr_cfg->attr_value) || attr_cfg->attr_value < 0)
		{
			return -6;
		}

		data_element = data_element.next_sibling();
	}

	for (int quality = 0; quality <= SHENSHOU_MAX_QUALITY; ++quality)
	{
		ShenshouEquipQualityCfg &quality_cfg = m_equip_quality_cfg_list[quality];

		// 要求所有品质配置都要有配
		if (quality_cfg.quality != quality)
		{
			return -2001;
		}

		if ((quality_cfg.rand_attr_num > 0 && quality_cfg.normal_attr_num < quality_cfg.rand_attr_num) || // 普通属性一定要有，星级属性看情况
			(quality_cfg.star_attr_max_num > 0 && quality_cfg.star_attr_num < quality_cfg.star_attr_max_num))
		{
			return -2000;
		}
	}

	return 0;
}

int ShenshouConfig::InitEquipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int item_id = 0;
		if (!PugiGetSubNodeValue(data_element, "item_id", item_id) || item_id != m_max_equip_id + 1 || item_id > SHENSHOU_MAX_EQUIP_ID)
		{
			return -1;
		}
		m_max_equip_id = item_id;

		ShenshouItemCfg &equip_cfg = m_equip_cfg_list[item_id];
		equip_cfg.item_id = item_id;

		if (!PugiGetSubNodeValue(data_element, "real_id", equip_cfg.real_id)|| nullptr == ITEMPOOL->GetItem(equip_cfg.real_id)) 
		{
			return -100;
		}

		int is_equip = 0;
		if (!PugiGetSubNodeValue(data_element, "is_equip", is_equip) || is_equip < 0 || is_equip > 1)
		{
			return -2;
		}
		equip_cfg.is_equip = (is_equip > 0);

		if (!PugiGetSubNodeValue(data_element, "quality", equip_cfg.quality) || equip_cfg.quality < 0 || equip_cfg.quality > SHENSHOU_MAX_QUALITY)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "slot_index", equip_cfg.slot_index) || equip_cfg.slot_index < 0 || equip_cfg.slot_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "contain_shulian", equip_cfg.contain_shulian) || equip_cfg.contain_shulian < 0)
		{
			return -6;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenshouConfig::InitEquipBaseAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int quality = 0;
		if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality < 0 || quality > SHENSHOU_MAX_QUALITY)
		{
			return -1;
		}

		ShenshouEquipQualityCfg &quality_cfg = m_equip_quality_cfg_list[quality];
		quality_cfg.quality = quality;

		int slot_index = 0;
		if (!PugiGetSubNodeValue(data_element, "slot_index", slot_index) || slot_index < 0 || slot_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX)
		{
			return -2;
		}

		ShenshouEquipQualityCfg::BaseAttrduCfg &level_cfg = quality_cfg.base_attr_cfg_list[slot_index];
		level_cfg.slot_index = slot_index;

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -101;
		}
		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -102;
		}
		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -103;
		}
		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -104;
		}
		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -105;
		}
		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -106;
		}
		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -107;
		}
		if (!PugiGetSubNodeValue(data_element, "constant_zengshang", level_cfg.constant_zengshang) || level_cfg.constant_zengshang < 0)
		{
			return -108;
		}
		if (!PugiGetSubNodeValue(data_element, "constant_mianshang", level_cfg.constant_mianshang) || level_cfg.constant_mianshang < 0)
		{
			return -109;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenshouConfig::InitEquipLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
//		int quality = 0;
//		if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality < 0 || quality > SHENSHOU_MAX_QUALITY)
//		{
//			return -1;
//		}

		// 后面改成不需要品质了，不同品质的部位等级属性都一样，数据结构不去改了，就这样存吧
		ShenshouEquipQualityCfg &quality_cfg = m_equip_quality_cfg_list[0];
		quality_cfg.quality = 0;

		int slot_index = 0;
		if (!PugiGetSubNodeValue(data_element, "slot_index", slot_index) || slot_index < 0 || slot_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX)
		{
			return -2;
		}
		int strength_level = 0;
		if (!PugiGetSubNodeValue(data_element, "strength_level", strength_level) || strength_level < 0 || strength_level > SHENSHOU_MAX_EQUIP_STRENGTH_LEVEL)
		{
			return -3;
		}

		if (strength_level > quality_cfg.max_equip_strength_level)
		{
			quality_cfg.max_equip_strength_level = strength_level;
		}
		
		ShenshouEquipQualityCfg::LevelShulianduCfg &level_cfg = quality_cfg.level_cfg_list[slot_index][strength_level];
		level_cfg.slot_index = slot_index;
		level_cfg.level = strength_level;
		
		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -101;
		}
		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -102;
		}
		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -103;
		}
		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -104;
		}
		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -105;
		}
		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -106;
		}
		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -107;
		}
		if (!PugiGetSubNodeValue(data_element, "constant_zengshang", level_cfg.constant_zengshang) || level_cfg.constant_zengshang < 0)
		{
			return -108;
		}
		if (!PugiGetSubNodeValue(data_element, "constant_mianshang", level_cfg.constant_mianshang) || level_cfg.constant_mianshang < 0)
		{
			return -109;
		}

		if (!PugiGetSubNodeValue(data_element, "upgrade_need_shulian", level_cfg.upgrade_need_shulian) || level_cfg.upgrade_need_shulian < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "contain_shulian", level_cfg.contain_shulian) || level_cfg.contain_shulian < 0)
		{
			return -5;
		}
		
		data_element = data_element.next_sibling();
	}

	//for (int quality = 0; quality <= SHENSHOU_MAX_QUALITY; ++quality)
	//{
		ShenshouEquipQualityCfg &quality_cfg = m_equip_quality_cfg_list[0]; // 同上

		// 要求所有slot跟level配置都要有配
		for (int slot_index = 0; slot_index <= SHENSHOU_MAX_EQUIP_SLOT_INDEX; ++slot_index)
		{
			for (int strength_level = 1; strength_level <= quality_cfg.max_equip_strength_level && strength_level <= SHENSHOU_MAX_EQUIP_STRENGTH_LEVEL; ++ strength_level)
			{
				if (quality_cfg.level_cfg_list[slot_index][strength_level].upgrade_need_shulian <= 0 ||
					quality_cfg.level_cfg_list[slot_index][strength_level].contain_shulian <= 0)
				{
					return -2001;
				}
			}
		}
	//}

	return 0;
}

int ShenshouConfig::InitZhuzhanCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int extra_num = 0;
		if (!PugiGetSubNodeValue(data_element, "extra_num", extra_num) || extra_num != m_max_extend_num + 1 || extra_num > SHENSHOU_MAX_EXTEND_NUM)
		{
			return -1;
		}
		m_max_extend_num = extra_num;

		ShenshouZhuzhanCfg &zhuzhan_cfg = m_zhuzhan_consume_cfg_list[extra_num];

		if (!PugiGetSubNodeValue(data_element, "stuff_id", zhuzhan_cfg.stuff_id) || NULL == ITEMPOOL->GetItem(zhuzhan_cfg.stuff_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_num", zhuzhan_cfg.stuff_num) || zhuzhan_cfg.stuff_num <= 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenshouConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	char element_name_str[256];

	while (!data_element.empty())
	{
		int skill_type = 0;
		if (!PugiGetSubNodeValue(data_element, "skill_type", skill_type) || skill_type <= SHENSHOU_SKILL_TYPE_MIN_INVALID || skill_type >= SHENSHOU_SKILL_TYPE_COUNT)
		{
			return -1;
		}

		ShenshouSkillCfg &skill_cfg = m_skill_cfg_list[skill_type];
		if (skill_cfg.max_level >= SHENSHOU_MAX_SKILL_LEVEL)
		{
			return -2;
		}

		skill_cfg.skill_type = skill_type;

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != skill_cfg.max_level + 1)
		{
			return -4;
		}
		skill_cfg.max_level = level;

		ShenshouSkillCfg::LevelCfg &level_cfg = skill_cfg.level_cfg_list[level];
		level_cfg.level = level;
		
		for (int param_index = 0; param_index < ShenshouSkillCfg::PARAM_COUNT; ++param_index)
		{
			SNPRINTF(element_name_str, sizeof(element_name_str), "param_%d", param_index + 1);
			if (!PugiGetSubNodeValue(data_element, element_name_str, level_cfg.param_list[param_index]) || level_cfg.param_list[param_index] < 0)
			{
				level_cfg.param_list[param_index] = 0; // 读取不成功给个默认值
			}
		}

		data_element = data_element.next_sibling();
	}

	// 不做等级一致性检查了，允许不同技能不同的最大等级

	return 0;
}

int ShenshouConfig::InitShenshouCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	char element_name_str[256];

	while (!data_element.empty())
	{
		int shou_id = 0;
		if (!PugiGetSubNodeValue(data_element, "shou_id", shou_id) || shou_id != m_shenshou_max_id + 1 || shou_id > SHENSHOU_MAX_ID)
		{
			return -1;
		}
		m_shenshou_max_id = shou_id;

		ShenshouInfoCfg &shenshou_cfg = m_shenshou_cfg_list[shou_id];
		shenshou_cfg.shou_id = shou_id;

		if (!PugiGetSubNodeValue(data_element, "maxhp", shenshou_cfg.maxhp) || shenshou_cfg.maxhp < 0)
		{
			return -101;
		}
		if (!PugiGetSubNodeValue(data_element, "gongji", shenshou_cfg.gongji) || shenshou_cfg.gongji < 0)
		{
			return -102;
		}
		if (!PugiGetSubNodeValue(data_element, "fangyu", shenshou_cfg.fangyu) || shenshou_cfg.fangyu < 0)
		{
			return -103;
		}
		//if (!PugiGetSubNodeValue(data_element, "shanbi", shenshou_cfg.shanbi) || shenshou_cfg.shanbi < 0)
		//{
		//	return -104;
		//}
		//if (!PugiGetSubNodeValue(data_element, "mingzhong", shenshou_cfg.mingzhong) || shenshou_cfg.mingzhong < 0)
		//{
		//	return -105;
		//}

		for (int skill_id_index = 0; skill_id_index < ShenshouSkillCfg::PARAM_COUNT; ++skill_id_index)
		{
			ShenshouInfoCfg::SkillItem &item_cfg = shenshou_cfg.skill_list[skill_id_index];

			SNPRINTF(element_name_str, sizeof(element_name_str), "skill_id_%d", skill_id_index + 1);
			if (!PugiGetSubNodeValue(data_element, element_name_str, item_cfg.skill_type) ||
				item_cfg.skill_type <= SHENSHOU_SKILL_TYPE_MIN_INVALID || item_cfg.skill_type >= SHENSHOU_SKILL_TYPE_COUNT)
			{
				item_cfg.skill_type = 0; // 读取不成功给个默认值
			}
			else
			{
				SNPRINTF(element_name_str, sizeof(element_name_str), "skill_level_%d", skill_id_index + 1);
				if (!PugiGetSubNodeValue(data_element, element_name_str, item_cfg.skill_level) || item_cfg.skill_level <= 0)
				{
					return -8;
				}
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenshouConfig::InitSlotNeedQualityCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int shou_id = 0;
		if (!PugiGetSubNodeValue(data_element, "shou_id", shou_id) || shou_id <= 0 || shou_id > m_shenshou_max_id || shou_id > SHENSHOU_MAX_ID)
		{
			return -1;
		}

		ShenshouInfoCfg &shenshou_cfg = m_shenshou_cfg_list[shou_id];

		int slot = 0;
		if (!PugiGetSubNodeValue(data_element, "slot", slot) || slot < 0 || slot > SHENSHOU_MAX_EQUIP_SLOT_INDEX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "slot_need_quality", shenshou_cfg.slot_need_quality[slot]) || shenshou_cfg.slot_need_quality[slot] < 0 || shenshou_cfg.slot_need_quality[slot] > SHENSHOU_MAX_QUALITY)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenshouConfig::InitComposeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_compose_cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_compose_cfg_count >= SHENSHOU_MAX_COMPOSE_CFG)
		{
			return -100001;
		}

		ShenshouComposeCfg &cfg = m_shenshou_compose_cfg_list[m_compose_cfg_count];
		if (!PugiGetSubNodeValue(data_element, "v_item_id", cfg.v_item_id) || !this->GetItemInfo(cfg.v_item_id))
		{
			return -100002;
		}

		if (!PugiGetSubNodeValue(data_element, "need_qualit", cfg.need_qualit) || cfg.need_qualit < 0 || cfg.need_qualit > SHENSHOU_MAX_QUALITY)
		{
			return -100003;
		}

		if (!PugiGetSubNodeValue(data_element, "need_start_num", cfg.need_start_num) || cfg.need_start_num < 0 || cfg.need_start_num > SHENSHOU_MAX_EQUIP_ATTR_COUNT)
		{
			return -100004;
		}

		if (!PugiGetSubNodeValue(data_element, "give_quality", cfg.give_quality) || cfg.give_quality < 0 || cfg.give_quality > SHENSHOU_MAX_QUALITY)
		{
			return -100005;
		}

		if (!PugiGetSubNodeValue(data_element, "give_start_num", cfg.give_start_num) || cfg.give_start_num < 0 || cfg.give_start_num > SHENSHOU_MAX_EQUIP_ATTR_COUNT)
		{
			return -100006;
		}

		int is_need_item = 0;
		if (!PugiGetSubNodeValue(data_element, "is_need_item", is_need_item) || is_need_item < 0)
		{
			return -100007;
		}
		cfg.is_need_item = is_need_item != 0;

		int is_broatcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broatcast", is_broatcast) || is_broatcast < 0 )
		{
			return -100008;
		}
		cfg.is_broatcast = is_broatcast != 0;

		if (cfg.is_need_item)
		{
			if (!PugiGetSubNodeValue(data_element, "item_id", cfg.item_id) || !ITEMPOOL->GetItem(cfg.item_id))
			{
				return -100009;
			}

			if (!PugiGetSubNodeValue(data_element, "item_num", cfg.item_num) || cfg.item_num <= 0)
			{
				return -100010;
			}
		}

		++m_compose_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenshouConfig::InitHuanlingPoolCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		HuanlingPoolCfg cfg;

		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "refresh_pool", index) || index < 0 || index != static_cast<int>(m_huanling_pool_cfg_vec.size()))
		{
			return -1;
		}

		int dupilcate = 0;
		if (!PugiGetSubNodeValue(data_element, "dupilcate", dupilcate) || (dupilcate != 0 && dupilcate != 1))
		{
			return -2;
		}
		cfg.dupilcate = dupilcate > 0 ? true : false;

		if (!PugiGetSubNodeValue(data_element, "refresh_weight", cfg.refresh_weight) || cfg.refresh_weight < 0)
		{
			return -3;
		}

		m_huanling_pool_total_refresh_weight += cfg.refresh_weight;

		if (!PugiGetSubNodeValue(data_element, "baodi_limit_times", cfg.baodi_limit_times) || cfg.baodi_limit_times < 0)
		{
			return -4;
		}

		m_huanling_pool_cfg_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	if (m_huanling_pool_total_refresh_weight <= 0)
	{
		return -5;
	}

	return 0;
}

int ShenshouConfig::InitHuanlingRefreshCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int pool = -1;
		if (!PugiGetSubNodeValue(data_element, "refresh_pool", pool) || pool < 0 || pool >= static_cast<int>(m_huanling_pool_cfg_vec.size()))
		{
			return -1;
		}

		HuanlingPoolCfg &cfg = m_huanling_pool_cfg_vec[pool];

		HuanlingRefreshItemCfg item_cfg;

		if (!PugiGetSubNodeValue(data_element, "refresh_weight", item_cfg.refresh_weight) || item_cfg.refresh_weight < 0)
		{
			return -2;
		}

		cfg.item_total_refresh_weight += item_cfg.refresh_weight;

		if (!PugiGetSubNodeValue(data_element, "draw_item_weight", item_cfg.draw_weight) || item_cfg.draw_weight < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "index", item_cfg.seq) || item_cfg.seq < 0)
		{
			return -4;
		}

		int broadcast = -1;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", broadcast) || (broadcast != 0 && broadcast != 1))
		{
			return -4;
		}

		item_cfg.is_broadcast = broadcast > 0;

		cfg.item_total_draw_weight += item_cfg.draw_weight;

		{
			PugiXmlNode item_data = data_element.child("item");
			if (NULL == item_data)
			{
				return -5;
			}
			if (!item_cfg.item.ReadConfig(item_data))
			{
				return -6;
			}
		}

		if (cfg.item_map.find(item_cfg.seq) == cfg.item_map.end())
		{
			cfg.item_map[item_cfg.seq] = item_cfg;
		}
		else
		{
			return -7;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenshouConfig::InitHuanlingDrawCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int old_draw_times = -1;
	HuanlingDrawCfg draw_cfg;

	while (!data_element.empty())
	{
		int draw_times = -1;
		if (!PugiGetSubNodeValue(data_element, "draw_times", draw_times) || draw_times < 0 || draw_times >= SHENSHOU_MAX_DRAW_TIMES)
		{
			return -1;
		}

		if (old_draw_times != draw_times)
		{
			old_draw_times = draw_times;
			m_huanling_draw_cfg_vec.push_back(draw_cfg);
		}

		if (draw_times != static_cast<int>(m_huanling_draw_cfg_vec.size()) - 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "score", m_huanling_draw_cfg_vec[draw_times].score) || m_huanling_draw_cfg_vec[draw_times].score < 0)
		{
			return -3;
		}

		int draw_weight = -1;
		if (!PugiGetSubNodeValue(data_element, "draw_weight", draw_weight) || draw_weight < 0)
		{
			return -4;
		}

		m_huanling_draw_cfg_vec[draw_times].total_weight += draw_weight;
		m_huanling_draw_cfg_vec[draw_times].draw_pool_weight_vec.push_back(draw_weight);
		
		data_element = data_element.next_sibling();
	}

	if (m_other_cfg.huanling_draw_limit != static_cast<int>(m_huanling_draw_cfg_vec.size()))
	{
		return -5;
	}

	return 0;
}

int ShenshouConfig::HuanlingRefreshPoolHelper(int refresh_times, int &dupilcate_flag)
{
	// 检查保底
	std::vector<int> index_vec;
	int baodi_total_weight = 0;
	for (int index = 0; index < static_cast<int>(m_huanling_pool_cfg_vec.size()); ++index)
	{
		if (m_huanling_pool_cfg_vec[index].baodi_limit_times > 0 && 0 == refresh_times % m_huanling_pool_cfg_vec[index].baodi_limit_times)
		{
			// 检查是否重复
			if (0 == (dupilcate_flag & (1 << index)))
			{
				index_vec.push_back(index);
				baodi_total_weight += m_huanling_pool_cfg_vec[index].refresh_weight;
			}
		}
	}

	if (baodi_total_weight > 0)
	{
		// 在所有需要保底的地方随机
		int rand_weight = RandomNum(baodi_total_weight);
		for (std::vector<int>::iterator iter = index_vec.begin(); iter != index_vec.end(); ++iter)
		{
			if (rand_weight < m_huanling_pool_cfg_vec[*iter].refresh_weight)
			{
				if (!m_huanling_pool_cfg_vec[*iter].dupilcate)
				{
					dupilcate_flag |= 1 << *iter;
				}
				return *iter;
			}
			else
			{
				rand_weight -= m_huanling_pool_cfg_vec[*iter].refresh_weight;
			}
		}
	}
	else if (m_huanling_pool_total_refresh_weight > 0)
	{
		// 没有保底,正常刷新
		for (int loop_times = 0; loop_times < 1000; ++loop_times)
		{
			int rand_weight = RandomNum(m_huanling_pool_total_refresh_weight);
			int index = 0;
			for (std::vector<HuanlingPoolCfg>::iterator iter = m_huanling_pool_cfg_vec.begin(); iter != m_huanling_pool_cfg_vec.end(); ++iter, ++index)
			{
				if (rand_weight < iter->refresh_weight)
				{
					if (!iter->dupilcate)
					{
						if (0 != (dupilcate_flag & (1 << index)))				//如果重复了
						{
							break;
						}
						else
						{
							dupilcate_flag |= 1 << index;
						}
					}

					return index;
				}
				else
				{
					rand_weight -= iter->refresh_weight;
				}
			}
		}
	}

	return -1;
}
