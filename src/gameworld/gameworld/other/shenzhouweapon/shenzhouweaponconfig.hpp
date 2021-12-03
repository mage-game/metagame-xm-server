#ifndef __SHENZHOU_WEAPON_CONFIG_H__
#define __SHENZHOU_WEAPON_CONFIG_H__

#include "obj/character/attribute.hpp"
#include "servercommon/shenzhouweapondef.hpp"
#include <vector>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"
#include <string>
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

// Y - 异火.xls

const static int SHENZHOU_WEAPON_MAX_MELT_GATHER_NUM = 100;					// 最大采集物数量
const static int SHENZHOU_MAX_GATHER_REWARD_COUNT = 20;						// 最大采集奖励数量
const static int SHENZHOU_EXCHANGE_IDENTIFY_EXP_CFG_COUNT = 20;				// 鉴定经验配置数量最大值
const static int SHENZHOU_IDENTIFY_LEVEL_CFG_COUNT = 12;					// 鉴定等级配置数量最大值
const static int SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT = 10;					// 鉴定星级配置数量最大值
const static int SHENZHOU_IDENTIFY_CONSUME_ITEM_MAX_COUNT = 5;				// 鉴定消耗物品配置数量最大值
const static int SHENZHOU_ELEMET_MAX_LEVEL = 100;							// 元素最大等级
const static int SHENZHOU_HUNYIN_LINGSHU_MAX_LEVEL = 100;					// 魂印灵枢最大等级
const static int SHENZHOU_XILIAN_SUIT_MAX_COUNT = 20;						// 洗练套装最大数量
const static int SHENZHOU_NEW_SUIT_MAX_COUNT = 6;							// 每个品质套装最大配置数量

struct ShenzhouOtherCfg
{
	ShenzhouOtherCfg() : refresh_gather_begin_time(0), refresh_gather_end_time(0),
		gather_scene_id(0), gather_id(0), gather_time_s(0), gather_live_time_s(0), gather_num(0), role_day_gather_num(0),
		buy_day_gather_num_cost(0), buy_day_gather_num_limit(0), box_help_num_limit(0), box_free_times(0), box_free_times_cd(0),
		skip_gather_consume(0), skip_gather_level_limit(0), free_xilian_times(0), replacement_id(0), consume_replacement(0), 
		open_box_10_use_itemid(0), open_box_10_use_item_num(0), can_use_item(true)
	{}

	int refresh_gather_begin_time;											// 采集物刷新开始时间
	int refresh_gather_end_time;											// 采集物刷新结束时间
	int gather_scene_id;													// 采集物出现的场景ID
	int gather_id;															// 采集物ID
	int gather_time_s;														// 采集时间
	int gather_live_time_s;													// 采集物存在时间
	int gather_num;															// 采集物数量
	int role_day_gather_num;												// 采集-每日采集总数限制
	int buy_day_gather_num_cost;											// 购买采集次数花费
	int buy_day_gather_num_limit;											// 采集次数购买上限
	int box_help_num_limit;													// 宝箱协助上限
	int box_free_times;
	int box_free_times_cd;		
	int skip_gather_consume;												// 一键采集消耗
	int skip_gather_level_limit;											// 一键采集限制等级

	int free_xilian_times;													// 每日免费洗炼次数
	int min_shenzhouweapon_score;											// 抽奖一次获得最小积分
	int max_shenzhouweapon_score;											// 抽奖一次获得最大积分
	int replacement_id;
	int consume_replacement;
	int open_box_10_use_itemid;
	int open_box_10_use_item_num;
	bool can_use_item;
};

enum GATHER_REWARD_TYPE
{
	NORMAL_GAHTER_REWARD = 0,
	RARE_GATHER_REWARD,
	UNIQUE_GATHER_REWARD,
};

struct ShenzhouGatherRewardCfg
{
	ShenzhouGatherRewardCfg(): gain_pro(0), reward_type(0){}

	ItemConfigData gather_reward[MAX_ATTACHMENT_ITEM_NUM];
	int gain_pro;
	int reward_type;
};

struct ShenzhouExchangeIdentifyExpCfg
{
	ShenzhouExchangeIdentifyExpCfg() : seq(0), consume_gold(0), reward_exp(0) {}

	int seq;																// 索引
	int consume_gold;														// 消耗钻石
	int reward_exp;															// 得到经验
};

struct ShenzhouIdentifyLevelCfg
{
	ShenzhouIdentifyLevelCfg() : level(0), star_max_level(0) {}

