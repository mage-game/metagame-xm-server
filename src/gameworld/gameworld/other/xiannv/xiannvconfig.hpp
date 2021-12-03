#ifndef __XIANNV_CONFIG_HPP__
#define __XIANNV_CONFIG_HPP__

#include "obj/character/attribute.hpp"
#include "servercommon/struct/xiannvparam.hpp"
#include <set>
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct XiannvOtherCfg
{
	XiannvOtherCfg() : main_percent(0), aid_percent(0), idle_percent(0), rename_consume_gold(0), chou_ling_free_times(0), double_ling_gold(0), fetch_ling_time(0)
	{
	}

	int main_percent;
	int aid_percent;
	int idle_percent;
	int rename_consume_gold;
	int chou_ling_free_times;										// ÿ������������
	int double_ling_gold;											// ˫����ȡ��Һ
	int fetch_ling_time;											// ÿ�������ȡ����
};

struct XiannvBaseCfg
{
	struct ConfigItem 
	{
		ConfigItem() : xiannv_id(0), skill_id(0), active_item_id(0), is_send_system_msg(false)
		{
			memset(xiannv_name, 0, sizeof(xiannv_name));
		}

		int xiannv_id;
		GameName xiannv_name;
		int skill_id;
		ItemID active_item_id;
		bool is_send_system_msg;			//�Ƿ��ʹ���
	};

	XiannvBaseCfg() {}

	ConfigItem cfg_list[MAX_XIANNV_COUNT];
};

const static int MAX_XIANNV_LEVEL = 1000;	// �����Ů�ȼ�
struct XiannvLevelCfg
{
	struct ConfigItem
	{
		ConfigItem() : id(0), level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), xiannv_gongji(0)
		{
		}

		int id;
		int level;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		Attribute xiannv_gongji;
	};

	XiannvLevelCfg() {}

	ConfigItem cfg_list[MAX_XIANNV_COUNT][MAX_XIANNV_LEVEL + 1];
};

const static int MAX_XIANNV_ZIZHI = 100;	// �����Ů����
struct XiannvZiZhiCfg
{
	struct ConfigItem
	{
		ConfigItem() : id(0), level(0), uplevel_stuff_id(0), uplevel_stuff_num(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), xiannv_gongji(0)
		{
		}

		int id;
		int level;
		ItemID uplevel_stuff_id;
		int uplevel_stuff_num;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		Attribute xiannv_gongji;
	};

	XiannvZiZhiCfg() {}

	ConfigItem cfg_list[MAX_XIANNV_COUNT][MAX_XIANNV_ZIZHI + 1];
};

struct XiannvHuanHuaCfg
{
	struct ConfigItem
	{
		ConfigItem() : id(0), active_item_id(0), xiannv_gongji(0)
		{
		}

		int id;
		ItemID active_item_id;
		int xiannv_gongji;
	};

	XiannvHuanHuaCfg() : huanhua_count(0) {}

	int huanhua_count;
	ConfigItem cfg_list[MAX_XIANNV_HUANHUA_COUNT];
};

const static int MAX_XIANNV_HUANHUA_LEVEL = 150;	// �����Ů�û��ȼ�
struct XiannvHuanHuaLevelCfg
{
	struct ConfigItem
	{
		ConfigItem() : id(0), level(0), uplevel_stuff_id(0), uplevel_stuff_num(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), xiannv_gongji(0)
		{
		}

		int id;
		int level;
		ItemID uplevel_stuff_id;
		int uplevel_stuff_num;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		Attribute xiannv_gongji;
	};

	XiannvHuanHuaLevelCfg() {}

	ConfigItem cfg_list[MAX_XIANNV_HUANHUA_COUNT][MAX_XIANNV_HUANHUA_LEVEL + 1];
};

struct NvshenShengwuOtherCfg
{
	NvshenShengwuOtherCfg() : one_chou_need_gold(0), ten_chou_need_gold(0) {}

	int one_chou_need_gold;
	int ten_chou_need_gold;
};

struct NvshenShengwuSkillCfg
{
	static const int MAX_SKILL_LEVEL = 15;

	struct SkillItem
	{
		SkillItem() : level(0), trigger_rate(0), cool_down_ms(0), param_1(0), param_2(0), capability(0), capability_per(0) {}

		int level;
		int trigger_rate;							// ���ܴ�����ݱ�
		int cool_down_ms;
		int param_1;
		int param_2;
		int capability;								// ����ս���ӳ�
		int capability_per;							// ����ս���ӳ���ֱ�
	};

	NvshenShengwuSkillCfg() : skill_id(0), max_level(-1) {}

	int skill_id;

	int max_level;
	SkillItem level_cfg_list[MAX_SKILL_LEVEL + 1];
};

struct NvshenShengwuInfoCfg
{
	static const int MAX_LEVEL = 400;

	enum
	{
		BROADCAST_TYPE_NONE = 0,
		BROADCAST_TYPE_1,			// ������������
		BROADCAST_TYPE_2,			// ������������

