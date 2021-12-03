#ifndef __MAGIC_CARD_CONFIG_H__
#define __MAGIC_CARD_CONFIG_H__

#include "obj/character/attribute.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/magiccarddef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <set>
#include "item/itembase.h"

struct MagicCardOtherConfig
{
	MagicCardOtherConfig() : open_level(0), purple_chou_card_consume_gold(0), orange_chou_card_consume_gold(0), orange_chou_card_consume_item_id(0), day_free_times(0), red_chou_card_consume_gold(0), red_chou_card_consume_item_id(0) {}

	int open_level;												// 开放等级
	int purple_chou_card_consume_gold;							// 紫卡抽奖1次消耗元宝
	int orange_chou_card_consume_gold;							// 橙卡抽奖1次消耗元宝
	int orange_chou_card_consume_item_id;						//橙卡抽奖1次消耗物品
	int day_free_times;											// 每天免费次数
	int red_chou_card_consume_gold;								// 红卡抽奖1次消耗元宝
	int red_chou_card_consume_item_id;							//红卡抽奖1次消耗物品
};

struct MagicCardSuitConfig
{
	MagicCardSuitConfig() : color(0), skill_need_level(0), skill_id(0), attr_need_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

	int color;																// 魔卡颜色

	int skill_need_level;													// 全套达到指定等级激活技能
	int skill_id;															// 激活技能ID

	int attr_need_level;													// 全套达到指定等级激活套装属性
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
};

struct MagicCardInfoConfig
{
	struct StrengthAttr
	{
		StrengthAttr() : strength_level(0), up_level_need_exp(0), contain_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), 
			shanbi(0) {}

		int strength_level;						// 强化等级
		int up_level_need_exp;					// 升级所需经验
		int contain_exp;						// 被吞噬转化的经验


		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
	};

	MagicCardInfoConfig() : card_id(0), item_id(ItemBase::INVALID_ITEM_ID), color(0), slot_index(0), type(0), need_sprit_num(0), strength_max_level(0) {}

	const StrengthAttr * GetStrenthAttrCfg(int strength) const;

	int card_id;															// 魔卡ID
	ItemID item_id;															// 对应的物品ID
	int color;																// 魔卡颜色
	int slot_index;															// 卡槽位置
	int type;																// 类型
	int need_sprit_num;														// 兑换需要魔魂数量

	int strength_max_level;
	StrengthAttr strength_attr_list[MAGIC_CARD_LIMIT_STRENGTH_LEVEL_MAX + 1]; // 强化配置列表，下标就是强化等级
};

struct MagicCardChouCardConfig
{
	MagicCardChouCardConfig() : reward_seq(0), reward_card_id(0), reward_count(0), weight(0), is_broadcast(false) {}

	int reward_seq;															// 奖品序号
	int reward_card_id;														// 奖品魔卡ID
	int reward_count;														// 奖品数量
	int weight;																// 权重

	bool is_broadcast;														// 是否发传闻
};

enum MAGIC_CARD_CHOU_CARD_TYPE
{
	MAGIC_CARD_CHOU_CARD_TYPE_PURPLE = 0,									// 紫卡抽奖
	MAGIC_CARD_CHOU_CARD_TYPE_ORAGEN,										// 橙卡抽奖
	MAGIC_CARD_CHOU_CARD_TYPE_RED,											// 红卡抽奖

	MAGIC_CARD_CHOU_CARD_TYPE_MAX_NUM,
};

enum MAGIC_CARD_CARD_TYPE
{
	MAGIC_CARD_CARD_TYPE_NORMAL_CARD = 0,									// 普通卡牌卡牌
	MAGIC_CARD_CARD_TYPE_SPRITE,											// 魔魂
	MAGIC_CARD_CARD_TYPE_EXP_CARD,											// 经验卡

	MAGIC_CARD_CARD_TYPE_MAX_NUM,
};


class MagicCardConfig : public ConfigBase
{
public:
	MagicCardConfig();
	~MagicCardConfig();

	bool Init(const std::string &configname, std::string *err);

	const MagicCardOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const MagicCardSuitConfig * GetCardSuitCfg(int color);
	const MagicCardInfoConfig * GetCardCfg(int card_id);
	const MagicCardChouCardConfig * GetRandomRewardCfg(int type);
	int GetSpriteCardId(int color);
	int GetCardIdWithItemID(ItemID item_id);

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitCardSuitCfg(PugiXmlNode RootElement);
	int InitAllCardInfoCfg(PugiXmlNode RootElement);
	int InitStrengthCfg(PugiXmlNode RootElement);
	int InitPurpleChouCardCfg(PugiXmlNode RootElement);
	int InitOrangeChouCardCfg(PugiXmlNode RootElement);
	int InitRedChouCardCfg(PugiXmlNode RootElement);
	MagicCardOtherConfig m_other_cfg;

	int m_card_suit_cfg_count;
	MagicCardSuitConfig m_card_suit_cfg_list[MAGIC_CARD_COLOR_TYPE_COLOR_COUNT];

	int m_all_card_count;
	MagicCardInfoConfig m_all_card_list[MAGIC_CARD_MAX_LIMIT_COUNT + 1];

	std::map<ItemID, int> m_item_id_to_card_id_map;

	int m_purple_choucard_cfg_count;
	MagicCardChouCardConfig m_purple_choucard_cfg_list[MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT];

	int m_orange_choucard_cfg_count;
	MagicCardChouCardConfig m_orange_choucard_cfg_list[MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT];

	int m_red_choucard_cfg_count;
	MagicCardChouCardConfig m_red_choucard_cfg_list[MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT];

	int m_sprite_list_cout;
	int m_sprite_card_id_list[MAGIC_CARD_COLOR_TYPE_COLOR_COUNT];
};

#endif

