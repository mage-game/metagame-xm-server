// ����ϵͳ���ã�M-����.xls��
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

// ����
struct GreateSoldierOtherConfig
{
	int has_read = 0;

	int max_level = 0;						// ������ߵȼ�
	int bianshen_duration_s = 0;			// �������ʱ��
	int bianshen_cd_s = 0;					// ����CD
	ConsumeItemConfig draw_1_consume_item;	// ����������Ʒ
	ConsumeItemConfig dailyfirst_draw_10_consume_item;	//ÿ���׳�ʮ����������Ʒ
	ConsumeItemConfig draw_10_consume_item;	// 10��������Ʒ
	ConsumeItemConfig draw_50_consume_item;	// 50��������Ʒ
	int bianyi_need_potential_per = 0;		// ��������Ǳ�ܰٷֱ�
	int bianyi_add_passive_skill_per = 0;	// ����ӳɱ������ܰٷֱ�
	int bianshen_trail_s = 0;				// ��������ʱ�����룩
	int bianshen_trail_capability = 0;		// ��������ս��
};

// �ȼ�
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

		int seq = 0;							// ����
		std::string name;						// ������
		int image_id = 0;						// ����ID
		int color = 0;							// Ʒ��
		int max_equip_color = 0;				// �ɴ������װ��Ʒ��
		ItemID item_id = 0;						// ����������Ʒ
		int active_passive_skill_seq = 0;		// �������������
		int active_skill_type = 0;				// �������⼼������
		int active_skill_level = 0;				// ������������ܵȼ�
		AttributesConfig attrs_cfg;				// ��������
		int is_broadcast = 0;					// �����Ƿ�㲥
		int exchange_item_id = 0;				// �һ���Ʒid
		int exchange_item_num = 0;				// �һ���Ʒ����
		int exchange_counts = 0;				// �һ�����ÿ��
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_MAX_COUNT];
};

// Ǳ������
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

		int level = 0;						// �ȼ�
		ItemConfigData upgrade_item;		// ��������
		int succ_percent = 0;				// �����ɹ���
		short passive_strength = 0;			// ���������� �ٷֱ�
		Attribute gongji_strength = 0;			// ������������
		int special_skill_level = 0;		// �������⼼�ܵȼ�,0��ʾ������
		AttributesConfig attrs_cfg;			// ��������
	};

	std::vector<CfgItem> cfg_item_vec_list[GREATE_SOLDIER_MAX_COUNT];
};

// ��������
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

		int seq = 0;					// ������������
		int add_gongji_per = 0;			// �ӹ����ٷֱ�
		int add_fangyu_per = 0;			// �ӷ����ٷֱ�
		int add_maxhp_per = 0;			// ���������ްٷֱ�
		int add_hurt_increase = 0;		// ���˺����Ӱٷֱ�
		int add_hurt_reduce = 0;		// ���˺�����ٷֱ�
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_PASSIVE_SKILL_MAX_COUNT];
};

// ���⼼��
struct GreateSoldierSpecialSkillConfig
{
	enum SPECIAL_SKILL_MODE
	{
		SPECIAL_SKILL_MODE_SINGLE = 0, // ����
		SPECIAL_SKILL_MODE_TEAM = 1,   // ȫ��
		
		SPECIAL_SKILL_MODE_MAX,        // ����ģʽ����
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

		int active_skill_type = 0;		// ���⼼������
		int skill_level = 0;
		int skill_mode = -1;
		int param_a = 0;
		int param_b = 0;
		int param_c = 0;
		int param_d = 0;
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_SPECIAL_SKILL_TYPE_MAX][GREATE_SOLDIER_SPECIAL_SKILL_MAX_LEVEL];
};

// ��λ
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
		int seq = 0;					// ��λ����
		int level = 0;					// �ȼ�
		int need_val = 0;				// ��������ף��ֵ
		ItemID item_id = 0;				// ����������Ʒ
		int item_num = 0;				// ����������Ʒ����

		LuckyConfig item_add_cfg;		// ���ױ���

		int gongji_conv_rate = 0;		// ����ת����
		int fangyu_conv_rate = 0;		// ����ת����
		int hp_conv_rate = 0;			// ����ת����

		int wash_attr_add_percent = 0;	// ϴ�����԰ٷֱȼӳ�
		AttributesConfig attr_cfg;
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_SLOT_MAX_COUNT * GREATE_SOLDIER_SLOT_LEVEL_MAX_COUNT];
};

// ��λ����
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
		int seq = 0;                    // ����
		int need_level = 0;				// װ������ȼ�
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_SLOT_MAX_COUNT];
};

