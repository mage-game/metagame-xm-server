#ifndef __SHENBING_TULU_CONFIG_H__
#define __SHENBING_TULU_CONFIG_H__

#include <string>
#include "servercommon/configbase.h"
#include "servercommon/configcommon.h"
#include "gameworld/config/gameconfigcommon/gameconfigcommon.h"
#include "servercommon/struct/redequipcollectdef.hpp"
#include "servercommon/struct/orangeequipcollectdef.hpp"

struct ShenBingTuLuOtherConfig
{
	ShenBingTuLuOtherConfig() : red_equip_collect_active_puton_count(0), orange_equip_collect_active_puton_count(0) {}

	int red_equip_collect_active_puton_count;
	int orange_equip_collect_active_puton_count;						
};

//-------------------------------------------------------------------------------------------

// 红装收集
struct RedEquipCollectConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _prof)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			prof = _prof;

			if (!equip_items.ReadConfig(data_element, "equip_items")) return -2;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// 套装索引
		int prof = 0;					// 职业
		ListConfig<ItemID> equip_items;	// 可装备物品
	};

	CfgItem cfg_item_list[RED_EQUIP_COLLECT_SEQ_MAX_COUNT * PROF_TYPE_PROF_NO_LIMIT];
};

// 红装收集-属性加成
struct RedEquipCollectAttrConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _collect_count)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			collect_count = _collect_count;

			if (attr_cfg.ReadConfig(data_element) < 0)
			{
				return -2;
			}

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// 套装索引
		int collect_count = 0;			// 收集个数
		AttributesConfig attr_cfg;		// 属性加成

	};

	CfgItem cfg_item_list[RED_EQUIP_COLLECT_SEQ_MAX_COUNT * RED_EQUIP_COLLECT_SLOT_COUNT];
};

// 红装收集-其他
struct RedEquipCollectOtherConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _pro)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			pro = _pro;
			if (!PugiGetSubNodeValue(data_element, "order", order)) return -2;
			if (!PugiGetSubNodeValue(data_element, "color", color)) return -3;
			if (!PugiGetSubNodeValue(data_element, "level", level)) return -4;
			if (!PugiGetSubNodeValue(data_element, "star_add_attr_percent", star_add_attr_percent)) return -5;
			if (!PugiGetSubNodeValue(data_element, "reward_title_id", reward_title_id)) return -6;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// 套装索引
		int pro = 0;					// 职业区分
		int order = 0;					// 对应装备阶
		int color = 0;					// 对应装备颜色
		int level = 0;					// 人物等级
		int star_add_attr_percent = 0;	// 星级加成属性百分比
		int reward_title_id = 0;		// 奖励称号id
	};

	CfgItem cfg_item_list[RED_EQUIP_COLLECT_SEQ_MAX_COUNT][RED_EQUIP_COLLECT_PRO_MAX];
};

// 红装收集-开服活动
struct RedEquipCollectActConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			if (!PugiGetSubNodeValue(data_element, "collect_count", collect_count)) return -2;
			if (!reward_item.ReadConfig(data_element.child("reward_item"))) return -3;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// 序号
		int collect_count = 0;			// 集齐套装数
		ItemConfigData reward_item;		// 奖励物品
	};

	CfgItem cfg_item_list[RED_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT];
};

// ----------------------------------------------------------------------------------------
// 橙装收集
struct OrangeEquipCollectConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _prof)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			prof = _prof;

			if (!equip_items.ReadConfig(data_element, "equip_items")) return -2;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// 套装索引
		int prof = 0;					// 职业
		ListConfig<ItemID> equip_items;	// 可装备物品
	};

	CfgItem cfg_item_list[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT * PROF_TYPE_PROF_NO_LIMIT];
};

// 橙装收集-属性加成
struct OrangeEquipCollectAttrConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _collect_count)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			collect_count = _collect_count;

			if (attr_cfg.ReadConfig(data_element) < 0) return -2;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// 套装索引
		int collect_count = 0;			// 收集个数
		AttributesConfig attr_cfg;		// 属性加成

	};

	CfgItem cfg_item_list[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT * ORANGE_EQUIP_COLLECT_SLOT_COUNT];
};

// 橙装收集-其他
struct OrangeEquipCollectOtherConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			if (!PugiGetSubNodeValue(data_element, "active_role_level", active_level)) return -2;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// 套装索引
		int active_level;				// 激活等级
	};

	CfgItem cfg_item_list[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT];
};

