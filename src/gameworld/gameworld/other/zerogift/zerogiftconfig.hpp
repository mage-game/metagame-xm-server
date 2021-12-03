#ifndef __ZERO_GIFT_CONFIG_HPP__
#define __ZERO_GIFT_CONFIG_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"
#include "servercommon/struct/zerogiftparam.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct ZeroGiftPhaseCfg
{
	ZeroGiftPhaseCfg() : seq(-1), open_day(0), active_level(0), level_limit(0), vaild_buy_time(0), buy_gold(0), need_wait_time(0), is_bind_gold(0), reward_gold(0) {}

	short seq;
	short open_day;
	short active_level;
	short level_limit;
	unsigned int vaild_buy_time;
	int buy_gold;
	unsigned int need_wait_time;
	short is_bind_gold;
	int reward_gold;
	ItemConfigData gift_item[MAX_ATTACHMENT_ITEM_NUM];

	static const int REWARD_DAY_COUNT = 3;
	ItemConfigData reward_item_list[REWARD_DAY_COUNT][MAX_ATTACHMENT_ITEM_NUM];
};

struct ZeroGiftGodCostumeCfg
{
	ZeroGiftGodCostumeCfg() : seq(-1), open_day(0), vaild_buy_time(0), buy_gold(0), need_wait_time(0), is_bind_gold(0), reward_gold(0) 
	{
		memset(active_level, 0, sizeof(active_level));
		memset(level_limit, 0, sizeof(level_limit));
	}

	short seq;
	short open_day;
	short active_level[ZERO_GIFT_GOD_LEVEL_PHASE];
	short level_limit[ZERO_GIFT_GOD_LEVEL_PHASE];
	unsigned int vaild_buy_time;
	int buy_gold;
	unsigned int need_wait_time;
	short is_bind_gold;
	int reward_gold;
	ItemConfigData gift_item[MAX_ATTACHMENT_ITEM_NUM];

	ItemConfigData reward_item_list[ZERO_GIFT_GOD_LEVEL_PHASE][MAX_ATTACHMENT_ITEM_NUM];
};

class ZeroGifgConfig : public ConfigBase
{
public:
	ZeroGifgConfig();
	~ZeroGifgConfig();

	bool Init(std::string path, std::string *err);

	const ZeroGiftPhaseCfg * GetPhaseCfg(int seq);
	const ZeroGiftGodCostumeCfg * GetGodCostumeCfg(int seq);
	const int GetPhaseNum(){ return m_zero_gift_pahse_max;}
	const int GetGodCostumeNum(){ return m_zero_gift_god_costume_max;}

private:
	int InitPhaseCfg(PugiXmlNode RootElement);
	int InitGodCostumeCfg(PugiXmlNode RootElement);	//ËÄ½×Éñ×°ÅäÖÃ

	int m_zero_gift_pahse_max;
	ZeroGiftPhaseCfg m_phase_cfg[ZERO_GIFT_PHASE_MAX];
	int m_zero_gift_god_costume_max;
	ZeroGiftGodCostumeCfg m_god_costume_cfg[ZERO_GIFT_GOD_COSTUME_MAX];
};

#endif // __ZERO_GIFT_CONFIG_HPP__

