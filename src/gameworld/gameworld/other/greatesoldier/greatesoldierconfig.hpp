// 名将系统配置（M-名将.xls）
#ifndef __GREATE_SOLDIER_CONFIG_HPP__
#define __GREATE_SOLDIER_CONFIG_HPP__

#include "config/gameconfigcommon/gameconfigcommon.h"
#include "servercommon/greatesoldierdef.hpp"
#include "item/itempool.h"
#include "item/itembase.h"

#define GREATE_SOLDIER_CFG() LOGIC_CONFIG->GetGreateSoldierConfig()
#define GREATE_SOLDIER_OTHER_CFG() LOGIC_CONFIG->GetGreateSoldierConfig().GetOtherCfg()
#define GREATE_SOLDIER_LEVEL_CFG(seq) LOGIC_CONFIG->GetGreateSoldierConfig().GetLevelCfg(seq)
#define GREATE_SOLDIER_POTENTIAL_CFG(seq, level) LOGIC_CONFIG->GetGreateSoldierConfig().GetPotentialCfg(seq, level)
#define GREATE_SOLDIER_PASSIVE_SKILL_CFG(skill_seq) LOGIC_CONFIG->GetGreateSoldierConfig().GetPassiveSkillCfg(skill_seq)
#define GREATE_SOLDIER_SLOT_CFG(slot_seq, slot_level) LOGIC_CONFIG->GetGreateSoldierConfig().GetSlotCfg(slot_seq, slot_level)
#define GREATE_SOLDIER_COMBINE_CFG(combine_seq) LOGIC_CONFIG->GetGreateSoldierConfig().GetCombineCfg(combine_seq)
#define GREATE_SOLDIER_COMBINE_ATTR_CFG(combine_seq, level) LOGIC_CONFIG->GetGreateSoldierConfig().GetCombineAttrCfg(combine_seq, level)

// 其它
struct GreateSoldierOtherConfig
{
	int has_read = 0;

	int max_level = 0;						// 名将最高等级
	int bianshen_duration_s = 0;			// 变身持续时间
	int bianshen_cd_s = 0;					// 变身CD
	ConsumeItemConfig draw_1_consume_item;	// 单抽消耗物品
	ConsumeItemConfig dailyfirst_draw_10_consume_item;	//每天首冲十连抽消耗物品
	ConsumeItemConfig draw_10_consume_item;	// 10抽消耗物品
	ConsumeItemConfig draw_50_consume_item;	// 50抽消耗物品
	int bianyi_need_potential_per = 0;		// 变异所需潜能百分比
	int bianyi_add_passive_skill_per = 0;	// 变异加成被动技能百分比
	int bianshen_trail_s = 0;				// 变身体验时长（秒）
	int bianshen_trail_capability = 0;		// 变身体验战力
};

// 等级
struct GreateSoldierLevelConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1000;
			has_read = 1;

			seq = _seq;
			if (!PugiGetSubNodeValue(data_element, "item_id", item_id) || NULL == ITEMPOOL->GetItem(item_id)) return -1;
			if (!PugiGetSubNodeValue(data_element, "image_id", image_id) || image_id <= 0) return -2;
			if (!PugiGetSubNodeValue(data_element, "color", color)) return -3;
			if (!PugiGetSubNodeValue(data_element, "max_equip_color", max_equip_color)) return -4;
			if (!PugiGetSubNodeValue(data_element, "name", name)) return -5;
			if (!PugiGetSubNodeValue(data_element, "active_passive_skill_seq", active_passive_skill_seq) || active_passive_skill_seq < 0) return -6;
			if (!PugiGetSubNodeValue(data_element, "active_skill_type", active_skill_type) || active_skill_type < 0) return -7;
			if (!PugiGetSubNodeValue(data_element, "active_skill_level", active_skill_level) || active_skill_level <= 0) return -8;
			if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast)) return -9;
			if (!PugiGetSubNodeValue(data_element, "exchange_item_id", exchange_item_id)) return -10;
			if (!PugiGetSubNodeValue(data_element, "exchange_item_num", exchange_item_num)) return -11;
			if (!PugiGetSubNodeValue(data_element, "exchange_counts", exchange_counts)) return -12;

			if (attrs_cfg.ReadConfig(data_element) < 0) return -50;

			return 0;
		}

		int has_read = 0;

		int seq = 0;							// 索引
		std::string name;						// 名将名
		int image_id = 0;						// 形象ID
		int color = 0;							// 品质
		int max_equip_color = 0;				// 可穿戴最高装备品质
		ItemID item_id = 0;						// 升级消耗物品
		int active_passive_skill_seq = 0;		// 激活被动技能索引
		int active_skill_type = 0;				// 激活特殊技能类型
		int active_skill_level = 0;				// 激活的主动技能等级
		AttributesConfig attrs_cfg;				// 基础属性
		int is_broadcast = 0;					// 激活是否广播
		int exchange_item_id = 0;				// 兑换物品id
		int exchange_item_num = 0;				// 兑换物品数量
		int exchange_counts = 0;				// 兑换次数每天
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_MAX_COUNT];
};