	struct AttributeConfig
	{
		AttributeConfig() : star_level(0), upgrade_need_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), 
			shanbi(0), baoji(0), jianren(0), is_notice(0) {}

		int star_level;
		int upgrade_need_exp;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		int is_notice;
	};

	int level;
	int star_max_level;
	AttributeConfig attribute_cfglist[SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT + 1]; // 星级属性（不同等级）
};

struct ShenzhouIdentifyRewardCfg
{
	ShenzhouIdentifyRewardCfg():consume_item_id(0), index(0), weight(0), identify_level(0){}

	int consume_item_id;
	int index;
	int weight;
	int identify_level;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct ShenzhouIdentifyItemCfg
{
	ShenzhouIdentifyItemCfg():consume_item_id(0), identify_suc_pro(0), fail_jinghua(0), identify_exp(0){}

	int consume_item_id;
	int identify_suc_pro;
	int fail_jinghua;
	int identify_exp;

	std::map<int, ShenzhouIdentifyRewardCfg> reward_pool_map;
};

struct ShenzhouSlotAttrCfg
{
	const static int CFG_MAX_COUNT = 120;

	struct LevelItemConfig
	{
		LevelItemConfig() :hunqi_id(0), slot(0), level(-1), up_level_jinghua(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0){}

		int hunqi_id;
		int slot;
		int level;
		int up_level_jinghua;

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		ItemConfigData up_level_item;
	};

	ShenzhouSlotAttrCfg()
	{
		memset(cfg_list, 0, sizeof(cfg_list));
	}

	LevelItemConfig cfg_list[CFG_MAX_COUNT + 1];
};

// 异火技能类型
enum SHENZHOU_WEAPON_SKILL_TYPE
{
	WEAPON_SKILL_INVALID_TYPE = 0,

	WEAPON_SKILL_ABSORB_HARM_01 = 1,					// 受到攻击有概率抵挡X点伤害
	WEAPON_SKILL_ADD_HARM_ON_MONSTER_02 = 2,			// 攻击时有概率造成X点额外伤害
	WEAPON_SKILL_ADD_HARM_ON_ROLE_03 = 3,				// 受到攻击有概率恢复X点生命值
	WEAPON_SKILL_CAUSE_MORE_HARM_04 = 4,				// 格挡攻击时，受到的伤害额外降低X%（格挡减伤+X%）
	WEAPON_SKILL_ADD_HARM_ON_DEBUFF_ROLE_05 = 5,		// 攻击会心时，造成X%额外伤害（会心伤害+X%）
	WEAPON_SKILL_ZHUOSHAO_06 = 6,						// 攻击时有几率使目标灼烧，每秒造成x%攻击的伤害，持续x秒
	WEAPON_SKILL_ADD_HARM_ON_KILL_ROLE_07 = 7,			// 生命值低于A%时，恢复自身X%最大生命值
	WEAPON_SKILL_DEC_HARM_LESS_THAN_HP_08 = 8,			// 受到伤害有几率获得无敌效果，持续1秒

	WEAPON_SKILL_ADD_HARM_MORE_THAN_HP_09 = 9,			// 自身血量高于x%是，攻击时增伤提升x%（PVP有效）
	WEAPON_SKILL_REBOUND_HARM_10 = 10,					// 被攻击时x%的概率触发，将承受的伤害的x%反弹给攻击的玩家，冷却x秒（PVP有效）
	WEAPON_SKILL_XUANYUN_ON_RANGE_TARGET_11 = 11,		// 攻击时x%的概率触发，对自身范围里主目标及附近x名玩家造成x%的伤害并眩晕，持续x秒，冷却x秒（PVP有效）

	WEAPON_SKILL_MAX_TYPE,
};

struct ShenzhouSkillCfg
{
	ShenzhouSkillCfg():hunqi(0), level(0), skill_type(0), param_1(0), param_2(0), param_3(0), param_4(0), param_5(0), is_notice(0), skill_cap_per(0), capbility(0){}

	int hunqi;
	int level;
	int skill_type;
	int param_1;		// cooldown 冷却时间(s)
	int param_2;		// 参数(如果是比率统一用万分比)
	int param_3;
	int param_4;
	int param_5;
	short is_notice;
	int skill_cap_per;
	int capbility;
};

struct ShenzhouBoxCfg
{
	ShenzhouBoxCfg():box_id(0), cousume_diamond(0), cousume_diamond10(0){}

	int box_id;
	int cousume_diamond;
	int cousume_diamond10;
	ItemConfigData help_reward_item;
	
