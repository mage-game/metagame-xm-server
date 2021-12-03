#ifndef _TIANSHENHUTI_CONFIG_HPP_
#define _TIANSHENHUTI_CONFIG_HPP_

#include <map>
#include <set>
#include <vector>

#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/tianshenhutidef.hpp"

// Z-周末装备.xls

const static int TIANSHENHUTI_ROLL_REWARD_EQUIP_MIN_LEVEL = 2;				// 天神护体通过抽奖能获得的最低等级的装备
const static int TIANSHENHUTI_ROLL_REWARD_EQUIP_MAX_LEVEL = 6;				// 天神护体通过抽奖能获得的最高等级的装备
const static int TIANSHENHUTI_ROLL_BATCH_FIX_EQUIP_LEVEL = 3;				// 天神护体抽奖-连抽必出等级

UNSTD_STATIC_CHECK(TIANSHENHUTI_ROLL_REWARD_EQUIP_MIN_LEVEL >= 1);
UNSTD_STATIC_CHECK(TIANSHENHUTI_ROLL_REWARD_EQUIP_MIN_LEVEL <= TIANSHENHUTI_ROLL_REWARD_EQUIP_MAX_LEVEL);
UNSTD_STATIC_CHECK(TIANSHENHUTI_ROLL_REWARD_EQUIP_MAX_LEVEL <= TIANSHENHUTI_MAX_EQUIP_LEVEL);

enum TIANSHENHUTI_ROLL_TYPE
{
	TIANSHENHUTI_ROLL_TYPE_INVALID,
	TIANSHENHUTI_ROLL_TYPE_COMMON,	// 免费
	TIANSHENHUTI_ROLL_TYPE_SUPER,	// 花钱
	TIANSHENHUTI_ROLL_TYPE_BATCH,	// 花钱多抽

	TIANSHENHUTI_ROLL_TYPE_MAX
};

struct TianshenhutiOtherConfig
{
	TianshenhutiOtherConfig() : common_roll_cost(0), super_roll_cost(0), batch_roll_cost(0), 
		common_roll_item_id(0), super_roll_item_id(0), batch_roll_item_id(0), common_roll_item_num(0), super_roll_item_num(0), batch_roll_item_num(0),
		free_times(0), free_cd(0), taozhuang_skill_need_level(0) {}

	int common_roll_cost;													// 普通抽花费
	int super_roll_cost;													// 高级抽花费
	int batch_roll_cost;													// 批量抽花费

	ItemID common_roll_item_id;												// 普通抽消耗物品ID
	ItemID super_roll_item_id;												// 高级抽消耗物品ID
	ItemID batch_roll_item_id;												// 批量抽消耗物品ID
	int common_roll_item_num;												// 普通抽消耗物品数量
	int super_roll_item_num;												// 高级抽消耗物品数量
	int batch_roll_item_num;												// 批量抽消耗物品数量

	short free_times;
	short free_cd;

	int taozhuang_skill_need_level;											// 激活套装技能所需要阶数
};

struct TianshenHutiEquipConfig
{
	TianshenHutiEquipConfig() : equip_id(0), taozhuang_type(0), level_taozhuang_type(0), slot_index(0), level(0),
	maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) 
	{
		memset(roll_weight_list, 0, sizeof(roll_weight_list));
	}

	bool IsValid() const { return 0 != equip_id; } 

	TianshenEquipID equip_id;

	int taozhuang_type;														// 套装类型
	int level_taozhuang_type;												// 阶数套装类型
	int slot_index;															// 部位下标
	int level;																// 阶数

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;

	int roll_weight_list[TIANSHENHUTI_ROLL_TYPE_MAX];
};

struct TianshenHutiTaozhuangConfig
{
	TianshenHutiTaozhuangConfig() : taozhuang_type(0), need_taozhuang_num(0), skill_num(0), static_injure(0), rate_injure(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), per_gongji(0), per_fangyu(0), per_maxhp(0) {}

	int taozhuang_type;														// 阶数套装类型
	int need_taozhuang_num;													// 套装数量

	int skill_num;															// 第几个技能
	int static_injure;														// 固定伤害
	int rate_injure;														// 百分比伤害

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute per_gongji;
	Attribute per_fangyu;
	Attribute per_maxhp;
};