// 橙装收集-开服活动
struct OrangeEquipCollectActConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			if (!PugiGetSubNodeValue(data_element, "collect_count", collect_count)) return -2;
			if (!reward_item.ReadConfig(data_element.child("reward_item"))) return -3;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// 序号
		int collect_count = 0;			// 集齐套装数
		ItemConfigData reward_item;		// 奖励物品
	};

	CfgItem cfg_item_list[ORANGE_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT];
};

class ShenBingTuLuConfig : public ConfigBase
{
public:
	ShenBingTuLuConfig();
	~ShenBingTuLuConfig();

	bool Init(const std::string &path, std::string *err);
	int InitOtherCfg(PugiXmlNode RootElement);

	const ShenBingTuLuOtherConfig & GetOtherCfg() { return m_other_cfg; }

	const RedEquipCollectAttrConfig::CfgItem * GetRedEquipCollectAttrCfg(int seq, int cnt);
	const RedEquipCollectConfig & GetRedEquipCollectConfig() { return m_red_equip_collect_cfg; }
	const RedEquipCollectConfig::CfgItem * GetRedEquipCollectItemCfg(int seq, int prof);
	const RedEquipCollectOtherConfig::CfgItem * GetRedEquipCollectOtherCfg(int seq, int pro);
	const RedEquipCollectActConfig::CfgItem * GetRedEquipCollectActCfg(int seq);

	//----------------------------------------------------------------------------------------
	const OrangeEquipCollectAttrConfig::CfgItem * GetOrangeEquipCollectAttrCfg(int seq, int cnt);
	const OrangeEquipCollectConfig & GetOrangeEquipCollectConfig() { return m_orange_equip_collect_cfg; }
	const OrangeEquipCollectConfig::CfgItem * GetOrangeEquipCollectItemCfg(int seq, int prof);
	const OrangeEquipCollectOtherConfig::CfgItem * GetOrangeEquipCollectOtherCfg(int seq);
	const OrangeEquipCollectActConfig::CfgItem * GetOrangeEquipCollectActCfg(int seq);

public:
	int InitRedEquipCollectCfg(PugiXmlNode root_element);
	int InitRedEquipCollectAttrCfg(PugiXmlNode root_element);
	int InitRedEquipCollectOtherCfg(PugiXmlNode root_element);
	int InitRedEquipCollectActCfg(PugiXmlNode root_element);

	//-------------------------------------------------------------------------------------------
	int InitOrangeEquipCollectCfg(PugiXmlNode root_element);
	int InitOrangeEquipCollectAttrCfg(PugiXmlNode root_element);
	int InitOrangeEquipCollectOtherCfg(PugiXmlNode root_element);
	int InitOrangeEquipCollectActCfg(PugiXmlNode root_element);

	//-------------------------------------------------------------------------------------------

	bool IsValidRedEquipCollectAttrConfigSeqCount(int seq, int cnt, bool is_init = false);	// 是否是合法的红装收集-属性加成索引
	bool IsValidRedEquipCollectConfigSeqProf(int seq, int prof, bool is_init = false);		// 是否是合法的红装收集索引
	//-------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------

	bool IsValidOrangeEquipCollectAttrConfigSeqCount(int seq, int cnt, bool is_init = false);	// 是否是合法的橙装收集-属性加成索引
	bool IsValidOrangeEquipCollectConfigSeqProf(int seq, int prof, bool is_init = false);		// 是否是合法的橙装收集索引
	//-------------------------------------------------------------------------------------------

	ShenBingTuLuOtherConfig m_other_cfg;

	RedEquipCollectConfig m_red_equip_collect_cfg;
	RedEquipCollectAttrConfig m_red_equip_collect_attr_cfg;
	RedEquipCollectOtherConfig m_red_equip_collect_other_cfg;
	RedEquipCollectActConfig m_red_equip_collect_act_cfg;

	OrangeEquipCollectConfig m_orange_equip_collect_cfg;
	OrangeEquipCollectAttrConfig m_orange_equip_collect_attr_cfg;
	OrangeEquipCollectOtherConfig m_orange_equip_collect_other_cfg;
	OrangeEquipCollectActConfig m_orange_equip_collect_act_cfg;
};


#endif // __SHENBING_TULU_CONFIG_H__