	std::vector<int> open_reward_num_vec;
};

struct ShenzhouBoxRewardCfg
{
	ShenzhouBoxRewardCfg():level(0), weight(0), is_broadcast(false){}

	int level;
	ItemConfigData reward_item;
	int weight;
	bool is_broadcast;
};

struct ShenzhouElementCfg
{
	ShenzhouElementCfg(){}

	struct ShenzhouElementLevelCfg
	{
		ShenzhouElementLevelCfg(): hunqi(0), element_type(0), element_level(0), attr_add_per(0), maxhp(0), gongji(0), fangyu(0),
			mingzhong(0), shanbi(0), baoji(0), jianren(0), huqi_level_limit(0), is_notice(0)
		{}

		int hunqi;
		int element_type;
		int element_level;
		int attr_add_per;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		int huqi_level_limit;
		int is_notice;
		ItemConfigData up_level_item;
	};

	ShenzhouElementLevelCfg element_level_cfg[SHENZHOU_ELEMET_MAX_TYPE][SHENZHOU_ELEMET_MAX_LEVEL + 1];
};

enum SHENZHOU_HUNYIN_COLOR
{
	HUNYIN_COLOR_COLOR_INVALID = 0,

	HUNYIN_COLOR_GREEN,
	HUNYIN_COLOR_BLUE,
	HUNYIN_COLOR_PURPLE,
	HUNYIN_COLOR_ORANGE,
	HUNYIN_COLOR_RED,
	HUNYIN_COLOR_PINK,
	HUNYIN_COLOR_MAX,
};

struct ShenzhouHunYinCfg
{
	ShenzhouHunYinCfg() : hunyin_id(0), hunyin_color(0), inlay_slot(0), discard_exp(0), blue_lingzhi(0),
		purple_lingzhi(0), orange_lingzhi(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0),
		baoji(0), jianren(0)
	{}

	int hunyin_id;
	int hunyin_color;
	int inlay_slot;
	int discard_exp;
	int blue_lingzhi;
	int purple_lingzhi;
	int orange_lingzhi;
	
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

struct ShenzhouLingshuLevelCfg
{
	ShenzhouLingshuLevelCfg(){}
		
	struct LingshuLevelCfg
	{
		LingshuLevelCfg(): hunqi_id(0), hunyin_slot(0), slot_level(0), lingshu_color(0), up_level_exp(0),
			maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_per(0)
		{}

		int hunqi_id;
		int hunyin_slot;
		int slot_level;
		int lingshu_color;
		int up_level_exp;

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		int add_per;
	};

	LingshuLevelCfg linshu_level_cfg[SHENZHOU_HUNYIN_MAX_SLOT][SHENZHOU_HUNYIN_LINGSHU_MAX_LEVEL + 1];
};

struct ShenzhouHunyinSuitCfg
{
	ShenzhouHunyinSuitCfg(): hunqi_id(0), suit_color(0), maxhp(0), fangyu(0), mingzhong(0), shanbi(0),
		baoji(0), jianren(0), per_hunshi(0), per_gongji(0), per_maxhp(0), shanbi_per(0),
		per_mianshang(0), skill_jianshang_per(0), per_pofang(0), same_qulitily_count(0)
	{}

	int hunqi_id;
	int suit_color;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int per_hunshi;
	int per_gongji;
	int per_maxhp;
	Attribute shanbi_per;
	Attribute per_mianshang;
	Attribute skill_jianshang_per;
	Attribute per_pofang;

	int same_qulitily_count;
};

struct ShenzhouHunyinSoltOpenCfg
{
	ShenzhouHunyinSoltOpenCfg(): hunqi(0), hunyin_slot(0), open_hunqi_level(0), gold(0), limit_1(0), limit_2(0)
	{}

	int hunqi;
	int hunyin_slot;
	int open_hunqi_level;
	int gold;
	int limit_1;	//条件1
	int limit_2;	//条件2
};

struct BoxRewardCountCfg
{
	BoxRewardCountCfg() : level(0), open_reward1(0), open_reward2(0), open_reward3(0), open_reward4(0), open_reward5(0)
	{}

	int level;
	int open_reward1;
	int open_reward2;
	int open_reward3;
	int open_reward4;
	int open_reward5;
};

struct SZXilianOpenCfg
{
	SZXilianOpenCfg():hunqi_id(0), slot_id(0), lingshu_level_limit(0), gold_cost(0)
	{}