const static int TIANSHENHUTI_MAX_DISCOUNT_CFG_COUNT = 16;

struct TianshenHutiDiscountConfig
{
	TianshenHutiDiscountConfig() : seq(0), begin_week_day(0), begin_hour(0), begin_minute(0), end_week_day(0), end_hour(0), end_minute(0), discount_percent(0) {}

	int seq;
	int begin_week_day;
	int begin_hour;
	int begin_minute;

	int end_week_day;
	int end_hour;
	int end_minute;

	int discount_percent;
};

struct TianshenHutiAccumulateRewardConfig
{
	static const int ACCMULATE_REWARD_INDEX_MAX = 64;

	struct TianshenHutiAccumulateRewardItem
	{
		int index = 0;
		int accumulate_times = 0;
		int reward_type = 0;
		ItemConfigData item_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	int opengame_day = 0;
	std::vector<TianshenHutiAccumulateRewardItem> reward_vec;
};

struct TianshenHutiTaoZhuangSkillCfg
{
	int taozhuang_type = 0;
	int level = 0;
	int active_skill_id = 0;
};

struct TianshenHutiAccumulateRewardWeightCfg
{
	TianshenHutiAccumulateRewardWeightCfg(ItemID __item_id, short __weight) : item_id(__item_id), weight(__weight) {};

	ItemID item_id;
	short weight;
};

class TianshenHutiConfig : public ConfigBase
{
public:
	TianshenHutiConfig();
	~TianshenHutiConfig();

	bool Init(const std::string &configname, std::string *err);

	const TianshenhutiOtherConfig & GetOtherConfig() { return m_other_cfg; }
	const TianshenHutiEquipConfig * GetEquipConfigWithID(TianshenEquipID equip_id);
	const TianshenHutiEquipConfig * GetRandomEquipConfigWithLevel(int level, int new_equip_slot = -1);
	const TianshenHutiTaozhuangConfig * GetTaozhuangConfig(int taozhuang_type, int need_taozhuang_num);

	TianshenEquipID RollEquip(int roll_type, const std::set<int> &allow_level_set);
	TianshenHutiDiscountConfig * GetDiscountCfg();
	TianshenHutiAccumulateRewardConfig * GetAccumulateRewardCfg();

	int GetFreeTimes() { return m_other_cfg.free_times; };
	int GetFreeCD() { return m_other_cfg.free_cd; };
	int GetTaoZhuangSkillNeedLevel() { return m_other_cfg.taozhuang_skill_need_level; }

	std::vector<TianshenHutiTaoZhuangSkillCfg> & GetTaoZhuangSkillCfg();

	int GetAccumulateRandomRewardItem(int reward_type);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitTaozhuangTypeCfg(PugiXmlNode RootElement);
	int InitEquipInfoCfg(PugiXmlNode RootElement);
	int InitTaozhuangInfoCfg(PugiXmlNode RootElement);
	int InitDiscountCfg(PugiXmlNode RootElement);
	int InitAccumulateRewardItemPoolCfg(PugiXmlNode RootElement);
	int InitAccumulateRewardCfg(PugiXmlNode RootElement);
	int InitTaoZhuangSkillCfg(PugiXmlNode RootElement);

	TianshenhutiOtherConfig m_other_cfg;
	TianshenHutiEquipConfig m_equip_list[TIANSHENHUTI_MAX_EQUIP_ID + 1];
	std::map<long long, TianshenHutiTaozhuangConfig> m_taozhuang_map; 
	std::vector<TianshenHutiEquipConfig*> m_level_equip_snapshot[TIANSHENHUTI_MAX_EQUIP_LEVEL + 1];

	int m_max_taozhuang_type;
	int m_max_equip_id;

	int m_discount_cfgcount;
	TianshenHutiDiscountConfig m_discount_cfglist[TIANSHENHUTI_MAX_DISCOUNT_CFG_COUNT];

	std::map<int, TianshenHutiAccumulateRewardConfig> m_accumulate_reward_cfg_map;

	std::vector<TianshenHutiTaoZhuangSkillCfg> m_taozhuang_skill_cfg_vec;

	std::map<int, std::vector<TianshenHutiAccumulateRewardWeightCfg>> m_accumulate_reward_weight_map_cfg;
};

#endif