// 潜能升级
struct GreateSoldierPotentialConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1000;
			has_read = 1;

			if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0) return -2;
			if (!PugiGetSubNodeValue(data_element, "succ_percent", succ_percent) || succ_percent <= 0) return -3;
			if (!PugiGetSubNodeValue(data_element, "passive_strength", passive_strength) || passive_strength < 0) return -4;
			if (!PugiGetSubNodeValue(data_element, "gongji_strength", gongji_strength) || gongji_strength < 0) return -5;
			if (!PugiGetSubNodeValue(data_element, "special_skill_level", special_skill_level) || special_skill_level < 0) return -6;
			if (attrs_cfg.ReadConfig(data_element) < 0) return -50;
			PugiXmlNode element = data_element.child("upgrade_item");
			if (!upgrade_item.ReadConfig(element)) return -51;

			return 0;
		}

		int has_read = 0;

		int level = 0;						// 等级
		ItemConfigData upgrade_item;		// 升级道具
		int succ_percent = 0;				// 升级成功率
		short passive_strength = 0;			// 变身被动提升 百分比
		Attribute gongji_strength = 0;			// 名将攻击提升
		int special_skill_level = 0;		// 开启特殊技能等级,0表示不激活
		AttributesConfig attrs_cfg;			// 基础属性
	};

	std::vector<CfgItem> cfg_item_vec_list[GREATE_SOLDIER_MAX_COUNT];
};

// 被动技能
struct GreateSoldierPassiveSkillConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1000;
			has_read = 1;

			seq = _seq;
			if (!PugiGetSubNodeValue(data_element, "add_gongji_per", add_gongji_per) || add_gongji_per < 0) return -1;
			if (!PugiGetSubNodeValue(data_element, "add_fangyu_per", add_fangyu_per) || add_fangyu_per < 0) return -2;
			if (!PugiGetSubNodeValue(data_element, "add_maxhp_per", add_maxhp_per) || add_maxhp_per < 0) return -3;
			if (!PugiGetSubNodeValue(data_element, "add_hurt_increase", add_hurt_increase) || add_hurt_increase < 0) return -4;
			if (!PugiGetSubNodeValue(data_element, "add_hurt_reduce", add_hurt_reduce) || add_hurt_reduce < 0) return -5;

			return 0;
		}

		int has_read = 0;

		int seq = 0;					// 技能类型索引
		int add_gongji_per = 0;			// 加攻击百分比
		int add_fangyu_per = 0;			// 加防御百分比
		int add_maxhp_per = 0;			// 加生命上限百分比
		int add_hurt_increase = 0;		// 加伤害增加百分比
		int add_hurt_reduce = 0;		// 加伤害减免百分比
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_PASSIVE_SKILL_MAX_COUNT];
};

// 特殊技能
struct GreateSoldierSpecialSkillConfig
{
	enum SPECIAL_SKILL_MODE
	{
		SPECIAL_SKILL_MODE_SINGLE = 0, // 单体
		SPECIAL_SKILL_MODE_TEAM = 1,   // 全队
		