		BROADCAST_TYPE_COUNT,
	};

	struct ShengwuItem
	{
		ShengwuItem() : level(0), upgrade_need_exp(0), upgrade_need_shengwu_essence(0), upgrade_need_stuff_number(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), fujia(0), mianshang(0), skill_id(0), skill_level(0), broadcast_type(BROADCAST_TYPE_NONE)
		{
		}

		int level;					// �ȼ�	
		int upgrade_need_exp;		// ������Ҫ����
		int upgrade_need_shengwu_essence;	// ������Ҫʥ�ﾫ������
		int upgrade_need_stuff_number;	// ������Ҫ���ĵ���Ʒ����
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		Attribute fujia;
		Attribute mianshang;
		int skill_id;				// ��������
		int skill_level;			// ���ܵȼ�

		int broadcast_type;			// �鵽��������
	};

	NvshenShengwuInfoCfg() : shengwu_id(0), max_level(-1) {}

	int shengwu_id;

	int max_level;
	ShengwuItem level_cfg_list[MAX_LEVEL + 1];
};

struct NvshenShengwuChouConsumeCfg
{
	NvshenShengwuChouConsumeCfg() : min_level(0), max_level(0), chou_exp_stuff(0), chou_exp_gold(0)
	{}

	int min_level;
	int max_level;
	int chou_exp_stuff;
	int chou_exp_gold;
};

struct NvshenShengwuUpGradeCfg
{
	enum CONSUME_TYPE
	{
		CONSUME_TYPE_INVALID = 0,
		CONSUME_TYPE_ESSENCE = 1,				// ����ʥ�ﾫ��
		CONSUME_TYPE_ITEM = 2,					// ������Ʒ
		CONSUME_TYPE_ESSENCE_AND_ITEM = 3,		// ������Ʒ��ʥ�ﾫ��
		CONSUME_TYPE_MAX
	};
	NvshenShengwuUpGradeCfg() : shengwu_id(0), upgrade_stuff_id(0), exp_reward(0), max_upgrade_level(0), active_need_fairy(0), consume_type(0)
	{}

	int shengwu_id;
	ItemID upgrade_stuff_id;
	int exp_reward;
	int max_upgrade_level;
	int active_need_fairy;		// ����ʥ��������Ů����
	int consume_type;			// ��������
};

struct NvshenShengwuExpChouCfg
{
	enum ChouType
	{
		CHOU_TYPE_NORMAL = 0,		// ��ͨ��
		CHOU_TYPE_PERFECT,			// ������

		CHOU_TYPE_COUNT
	};

	NvshenShengwuExpChouCfg() : chou_type(0), rate1_exp(0), rate2_exp(0), rate3_exp(0)
	{
		memset(rate_shengwu_list, 0, sizeof(rate_shengwu_list));
	}

	int chou_type;
	int rate1_exp;					// ����1(20%)���ӵľ���ֵ
	int rate2_exp;					// ����2(20%)���ӵľ���ֵ
	int rate3_exp;					// ����3(60%)���ӵľ���ֵ

	int rate_shengwu_list[XIANNV_SHENGWU_MAX_ID + 1]; // ÿ��ʥ���Ӧ�ܳ鵽�ĸ���
};

struct NvshenShengwuGongmingCfg
{
	static const int MAX_LEVEL = 150;	// �����ӵȼ�

	struct ItemCfg
	{
		ItemCfg() : level(0), upgrade_need_ling(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), fujia(0), mianshang(0), skill_id(0),
		            trigger_rate(0), cool_down_ms(0), param_1(0), param_2(0), capbility(0), attr_percent(0), is_broadcast(false) {}

		int level;
		int upgrade_need_ling;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		Attribute fujia;
		Attribute mianshang;

		int skill_id;			// �����Ӧ���ܵ�����
		int trigger_rate;		// ������ֱ�
		int cool_down_ms;		// ��ȴʱ�䣨���룩
		int param_1;
		int param_2;

		int capbility;			// �������Ӽ�ս����û���Լӳɵ�ʱ��ҲҪ����ս�����ӣ�
		int attr_percent;		// ��������ϵͳ������ݱ�

		bool is_broadcast;
	};

	NvshenShengwuGongmingCfg() : grid_id(0), max_level(-1) {}

	int grid_id;

	int max_level;
	ItemCfg level_cfg_list[MAX_LEVEL + 1];
};

struct NvshenShengwuChouLingCfg
{
	static const int MAX_LING_CFG_COUNT = 7;		// ע�⣬��ָ�������õĸ���������������ġ�����������

	NvshenShengwuChouLingCfg() : ling_count(0), ling_value(0), gold_rate(0), free_rate(0) {}

	int ling_count;									// ��������
	int ling_value;
	int gold_rate;
	int free_rate;
};

struct NvshenShengwuChouLingConsumeCfg
{
	const static int MAX_CFG_COUNT = 10;

	NvshenShengwuChouLingConsumeCfg(): chou_ling_times(0), consume_gold(0) {}

	int chou_ling_times;
	int consume_gold;								// ����Ԫ��
};