	int hunqi_id;
	int slot_id;
	int lingshu_level_limit;
	int gold_cost;
};

struct SZXilianLockCfg
{
	SZXilianLockCfg():lock_num(0)
	{}
	int lock_num;
	ItemConfigData lock_comsume_item;
};

enum SZ_XL_COMSUME_COLOR
{
	XL_COUMME_COLOR_INVALID = -1,

	XL_COUMME_COLOR_FREE,
	XL_COUMME_COLOR_BLUE,
	XL_COUMME_COLOR_PURPLE,
	XL_COUMME_COLOR_ORANGE,
	XL_COUMME_COLOR_RED,

	XL_COUMME_COLOR_MAX,
};

struct SZXilianComsumeCfg
{
	SZXilianComsumeCfg():comsume_color(XL_COUMME_COLOR_INVALID)
	{
		memset(star_weight, 0, sizeof(star_weight));
	}

	int comsume_color;
	ItemConfigData consume_item;
	int star_weight[SHENZHOU_XILIAN_MAX_STAR_COUNT];
};

enum SZ_XL_SHUXING_TYPE
{
	XL_SHUXING_TYPE_INVALID = 0,

	XL_SHUXING_TYPE_MAXHP,
	XL_SHUXING_TYPE_GONGJI,
	XL_SHUXING_TYPE_FANGYU,
	XL_SHUXING_TYPE_MINGZHONG,
	XL_SHUXING_TYPE_SHANBI,
	XL_SHUXING_TYPE_BAOJI,
	XL_SHUXING_TYPE_JIANREN,

	XL_SHUXING_TYPE_HUNQI_LEVEL_MAXHP_PER,				// 魂器等级万分比属性
	XL_SHUXING_TYPE_HUNQI_LEVEL_GONGJI_PER,
	XL_SHUXING_TYPE_HUNQI_LEVEL_FANGYU_PER,
	XL_SHUXING_TYPE_HUNQI_LEVEL_MINGZHONG_PER,
	XL_SHUXING_TYPE_HUNQI_LEVEL_SHANBI_PER,
	XL_SHUXING_TYPE_HUNQI_LEVEL_BAOJI_PER,
	XL_SHUXING_TYPE_HUNQI_LEVEL_JIANREN_PER,

	XL_SHUXING_TYPE_HUNQI_IDENTIFY_MAXHP_PER,			// 魂器鉴定万分比属性
	XL_SHUXING_TYPE_HUNQI_IDENTIFY_GONGJI_PER,
	XL_SHUXING_TYPE_HUNQI_IDENTIFY_FANGYU_PER,
	XL_SHUXING_TYPE_HUNQI_IDENTIFY_MINGZHONG_PER,
	XL_SHUXING_TYPE_HUNQI_IDENTIFY_SHANBI_PER,
	XL_SHUXING_TYPE_HUNQI_IDENTIFY_BAOJI_PER,
	XL_SHUXING_TYPE_HUNQI_IDENTIFY_JIANREN_PER,

	XL_SHUXING_TYPE_MAX,
};

struct SZXilianShuxingCfg
{
	SZXilianShuxingCfg():hunqi_id(0), shuxing_type(0), type_weight(0)
	{
		memset(star_min, 0, sizeof(star_min));
		memset(star_max, 0, sizeof(star_max));
	}

	int hunqi_id;
	int shuxing_type;
	int type_weight;
	int star_min[SHENZHOU_XILIAN_MAX_STAR_COUNT];
	int star_max[SHENZHOU_XILIAN_MAX_STAR_COUNT];
};

struct SZXilianSuitCfg
{
	SZXilianSuitCfg():hunqi_id(0), need_start_count(0),
	maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_per(0)
	{}

	int hunqi_id;
	int need_start_count;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int add_per;
};

struct ShenzhouHunyinNewSuitCfg
{
	ShenzhouHunyinNewSuitCfg() : cfg_count(0)
	{
		memset(attr_cfg, 0, sizeof(attr_cfg));
	}

	int cfg_count;
	ShenzhouHunyinSuitCfg attr_cfg[SHENZHOU_NEW_SUIT_MAX_COUNT];
};

class ShenzhouWeaponConfig : public ConfigBase
{
public:
	ShenzhouWeaponConfig();
	~ShenzhouWeaponConfig();

	bool Init(const std::string &configname, std::string *err);

	const ShenzhouOtherCfg & GetOtherCfg() { return m_other_cfg; }

	std::vector<Posi> &GatherPointList() { return m_gather_point_list; }
	ShenzhouGatherRewardCfg * GetGatherRandReward();