		SPECIAL_SKILL_MODE_MAX,        // 技能模式上限
	};

	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _active_skill_type, int _skill_level)
		{
			if (has_read) return -1000;
			has_read = 1;

			active_skill_type = _active_skill_type;
			skill_level = _skill_level;
			if (!PugiGetSubNodeValue(data_element, "skill_mode", skill_mode) || skill_mode < SPECIAL_SKILL_MODE_SINGLE || skill_mode > SPECIAL_SKILL_MODE_TEAM) return -1;
			if (!PugiGetSubNodeValue(data_element, "param_a", param_a) || param_a < 0) return -2;
			if (!PugiGetSubNodeValue(data_element, "param_b", param_b) || param_b < 0) return -3;
			if (!PugiGetSubNodeValue(data_element, "param_c", param_c) || param_c < 0) return -4;
			if (!PugiGetSubNodeValue(data_element, "param_d", param_d) || param_d < 0) return -5;

			return 0;
		}

		int has_read = 0;

		int active_skill_type = 0;		// 特殊技能类型
		int skill_level = 0;
		int skill_mode = -1;
		int param_a = 0;
		int param_b = 0;
		int param_c = 0;
		int param_d = 0;
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_SPECIAL_SKILL_TYPE_MAX][GREATE_SOLDIER_SPECIAL_SKILL_MAX_LEVEL];
};

// 将位
struct GreateSoldierSlotConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _level)
		{
			if (has_read) return -1000;
			has_read = 1;

			seq = _seq;
			level = _level;

			if (!PugiGetSubNodeValue(data_element, "need_val", need_val) || need_val <= 0) return -1;
			if (!PugiGetSubNodeValue(data_element, "item_id", item_id) || NULL == ITEMPOOL->GetItem(item_id)) return -2;
			if (!PugiGetSubNodeValue(data_element, "item_num", item_num) || item_num <= 0) return -3;

			if (!item_add_cfg.ReadConfig(data_element, "item_add_val", "item_add_rates")) return -4;

			if (!PugiGetSubNodeValue(data_element, "gongji_conv_rate", gongji_conv_rate) || gongji_conv_rate < 0) return -5;
			if (!PugiGetSubNodeValue(data_element, "fangyu_conv_rate", fangyu_conv_rate) || fangyu_conv_rate < 0) return -6;
			if (!PugiGetSubNodeValue(data_element, "hp_conv_rate", hp_conv_rate) || hp_conv_rate < 0) return -7;

			if (!PugiGetSubNodeValue(data_element, "wash_attr_add_percent", wash_attr_add_percent) || wash_attr_add_percent < 0) return -8;
			if (attr_cfg.ReadConfig(data_element) < 0) return -9;
			return 0;
		}

		int has_read = 0;
		int seq = 0;					// 将位索引
		int level = 0;					// 等级
		int need_val = 0;				// 升级所需祝福值
		ItemID item_id = 0;				// 升级消耗物品
		int item_num = 0;				// 升级消耗物品数量

		LuckyConfig item_add_cfg;		// 进阶暴击

		int gongji_conv_rate = 0;		// 攻击转换率
		int fangyu_conv_rate = 0;		// 防御转换率
		int hp_conv_rate = 0;			// 生命转换率

		int wash_attr_add_percent = 0;	// 洗练属性百分比加成
		AttributesConfig attr_cfg;
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_SLOT_MAX_COUNT * GREATE_SOLDIER_SLOT_LEVEL_MAX_COUNT];
};

// 将位其他
struct GreateSoldierSlotOtherConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			if (!PugiGetSubNodeValue(data_element, "need_level", need_level)) return -2;

			return 0;
		}

		int has_read = 0;
		int seq = 0;                    // 索引
		int need_level = 0;				// 装备所需等级
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_SLOT_MAX_COUNT];
};

// 组合
struct GreateSoldierCombineConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1000;
			has_read = 1;

			seq = _seq;

			if (!greate_soldier_seq_list.ReadConfig(data_element, "greate_soldier_seq_list")) return -1;
			return 0;
		}

		int has_read = 0;
		int seq = 0;								// 组合索引
		ListConfig<int> greate_soldier_seq_list;	// 所需名将索引
		
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_COMBINE_MAX_COUNT];
};

