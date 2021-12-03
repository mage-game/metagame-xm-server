#ifndef __CARD_ZU_CONFIG_H__
#define __CARD_ZU_CONFIG_H__

#include "obj/character/attribute.hpp"
#include "servercommon/cardzudef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "common/tinyxml/tinyxml.h"
#include <set>

struct CardzuOtherConfig
{
	CardzuOtherConfig() : choucard_need_coin(0), choucard_need_gold(0), choucard_batch_need_gold(0), choucard_need_gold_bind(0), coin_choucard_lingli(0), gold_choucard_lingli(0), 
		batch_gold_choucard_lingli(0), gold_bind_choucard_lingli(0), coin_chouka_max_time(0), gold_bind_chouka_max_time(0), choucard_batch_need_item_id(0) {}

	int choucard_need_coin;													// 铜币抽卡消耗
	int choucard_need_gold;													// 元宝单抽消耗
	int choucard_batch_need_gold;											// 元宝批量抽消耗
	int choucard_need_gold_bind;											// 绑钻单抽消耗
	int coin_choucard_lingli;												// 铜币抽卡获得灵力
	int gold_choucard_lingli;												// 元宝单抽获得灵力
	int batch_gold_choucard_lingli;											// 元宝十连抽获得灵力
	int gold_bind_choucard_lingli;											// 绑钻单抽获得灵力
	int coin_chouka_max_time;												// 铜币抽卡每天限制次数
	int gold_bind_chouka_max_time;											// 绑钻抽卡每天限制次数
	int choucard_batch_need_item_id;										// 批量抽消耗物品
};

struct CardzuActiveConfig
{
	CardzuActiveConfig() : cardzu_id(0), need_active_count(0) {}

	int cardzu_id;															// 卡牌ID
	int need_active_count;													// 激活需要上层组合数
};

struct CardzuCardConfig
{
	CardzuCardConfig() : card_id(0), cardzu_id(0), star_count(0), sell_lingli(0), need_lingli(0), coin_weight(0), gold_weight(0), gold_batch_weight(0), gold_bind_weight(0), broadcast(false) {}

	int card_id;															// 卡牌ID
	int cardzu_id;															// 所属卡组ID
	int star_count;															// 星数
	int sell_lingli;														// 化灵可得
	int need_lingli;														// 铸灵所需

	int coin_weight;														// 免费获取权重
	int gold_weight;														// 元宝抽取权重
	int gold_batch_weight;													// 元宝十连抽权重
	int gold_bind_weight;													// 绑钻抽取权重

	bool broadcast;						 									// 是否公告
};

struct CardzuZuheConfig
{
	struct AttributeConfig
	{
		AttributeConfig() : zuhe_id(0), zuhe_level(0), upgrade_need_lingli(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

		int zuhe_id;														// 卡牌组合id
		int zuhe_level;														// 卡牌组合等级
		int upgrade_need_lingli;											// 升级所需灵力

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
	};

	CardzuZuheConfig() : zuhe_id(0), cardzu_id(0), need_card1_id(0), need_card1_num(0), need_card2_id(0), need_card2_num(0),
		need_card3_id(0), need_card3_num(0), zuhe_max_level(0) {}

	const AttributeConfig *GetAttributeCfg(int zuhe_level) const;

	int zuhe_id;															// 卡牌组合id
	int cardzu_id;															// 所属卡组ID

	int need_card1_id;														// 组合所需卡牌1 id
	int need_card1_num;														// 组合所需卡牌1 数量
	int need_card2_id;														// 组合所需卡牌2 id
	int need_card2_num;														// 组合所需卡牌2 数量
	int need_card3_id;														// 组合所需卡牌3 id
	int need_card3_num;														// 组合所需卡牌3 数量

	int zuhe_max_level;														// 卡牌属性个数
	AttributeConfig attribute_cfglist[CARDZU_ZUHE_MAX_LEVEL + 1];			// 卡牌属性列表（不同等级）
};

class CardzuConfig : public ConfigBase
{
public:
	typedef std::set<int> CardzuCardSet;
	typedef std::set<int>::iterator CardzuCardSetIt;

	typedef std::set<int> CardzuZuheSet;
	typedef std::set<int>::iterator CardzuZuheSetIt;

	CardzuConfig();
	~CardzuConfig();

	bool Init(const std::string &configname, std::string *err);

	const int GetCardzuIDWithCardID(int card_idx) const;					// 根据卡牌ID获取该卡牌所属卡组ID
	const int GetCardzuIDWithZuheID(int zuhe_idx) const;					// 根据卡牌组合ID获取该卡牌所属卡组ID

	const CardzuOtherConfig & GetOtherCfg() { return m_other_cfg; }
	int GetCardzuNeedActiveCount(int carzu_id);
	const CardzuCardConfig * GetCardCfg(int card_idx);
	const CardzuZuheConfig * GetZuheCfg(int zuhe_idx);
	const CardzuZuheConfig::AttributeConfig * GetZuheAttributeCfg(int zuhe_idx, int zuhe_level);
	CardzuZuheSet *GetZuheSet(int cardzu_id);
	const CardzuCardConfig *GetRandomCard(int choutcard_type, int cardzu_id, int card_star_num_min, int card_star_num_max);

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitCardzuInfoCfg(PugiXmlNode RootElement);
	int InitCardInfoCfg(PugiXmlNode RootElement);
	int InitZuheInfoCfg(PugiXmlNode RootElement);
	int InitZuheUpgradeCfg(PugiXmlNode RootElement);

	CardzuOtherConfig m_other_cfg;
	CardzuActiveConfig m_cardzu_active_cfglist[CARDZU_TYPE_MAX_COUNT];			// 卡组信息

	int m_max_card_id;
	CardzuCardConfig m_card_cfglist[CARDZU_MAX_CARD_ID + 1];					// 卡牌信息

	int m_max_zuhe_id;
	CardzuZuheConfig m_zuhe_cfglist[CARDZU_MAX_ZUHE_ID + 1];					// 卡牌组合信息

	CardzuCardSet m_cardzu_cardset_list[CARDZU_TYPE_MAX_COUNT];					// 卡牌映射数据（分组）
	CardzuZuheSet m_cardzu_zuheset_list[CARDZU_TYPE_MAX_COUNT];					// 卡牌组合映射数据（分组）
};

#endif

