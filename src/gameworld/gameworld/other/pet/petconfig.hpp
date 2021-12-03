#ifndef __PET_CONFIG_H__
#define __PET_CONFIG_H__

#include "obj/character/attribute.hpp"
#include "common/tinyxml/tinyxml.h"
#include <set>
#include "item/itembase.h"
#include "servercommon/petdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

struct PetLevelConfig
{
	PetLevelConfig() : level(0), uplevel_consume_coin(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

	int level;															// 等级
	int uplevel_consume_coin;											// 升到下一级消耗铜币数

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
};

struct PetUpGradeRateConfig
{
	PetUpGradeRateConfig() : grade(0), low_bless(0), high_bless(0), succ_rate(0), fail_add_1_rate(0), fail_add_2_rate(0), fail_add_3_rate(0) {}

	int grade;															// 当前阶数
	int low_bless;														// 祝福值下限
	int high_bless;														// 祝福值上限
	int succ_rate;														// 成功率万分比
	int fail_add_1_rate;												// 失败加8祝福值概率
	int fail_add_2_rate;												// 失败加10祝福值概率
	int fail_add_3_rate;												// 失败加12祝福值概率

	int GetRandomZhufuOnFail() const;
};

struct PetGradeConfig
{
	PetGradeConfig() : grade(0), consume_stuff_id(ItemBase::INVALID_ITEM_ID), consume_stuff_num(0), max_bless_value(0), is_broadcast(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

	int grade;															// 阶数
	int consume_stuff_id;												// 进阶消耗物品ID
	int consume_stuff_num;												// 进阶消耗物品数量
	int max_bless_value;												// 最大祝福值

	int is_broadcast;													// 是否传闻

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
};

struct PetUpgradeRateConfigArr
{
	std::vector<PetUpGradeRateConfig> vect;
};


struct PetInfoConfig
{
	PetInfoConfig() : pet_id(INVALID_PET_ID), active_skill_num(0), passive_skill_num(0), max_level(0), max_grade(-1) 
	{
		memset(pet_name, 0, sizeof(pet_name));
	}

	int pet_id;															// 宠物ID
	GameName pet_name;													// 宠物名字
	int active_skill_num;												// 携带主动技能的数量
	int passive_skill_num;												// 携带被动技能的数量

	int max_level;														// 宠物配置最大等级
	PetLevelConfig level_cfg_list[PET_MAX_LEVEL_LIMIT + 1];

	int max_grade;														// 宠物配置最大阶数
	PetGradeConfig grade_cfg_list[PET_MAX_GRADE_LIMIT + 1];
};

struct PetOtherConfig
{
	PetOtherConfig() : low_chou_consume_gold(0), low_chou_consume_item_id(ItemBase::INVALID_ITEM_ID), high_chou_consume_gold(0), high_chou_consume_item_id(ItemBase::INVALID_ITEM_ID), 
		free_chou_cold_time_hour(0), rename_card_id(ItemBase::INVALID_ITEM_ID), food_market_free_times_per_day(0), food_market_once_need_gold(0), food_market_ten_times_need_gold(0) {}

	int low_chou_consume_gold;											// 低级抽奖消费元宝
	ItemID low_chou_consume_item_id;									// 低级抽奖消费物品ID
	int high_chou_consume_gold;											// 高级抽奖消费元宝
	ItemID high_chou_consume_item_id;									// 高级抽奖消费物品ID
	int free_chou_cold_time_hour;										// 免费抽奖冷却时间
	ItemID rename_card_id;												// 宠物改名卡物品ID

	int food_market_free_times_per_day;									// 吃货市场每天免费次数
	int food_market_once_need_gold;										// 吃货市场一次所需元宝
	int food_market_ten_times_need_gold;								// 吃货市场十次所需元宝
};

struct PetRewardConfig
{
	PetRewardConfig() : seq(0), is_get_reward(0), low_weight(0), height_weight(0), is_broadcast(false){}

	int seq;															// 奖品索引
	int is_get_reward;													// 是否能够获得奖励
	ItemConfigData reward_item;											// 奖励物品
	int low_weight;														// 低级抽奖抽到的权重
	int height_weight;													// 高级抽奖抽到的权重