// 组合属性
struct GreateSoldierCombineAttrConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1000;
			has_read = 1;

			seq = _seq;

			if (!PugiGetSubNodeValue(data_element, "need_min_strength_level", need_min_strength_level) || need_min_strength_level <= 0) return -2;
			if (attrs_cfg.ReadConfig(data_element) < 0) return -3;
			if (!PugiGetSubNodeValue(data_element, "all_percent", all_percent) || all_percent < 0) return -4;
			return 0;
		}

		int has_read = 0;
		int seq = 0;								// 组合索引
		int need_min_strength_level = 0;			// 组合最低等级
		int all_percent = 0;                        // 全身攻防血属性加成
		AttributesConfig attrs_cfg;					// 组合属性加成
	};

	std::vector<CfgItem> cfg_item_vec_list[GREATE_SOLDIER_COMBINE_MAX_COUNT];
};

// 抽奖
struct GreateSoldierDrawConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -100;
			has_read = true;

			seq = _seq;
			if (!ReadItemConfig(data_element, "reward_item", reward_item)) return -1;
			if (!PugiGetSubNodeValue(data_element, "is_special_10_reward", is_special_10_reward) || is_special_10_reward < 0) return -2;
			if (!PugiGetSubNodeValue(data_element, "is_special_50_reward", is_special_50_reward) || is_special_50_reward < 0) return -3;
			if (!PugiGetSubNodeValue(data_element, "is_broadcast", broadcast_type) || broadcast_type < 0) return -4;
			return 0;
		}

		int seq = 0;							// 索引
		ItemConfigData reward_item;				// 物品奖励
		int is_special_10_reward = 0;			// 是否10抽特殊物品
		int is_special_50_reward = 0;			// 是否50抽特殊物品
		bool has_read = false;
		int broadcast_type = 0;					// 传闻类型 参考SYS_MSG_TYPE
	};

	CfgItem cfg_item_list[DRAW_ITEM_MAX_COUNT];
	RandItemConfig draw_1_weight;			// 单抽权重
	RandItemConfig draw_10_weight;			// 10抽权重
	RandItemConfig draw_50_weight;			// 50抽权重

	RandItemConfig special_draw_10_weight;		// 特殊10抽权重
	RandItemConfig special_draw_50_weight;		// 特殊50抽权重
};

// 保底领奖
struct GreateSoldierLimitDrawConfig
{
	struct CfgItem
	{
		int draw_num = 0;
		ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];	// 免费抽权重
	};
	std::vector<CfgItem> reward_vec;
};

// 目标
struct GreateSoldierGoalConfig
{
	int Read(PugiXmlNode node)
	{
		if (!PugiGetSubNodeValue(node, "goal_type", goal_type)) return -1;
		if (!PugiGetSubNodeValue(node, "need_days", need_days)) return -2;
		if (!PugiGetSubNodeValue(node, "need_active_seq", need_active_seq)) return -3;
		if (!ReadItemConfig(node, "can_fetch_reward", can_fetch_reward)) return -4;
		if (!PugiGetSubNodeValue(node, "add_base_attr_per", add_base_attr_per)) return -5;
		if (!PugiGetSubNodeValue(node, "cost", cost)) return -6;

		return 0;
	}

	int GetIndex() { return goal_type; }

	int goal_type = 0;					// 目标类型
	int need_days = 0;					// 完成所需时间（天）
	int need_active_seq = 0;			// 完成所需激活名将
	ItemConfigData can_fetch_reward;	// 可领取奖励
	int add_base_attr_per = 0;			// 基础属性加成百分比
	int cost = 0;						// 购买金额
};

// 幻化
struct GreateSoldierHuanhuaConfig
{
	GreateSoldierHuanhuaConfig() : id(0), res_id(0), active_skill_level(0) {}

	int id;							// 幻化id
	int res_id;						// 客户端资源ID
	int active_skill_level;			// 激活主动技能等级
	AttributesConfig attr_cfg;
};