// ���
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
		int seq = 0;								// �������
		ListConfig<int> greate_soldier_seq_list;	// ������������
		
	};

	CfgItem cfg_item_list[GREATE_SOLDIER_COMBINE_MAX_COUNT];
};

// �������
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
		int seq = 0;								// �������
		int need_min_strength_level = 0;			// �����͵ȼ�
		int all_percent = 0;                        // ȫ����Ѫ���Լӳ�
		AttributesConfig attrs_cfg;					// ������Լӳ�
	};

	std::vector<CfgItem> cfg_item_vec_list[GREATE_SOLDIER_COMBINE_MAX_COUNT];
};

// �齱
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

		int seq = 0;							// ����
		ItemConfigData reward_item;				// ��Ʒ����
		int is_special_10_reward = 0;			// �Ƿ�10��������Ʒ
		int is_special_50_reward = 0;			// �Ƿ�50��������Ʒ
		bool has_read = false;
		int broadcast_type = 0;					// �������� �ο�SYS_MSG_TYPE
	};

	CfgItem cfg_item_list[DRAW_ITEM_MAX_COUNT];
	RandItemConfig draw_1_weight;			// ����Ȩ��
	RandItemConfig draw_10_weight;			// 10��Ȩ��
	RandItemConfig draw_50_weight;			// 50��Ȩ��

	RandItemConfig special_draw_10_weight;		// ����10��Ȩ��
	RandItemConfig special_draw_50_weight;		// ����50��Ȩ��
};

// �����콱
struct GreateSoldierLimitDrawConfig
{
	struct CfgItem
	{
		int draw_num = 0;
		ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];	// ��ѳ�Ȩ��
	};
	std::vector<CfgItem> reward_vec;
};

// Ŀ��
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

	int goal_type = 0;					// Ŀ������
	int need_days = 0;					// �������ʱ�䣨�죩
	int need_active_seq = 0;			// ������輤������
	ItemConfigData can_fetch_reward;	// ����ȡ����
	int add_base_attr_per = 0;			// �������Լӳɰٷֱ�
	int cost = 0;						// ������
};

// �û�
struct GreateSoldierHuanhuaConfig
{
	GreateSoldierHuanhuaConfig() : id(0), res_id(0), active_skill_level(0) {}

	int id;							// �û�id
	int res_id;						// �ͻ�����ԴID
	int active_skill_level;			// �����������ܵȼ�
	AttributesConfig attr_cfg;
};

// ����װ���ȼ�
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

		int slot_index = 0;				// װ����λ
		int quality = 0;				// װ��Ʒ��
		int strength_level = 0;			// װ��ǿ���ȼ�

		int upgrade_need_shulian = 0;	// �����ȼ�����������
		int contain_shulian = 0;		// ��ǿ������ʱ����������ֵ

		AttributesConfig attr_cfg;
	};

	int max_strength_level = 0;			// ���ǿ���ȼ�

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
	const GreateSoldierDrawConfig::CfgItem * GetRandomDrawReward(int draw_t);	// ����齱
	const GreateSoldierLimitDrawConfig& GetLimitDrawConfig() { return m_limit_draw_reward_cfg; }
	//const GreateSoldierGoalConfig* GetGreateSoldierGoalConfig(int goal_type) { return m_goal_cfg.GetConfigItem(goal_type); }
	const GreateSoldierGoalConfig* GetGreateSoldierGoalConfig(int goal_type) { return NULL; }
	const GreateSoldierHuanhuaConfig* GetGreateSoldierHuanhuaConfig(int id);
	const GreateSoldierEquipLevelCfg::CfgItem* GetGreateSoldierEquipLevelConfig(int slot_index, int quality, int strength_level);
	int GetEquipShulianduMax(int equip_slot_index, int quality);
	int GetGreateSoldierItemMaxLevel() { return m_equip_level_cfg.max_strength_level; };

	bool IsValidSeq(int seq, bool is_init = false);								// �Ƿ��ǺϷ�����������
	bool IsValidPassiveSkillSeq(int seq, bool is_init = false);					// �Ƿ��ǺϷ��ı�����������
	bool IsValidSlotSeq(int seq);												// �Ƿ��ǺϷ��Ľ�λseq
	bool IsValidSlotSeqLevel(int seq, int level, bool is_init = false);			// �Ƿ��ǺϷ��Ľ�λseq��level
	bool IsValidCombineSeq(int seq, bool is_init = false);						// �Ƿ��ǺϷ����������
	bool IsValidDrawSeq(int seq, bool is_init = false);							// �Ƿ��ǺϷ��ĳ齱����

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
	std::map<int, int> m_level_attr_per_cfg;   // �ȼ�����<�ȼ������Լӳ���ֱ�>
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