	bool is_broadcast;													// 是否传闻
};

struct PetEggConfig
{
	PetEggConfig() : item_id(ItemBase::INVALID_ITEM_ID), transform_item_id(ItemBase::INVALID_ITEM_ID), transform_item_num(0), pet_id(INVALID_PET_ID) {}

	ItemID item_id;														// 宠物蛋物品ID
	ItemID transform_item_id;											// 转化成物品ID
	int transform_item_num;												// 转化成物品数量
	int pet_id;															// 变成宠物的ID
};

struct PetExtraAttrConfig
{
	PetExtraAttrConfig() : seq(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) 
	{
		for(int i = 0; i < PET_MAX_EXTRA_ATTR_PET_COUNT_LIMIT; ++ i)
		{
			pet_id_list[i] = INVALID_PET_ID;
		}
	}

	int pet_id_list[PET_MAX_EXTRA_ATTR_PET_COUNT_LIMIT];
	int seq;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
};

struct PetSkillConfig
{
	static const int MAX_SKILL_LEVEL_LIMIT = 3;

	struct LevelConfig
	{
		LevelConfig() : level(0), consume_item_id(ItemBase::INVALID_ITEM_ID), consume_item_num(0) {}

		int level;
		int consume_item_id;
		int consume_item_num;
	};

	PetSkillConfig() : skill_index(-1), skill_id(0), own_by_pet_id(INVALID_PET_ID), max_skill_level(0) {}

	int skill_index;
	int skill_id;
	int own_by_pet_id;

	int max_skill_level;
	LevelConfig skill_level_cfglist[MAX_SKILL_LEVEL_LIMIT + 1];
};

enum PET_CHOU_TYPE
{
	PET_CHOU_TYPE_LOW = 0,												// 宠物低级抽奖
	PET_CHOU_TYPE_HIGH,													// 宠物高级抽奖
	
	PET_CHOU_TYPE_MAX
};

struct PetSpecialConfig
{
	PetSpecialConfig():image_id(0),maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

	int image_id;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
};

struct PetQinmiLevelConfig
{
	struct StarConfig
	{
		StarConfig() : star(0), need_qinmi_val(0), ignore_fangyu_percent(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

		int star;
		int need_qinmi_val;
		Attribute ignore_fangyu_percent;

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
	};

	PetQinmiLevelConfig() : qinmi_level(0), max_star(0) {}

	int qinmi_level;
	int max_star;
	StarConfig star_cfg_list[PET_QINMI_LEVEL_MAX_STAR + 1];
};

struct PetFoodMarketConfig
{
	PetFoodMarketConfig() : seq(0), low_weight(0), hight_weight(0), can_feed(false), add_qinmi_val(0) {}

	int seq;
	ItemConfigData reward_item;
	int low_weight;
	int hight_weight;
	bool can_feed;
	int add_qinmi_val;
};

struct PetSpecialImageUplevelConfig
{
	PetSpecialImageUplevelConfig() : max_level(0)
	{
		
	}

	struct UplevelItemConfig
	{
		UplevelItemConfig() : level(0), item_id(0), item_num(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0)
		{
			
		}

		int level;
		int item_id;
		int item_num;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
	};

	int max_level;

	UplevelItemConfig uplevel_cfg_list[MAX_SPECIAL_IMG_LEVEL + 1];
};

class PetConfig : public ConfigBase
{
public:
	PetConfig();
	~PetConfig();

	bool Init(const std::string &configname, std::string *err);