	const ShenzhouExchangeIdentifyExpCfg * GetExchangeIdentifyExpCfg(int seq);

	int GetMaxIdentifyLevel() { return m_identify_level_cfg_count - 1; }
	const ShenzhouIdentifyLevelCfg * GetIdentifyLevelCfg(int level);
	const ShenzhouIdentifyLevelCfg::AttributeConfig * GetIdentifyAttributeCfg(int level, int star_level);
	const ShenzhouIdentifyLevelCfg::AttributeConfig * GetIdentifyNextAttributeCfg(int level, int star_level);

	ShenzhouIdentifyItemCfg * GetIdentifyItemById(int item_id);
	const int GetIdentifyConsumeItemList(int *item_list, int max_count);

	const ShenzhouSlotAttrCfg::LevelItemConfig * GetSlotLevelAttrCfg(int hunqi_id, int slot, int level);
	const ShenzhouElementCfg::ShenzhouElementLevelCfg * GetElemetLevelAttrCfg(int hunqi_id, int element_type, int elemet_level);

	ShenzhouSkillCfg * GetWeaponSkillCfg(int hunqi_id, int hunqi_level);
	int GetBoxRewardNum(int help_num);
	const std::vector<ShenzhouBoxRewardCfg> & GetBoxRewardCfg(){ return m_box_reward_vec; }
	ShenzhouBoxCfg & GetBoxCfg(){ return m_shen_zhou_box_cfg; }
	int GetBoxRewardTatolWeight(){ return m_box_reward_total_weight; }
	void GetBoxRandRewardByLevel(int level, std::vector<ShenzhouBoxRewardCfg> &box_reward_vec, int &all_weight);
	int GetBoxRewardCountByLevel(int level, int help_num);

	const ShenzhouHunYinCfg * GetHunYinCfg(int hunyin_id);
	const ShenzhouLingshuLevelCfg::LingshuLevelCfg * GetLingshuLevelAttrCfg(int hunqi_id, int hunyin_slot, int lingshu_level);
	const ShenzhouHunyinSuitCfg * GetHunyinSuitCfg(int hunqi_id, int suit_color);
	const ShenzhouHunyinSoltOpenCfg * GetHunyinSlotOpenCfg(int hunqi, int hunyin_slot);

	const SZXilianOpenCfg * GetXilianOpenCfg(int hunqi, int slot);
	const SZXilianLockCfg * GetXilianLockCfg(int lock_num);
	const int GetXilianConsuemeTotalWeight(int color);
	const SZXilianComsumeCfg * GetXilianConsumeCfg(int color);
	const int GetXilianShuxingTotalWeight(int hunqi);
	const SZXilianShuxingCfg * GetXilianShuxingCfg(int hunqi, int shuxing_type);
	std::map<int, std::map<int, SZXilianShuxingCfg> > & GetXilianShuxingMap() { return m_xilian_shuxing_map; }
	const SZXilianSuitCfg * GetXilianSuitCfg(int hunqi, int star_count);
	const SZXilianLockCfg * GetXilianLockCfg(int consume_color, int lock_num);

	const ShenzhouHunyinSuitCfg * GetSuitCfg(int hunqi, int color, int count);
	bool CheckSeq(int seq);

	//int GetRealHunqiNum() { return m_real_hunqi_num; }
	
protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitGatherPointsCfg(PugiXmlNode RootElement);
	int InitGatherRewardCfg(PugiXmlNode RootElement);
	int InitExchangeIdentifyExpCfg(PugiXmlNode RootElement);
	int InitIdentifyLevelCfg(PugiXmlNode RootElement);
	int InitIdentifyItemCfg(PugiXmlNode RootElement);
	int InitIdentifyRewardWeightCfg(PugiXmlNode RootElement);
	int InitSlotAttrCfg(PugiXmlNode RootElement);
	int InitWeaponSkillCfg(PugiXmlNode RootElement);
	int InitBoxCfg(PugiXmlNode RootElement);
	int InitBoxRewardCfg(PugiXmlNode RootElement);
	int InitElementLevelCfg(PugiXmlNode RootElement);
	int InitHunYinCfg(PugiXmlNode RootElement);
	int InitHunyinLingshuLevelCfg(PugiXmlNode RootElement);
	int InitHunyinSuitCfg(PugiXmlNode RootElement);
	int InitHunyinSlotOpenCfg(PugiXmlNode RootElement);
	int InitBoxRewardCountCfg(PugiXmlNode RootElement);
	int InitXilianOpenCfg(PugiXmlNode RootElement);
	int InitXilianLockCfg(PugiXmlNode RootElement);
	int InitXilianConsumeCfg(PugiXmlNode RootElement);
	int InitXilianShuxingCfg(PugiXmlNode RootElement);
	int InitXilianSuitCfg(PugiXmlNode RootElement);
	int InitNewHunyinSuitCfg(PugiXmlNode RootElement);