// 名将装备等级
struct GreateSoldierEquipLevelCfg
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _slot_index, int _quality, int _strength_level)
		{
			if (has_read) return -1000;
			has_read = 1;

			slot_index = _slot_index;
			quality = _quality;
			strength_level = _strength_level;

			if (!PugiGetSubNodeValue(data_element, "upgrade_need_shulian", upgrade_need_shulian) || upgrade_need_shulian <= 0) return -2;
			if (!PugiGetSubNodeValue(data_element, "contain_shulian", contain_shulian) || contain_shulian < 0) return -3;

			if (attr_cfg.ReadConfig(data_element) < 0) return -4;
			return 0;
		}

		int has_read = 0;

		int slot_index = 0;				// 装备部位
		int quality = 0;				// 装备品质
		int strength_level = 0;			// 装备强化等级

		int upgrade_need_shulian = 0;	// 提升等级所需熟练度
		int contain_shulian = 0;		// 当强化材料时提升熟练度值

		AttributesConfig attr_cfg;
	};

	int max_strength_level = 0;			// 最大强化等级

	CfgItem item_cfg_list[GREATE_SOLDIER_EQUIPMENT_SLOT_MAX_COUNT][ItemBase::I_COLOR_MAX][GREATE_SOLDIER_MAX_EQUIP_STRENGTH_LEVEL + 1];
};

class GreateSoldierConfig : public ConfigBase
{
public:
	bool Init(const std::string &configname, std::string *err);

	const GreateSoldierOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const GreateSoldierLevelConfig::CfgItem * GetLevelCfg(int seq);
	const int GetGreateSoldierLevelAttrPer(int level);
	const GreateSoldierPotentialConfig::CfgItem * GetPotentialCfg(int seq, int level);
	const GreateSoldierPassiveSkillConfig::CfgItem * GetPassiveSkillCfg(int seq);
	const GreateSoldierSpecialSkillConfig::CfgItem * GetSpecialSkillCfg(int special_skill_type, int skill_level);
	const GreateSoldierSlotConfig::CfgItem * GetSlotCfg(int seq, int level);
	const GreateSoldierSlotOtherConfig::CfgItem * GetSlotOtherCfg(int seq);
	const GreateSoldierCombineConfig::CfgItem * GetCombineCfg(int seq);
	const GreateSoldierCombineAttrConfig::CfgItem * GetCombineAttrCfg(int seq, int level);
	const GreateSoldierDrawConfig::CfgItem * GetRandomDrawReward(int draw_t);	// 随机抽奖
	const GreateSoldierLimitDrawConfig& GetLimitDrawConfig() { return m_limit_draw_reward_cfg; }
	//const GreateSoldierGoalConfig* GetGreateSoldierGoalConfig(int goal_type) { return m_goal_cfg.GetConfigItem(goal_type); }
	const GreateSoldierGoalConfig* GetGreateSoldierGoalConfig(int goal_type) { return NULL; }
	const GreateSoldierHuanhuaConfig* GetGreateSoldierHuanhuaConfig(int id);
	const GreateSoldierEquipLevelCfg::CfgItem* GetGreateSoldierEquipLevelConfig(int slot_index, int quality, int strength_level);
	int GetEquipShulianduMax(int equip_slot_index, int quality);
	int GetGreateSoldierItemMaxLevel() { return m_equip_level_cfg.max_strength_level; };