struct XiannvHuanHuaSpecialCapAddCfgItem
{
	XiannvHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};

class XiannvConfig : public ConfigBase
{
public:
	XiannvConfig();
	~XiannvConfig();

	bool Init(std::string path, std::string *err);

	const XiannvOtherCfg * GetOtherCfg() const { return &m_other_cfg; }
	const XiannvBaseCfg::ConfigItem * GetBaseCfg(int xiannv_id) const;
	const XiannvLevelCfg::ConfigItem * GetLevelCfg(int xiannv_id, int level) const;
	const XiannvZiZhiCfg::ConfigItem * GetZiZhiCfg(int xiannv_id, int level) const;
	int GetHuanHuaCount() const { return m_huanhua_cfg.huanhua_count; }
	const XiannvHuanHuaCfg::ConfigItem * GetHuanHuaCfg(int huanhua_id) const;
	const XiannvHuanHuaLevelCfg::ConfigItem * GetHuanHuaLevelCfg(int huanhua_id, int level) const;

	const NvshenShengwuSkillCfg::SkillItem * GetShengwuSkillCfg(int skill_id, int skill_level) const;
	const NvshenShengwuExpChouCfg * GetShengwuExpChouCfg(int chou_type) const;
	const NvshenShengwuUpGradeCfg * GetShengwuUpgradeCfg(int shengwu_id) const;
	const NvshenShengwuInfoCfg::ShengwuItem * GetShengwuCfg(int shengwu_id, int shengwu_level) const;
	const NvshenShengwuChouConsumeCfg* GetShengwuChouConsumeCfg(int level) const;
	const NvshenShengwuGongmingCfg::ItemCfg * GetGongmingGridCfg(int grid_id, int level) const;
	const std::set<int> * GetGridDependCfg(int grid) const;
	const NvshenShengwuChouLingCfg * GetChouLingCfg(int cur_ling_count) const;
	const XiannvHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;
	bool IsAddSpecialCap();
	const int GetChouLingConsumeGold(int chou_ling_times) const;

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitBaseCfg(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitZiZhiCfg(PugiXmlNode RootElement);
	int InitHuanHuaCfg(PugiXmlNode RootElement);
	int InitHuanHuaLevelCfg(PugiXmlNode RootElement);

	int InitSkillCfg(PugiXmlNode RootElement);
	int InitUpGradeCfg(PugiXmlNode RootElement);
	int InitShengwuInfoCfg(PugiXmlNode RootElement);
	int InitShengwuChouConsumeCfg(PugiXmlNode RootElement);
	int InitExpCfg(PugiXmlNode RootElement);
	int InitGongmingCfg(PugiXmlNode RootElement);
	int InitGridDependCfg(PugiXmlNode RootElement);
	int InitChouLingCfg(PugiXmlNode RootElement);
	int InitChouLingConsumeCfg(PugiXmlNode RootElement);
	int InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement);

	XiannvOtherCfg m_other_cfg;
	XiannvBaseCfg m_base_cfg;
	XiannvLevelCfg m_level_cfg;
	XiannvZiZhiCfg m_zizhi_cfg;
	XiannvHuanHuaCfg m_huanhua_cfg;
	XiannvHuanHuaLevelCfg m_huanhua_level_cfg;

	NvshenShengwuSkillCfg m_skill_cfg_list[XIANNV_SHENGWU_SKILL_TYPE_COUNT];

	int m_shengwu_max_id;
	NvshenShengwuInfoCfg m_shengwu_info_cfg_list[XIANNV_SHENGWU_MAX_ID + 1];

	int m_shengwu_chou_comsume_count;
	NvshenShengwuChouConsumeCfg m_shengwu_chou_comsuem_list[XIANNV_SHENGWU_CHOU_COMSUEM_MAX_COUNT];

	NvshenShengwuExpChouCfg m_chou_exp_cfg_list[NvshenShengwuExpChouCfg::CHOU_TYPE_COUNT];

	NvshenShengwuGongmingCfg m_grid_cfg_list[XIANNV_SHENGWU_GONGMING_MAX_GRID_ID + 1];

	std::map<int, std::set<int> > m_grid_depend_list_map;		// ����������ϵ����

	NvshenShengwuChouLingCfg m_chou_ling_cfg_list[NvshenShengwuChouLingCfg::MAX_LING_CFG_COUNT];

	int m_chou_ling_consume_cfg_count;
	NvshenShengwuChouLingConsumeCfg m_chou_ling_consume_cfg_list[NvshenShengwuChouLingConsumeCfg::MAX_CFG_COUNT];
	NvshenShengwuUpGradeCfg m_shengwu_upgrade_cfg_list[XIANNV_SHENGWU_MAX_ID + 1];

	bool m_is_open_special_cap_add;
	typedef std::map<int, XiannvHuanHuaSpecialCapAddCfgItem> XiannvHuanHuaSpecialCapAddCfg;
	XiannvHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;
};

#endif // __XIANNV_CONFIG_HPP__