	ShenzhouOtherCfg m_other_cfg;

	std::vector<Posi> m_gather_point_list;

	int m_gather_reward_count_max;
	ShenzhouGatherRewardCfg m_gather_reward_list[SHENZHOU_MAX_GATHER_REWARD_COUNT]; // 采集奖励配置

	int m_exchange_identify_exp_cfg_count;
	ShenzhouExchangeIdentifyExpCfg m_exchange_identify_exp_list[SHENZHOU_EXCHANGE_IDENTIFY_EXP_CFG_COUNT];// 兑换鉴定经验配置

	int m_identify_level_cfg_count;
	ShenzhouIdentifyLevelCfg m_identify_level_cfglist[SHENZHOU_IDENTIFY_LEVEL_CFG_COUNT];// 鉴定等级配置

	int m_identify_item_cfg_count;
	ShenzhouIdentifyItemCfg m_identify_comsume_item_cfglist[SHENZHOU_IDENTIFY_CONSUME_ITEM_MAX_COUNT];// 鉴定消耗物品配置

	ShenzhouSlotAttrCfg m_slot_attr_cfglist[SHENZHOU_WEAPON_COUNT][SHENZHOU_WEAPON_SLOT_COUNT];// 部位属性

	ShenzhouSkillCfg m_weapon_skill_cfg[SHENZHOU_WEAPON_COUNT][ShenzhouSlotAttrCfg::CFG_MAX_COUNT + 1];		// 技能配置

	ShenzhouElementCfg m_element_level_cfg[SHENZHOU_WEAPON_COUNT];		// 炼魂元素配置

	std::map<int, ShenzhouHunYinCfg> m_hunyin_map;		// 魂印配置
	ShenzhouLingshuLevelCfg m_hunyin_lingshu_level_cfg[SHENZHOU_WEAPON_COUNT]; // 魂印灵枢等级配置
	ShenzhouHunyinSuitCfg m_hunyin_suit_cfg[SHENZHOU_WEAPON_COUNT][HUNYIN_COLOR_MAX]; // 魂印套装属性
	ShenzhouHunyinSoltOpenCfg m_hunyin_slot_open_cfg[SHENZHOU_WEAPON_COUNT][SHENZHOU_HUNYIN_MAX_SLOT];		// 魂印槽开启配置

	ShenzhouBoxCfg m_shen_zhou_box_cfg;					// 宝箱
	int m_box_reward_total_weight;						// 宝箱奖励总权重
	std::vector<ShenzhouBoxRewardCfg> m_box_reward_vec;	// 宝箱奖励

	std::vector<BoxRewardCountCfg> m_box_reward_count_vec;

	SZXilianOpenCfg m_xilian_open_cfg[SHENZHOU_WEAPON_COUNT][SHENZHOU_HUNYIN_MAX_SLOT];
	SZXilianLockCfg m_xilian_lock_cfg[SHENZHOU_HUNYIN_MAX_SLOT];
	UNSTD_STATIC_CHECK(XL_COUMME_COLOR_MAX > 0);
	int m_xilian_consuem_total_weight[XL_COUMME_COLOR_MAX];
	SZXilianComsumeCfg m_xilian_consume_cfg[XL_COUMME_COLOR_MAX];
	int m_xilian_shuxing_total_weight[SHENZHOU_WEAPON_COUNT];
	std::map<int, std::map<int, SZXilianShuxingCfg> > m_xilian_shuxing_map;

	int m_xilian_suit_count[SHENZHOU_WEAPON_COUNT];
	SZXilianSuitCfg m_xilian_suit_cfg[SHENZHOU_WEAPON_COUNT][SHENZHOU_XILIAN_SUIT_MAX_COUNT];

	ShenzhouHunyinNewSuitCfg m_new_hunyin_suit_cfg[SHENZHOU_WEAPON_COUNT][HUNYIN_COLOR_MAX];

	//int m_real_hunqi_num;							//实际异火数量

	//锁洗消耗配置
	//std::vector<SZXilianLockCfg> m_lock_consume_vct;
};

#endif