	bool IsValidSeq(int seq, bool is_init = false);								// 是否是合法的名将索引
	bool IsValidPassiveSkillSeq(int seq, bool is_init = false);					// 是否是合法的被动技能索引
	bool IsValidSlotSeq(int seq);												// 是否是合法的将位seq
	bool IsValidSlotSeqLevel(int seq, int level, bool is_init = false);			// 是否是合法的将位seq和level
	bool IsValidCombineSeq(int seq, bool is_init = false);						// 是否是合法的组合索引
	bool IsValidDrawSeq(int seq, bool is_init = false);							// 是否是合法的抽奖索引

private:
	int InitOtherCfg(PugiXmlNode root_element);
	int InitLevelCfg(PugiXmlNode root_element);
	int InitLevelAttrPerCfg(PugiXmlNode root_element);
	int InitUpgradeCfg(PugiXmlNode root_element);
	int InitPassiveSkillCfg(PugiXmlNode root_element);
	int InitSpecialSkillCfg(PugiXmlNode root_element);
	int InitSlotCfg(PugiXmlNode root_element);
	int InitSlotOtherCfg(PugiXmlNode root_element);
	int InitCombineCfg(PugiXmlNode root_element);
	int InitCombineAttrCfg(PugiXmlNode root_element);
	int InitDrawCfg(PugiXmlNode root_element);
	int InitLimitDrawCfg(PugiXmlNode root_element);
	int InitEquipLevelCfg(PugiXmlNode root_element);
	int InitHuanHuaCfg(PugiXmlNode root_element);

private:
	GreateSoldierOtherConfig m_other_cfg;
	GreateSoldierLevelConfig m_level_cfg;
	std::map<int, int> m_level_attr_per_cfg;   // 等级属性<等级，属性加成万分比>
	GreateSoldierPotentialConfig m_upgrade_cfg;
	GreateSoldierPassiveSkillConfig m_passive_skill_cfg;
	GreateSoldierSpecialSkillConfig m_special_skill_cfg;
	GreateSoldierSlotConfig m_slot_cfg;
	GreateSoldierSlotOtherConfig m_slot_other_cfg;
	GreateSoldierCombineConfig m_combine_cfg;
	GreateSoldierCombineAttrConfig m_combine_attr_cfg;
	GreateSoldierDrawConfig m_draw_cfg;
	GreateSoldierLimitDrawConfig m_limit_draw_reward_cfg;
	GreateSoldierEquipLevelCfg m_equip_level_cfg;
	//ConfigItemList1<GreateSoldierGoalConfig, 2> m_goal_cfg;
	std::map<int, GreateSoldierHuanhuaConfig> m_huanhua_cfg;
};

//-------------------------------------------------------------------------------------------

inline bool GreateSoldierConfig::IsValidSeq(int seq, bool is_init)
{
	if (seq < 0 || seq >= GREATE_SOLDIER_MAX_COUNT) return false;

	if (!is_init && !m_level_cfg.cfg_item_list[seq].has_read) return false;

	return true;
}

inline bool GreateSoldierConfig::IsValidPassiveSkillSeq(int seq, bool is_init)
{
	if (seq < 0 || seq >= GREATE_SOLDIER_PASSIVE_SKILL_MAX_COUNT) return false;

	if (!is_init && !m_passive_skill_cfg.cfg_item_list[seq].has_read) return false;

	return true;
}

inline bool GreateSoldierConfig::IsValidSlotSeq(int seq)
{
	return this->IsValidSlotSeqLevel(seq, 1);
}

inline bool GreateSoldierConfig::IsValidSlotSeqLevel(int seq, int level, bool is_init)
{
	if (seq < 0 || seq >= GREATE_SOLDIER_SLOT_MAX_COUNT ||
		level < 0 || level >= GREATE_SOLDIER_SLOT_LEVEL_MAX_COUNT)
	{
		return false;
	}

	if (!is_init && !m_slot_cfg.cfg_item_list[seq * GREATE_SOLDIER_SLOT_LEVEL_MAX_COUNT + level].has_read)
	{
		return false;
	}

	return true;
}

inline bool GreateSoldierConfig::IsValidCombineSeq(int seq, bool is_init)
{
	if (seq < 0 || seq >= GREATE_SOLDIER_COMBINE_MAX_COUNT) return false;

	if (!is_init && !m_combine_cfg.cfg_item_list[seq].has_read) return false;

	return true;
}

inline bool GreateSoldierConfig::IsValidDrawSeq(int seq, bool is_init)
{
	if (seq < 0 || seq >= DRAW_ITEM_MAX_COUNT) return false;

	if (!is_init && !m_draw_cfg.cfg_item_list[seq].has_read) return false;

	return true;
}

//-------------------------------------------------------------------------------------------

#endif // __GREATE_SOLDIER_CONFIG_HPP__