	const PetOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const PetLevelConfig * GetPetLevelCfg(int id, int level);
	const PetGradeConfig * GetPetGradeCfg(int id, int grade);
	const PetRewardConfig * GetRandomRewardCfg(int type);
	const PetEggConfig * GetPetEggCfg(int pet_id);
	const PetEggConfig * GetPetEggCfgByItemid(ItemID egg_id);
	const PetInfoConfig * GetPetInfo(int id); 
	const PetUpGradeRateConfig * GetSuccRate(int grade, short bless);
	const PetExtraAttrConfig * GetExtraAttrPetList(int index);
	int GetExtraAttrCount() { return m_extra_attr_cfg_count; }
	int GetPetMaxGrade() { return m_grade_cfg_count; }
	const PetSkillConfig * GetSkillWithId(int skill_id);
	const PetSkillConfig * GetSkillWithIndex(int skill_index);
	const PetSpecialConfig * GetSpecialImgCfg(int image_id);
	int GetMaxQinmiLevel() { return m_max_qinmi_level; }
	const PetQinmiLevelConfig::StarConfig * GetQinmiLevelStarCfg(int level, int star);
	const PetFoodMarketConfig * GetFoodMarketCfg(int seq);
	int GetPetQinmiLevelMaxStar(int level);
	int GetPetCount() { return m_all_pet_cfg_count; }
	int GetPetFoodSeq(int index);
	const PetFoodMarketConfig * GetFoodMarketOnceRandomRewardCfg();
	bool GetFoodMarketRandomRewards(int result_list[], int list_len);
	const PetSpecialImageUplevelConfig::UplevelItemConfig * GetSpecialImgUplevelCfg(const int image_id, const int to_level);

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitPetInfoCfg(PugiXmlNode RootElement);
	int InitPetUplevelCfg(PugiXmlNode RootElement);
	int InitPetUpgradeCfg(PugiXmlNode RootElement);
	int InitPetUpgradeRateCfg(PugiXmlNode RootElement);
	int InitPetChoujiangRewardCfg(PugiXmlNode RootElement);
	int InitPetEggCfg(PugiXmlNode RootElement);
	int InitPetExtraAttrCfg(PugiXmlNode RootElement);
	int InitPetSkillCfg(PugiXmlNode RootElement);
	int InitPetSkillLevelCfg(PugiXmlNode RootElement);
	int InitPetSpecialCfg(PugiXmlNode RootElement);
	int InitPetQinmiLevelCfg(PugiXmlNode RootElement);
	int InitPetFoodMarketCfg(PugiXmlNode RootElement);
	int InitPetSpecialUplevelCfg(PugiXmlNode RootElement);

	PetOtherConfig m_other_cfg;

	int m_all_pet_cfg_count;
	PetInfoConfig m_all_pet_cfg_list[PET_MAX_COUNT_LIMIT];

	int m_egg_cfg_count;
	PetEggConfig m_egg_cfg_list[PET_EGG_MAX_COUNT_LIMIT];

	int m_reward_cfg_count;
	PetRewardConfig m_pet_reward_list[PET_REWARD_CFG_COUNT_LIMIT];

	int m_grade_cfg_count;
	PetUpgradeRateConfigArr m_grade_cfg_list[PET_MAX_GRADE_LIMIT + 1];

	int m_extra_attr_cfg_count;
	PetExtraAttrConfig m_extra_attr_cfg_list[PET_MAX_EXTRA_ATTR_CFG_COUNT_LIMIT];

	int m_skill_cfg_list_count;
	PetSkillConfig m_skill_cfg_list[PET_SKILL_CFG_MAX_COUNT_LIMIT];
	std::map<int, int> m_skill_id_2_skill_index_map;

	int m_max_special_img_id;
	PetSpecialConfig m_special_img_cfg_list[PET_SPECIAL_IMG_MAX_ID_LIMIT + 1];

	int m_max_qinmi_level;
	PetQinmiLevelConfig m_qinmi_level_cfg_list[PET_QINMI_LEVEL_MAX_LEVEL + 1];

	int m_food_market_cfg_count;
	PetFoodMarketConfig m_food_market_cfg_list[PET_FOOD_MARKET_MAX_REWARD_COUNT];
	ItemID m_food_item_id_list[PET_FOOD_MARKET_MAX_FOOD_COUNT];

	typedef std::map<int, PetSpecialImageUplevelConfig> SpecialImgUplevelCfgMap;
	SpecialImgUplevelCfgMap m_special_img_uplevel_map;
};

#endif

