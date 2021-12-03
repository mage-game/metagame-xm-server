#ifndef __FAIRY_BUDDHA_CARD_CONFIG_HPP__
#define __FAIRY_BUDDHA_CARD_CONFIG_HPP__
#include <string>
#include "servercommon/taskdef.hpp"
#include "gamedef.h"
#include "servercommon/struct/itemlistparam.h"
#include "item/itempool.h"
#include "servercommon/configbase.h"

struct  FairyBuddhaCardCfg
{
	FairyBuddhaCardCfg() : use_gold(0), add_gongji(0), title_id(0), add_mojing_per(0), monster_exp(0), day_gold_bind(0), useful_time(0){}
	int use_gold;
	int add_gongji;
	int title_id;
	int add_mojing_per;
	int monster_exp;
	int day_gold_bind;
	int useful_time;
	ItemConfigData stuff_item;
};

class FairyBuddhaCardConfig :public ConfigBase
{
public:
	FairyBuddhaCardConfig();
	~FairyBuddhaCardConfig();

	bool Init(const std::string &configname, std::string *err);

	const  FairyBuddhaCardCfg * GetFairyBuddhaCardCfg(int card_type);

	const int GetFairyBuddhaAllGoldBind();
private:
	
	const static int FAIRY_BUDDHA_CARD_TYPE_MAX = 5;
	FairyBuddhaCardCfg m_fairy_buddha_card_cfg[FAIRY_BUDDHA_CARD_TYPE_MAX];
	int use_gold;

};

#endif // __FAIRY_BUDDHA_CARD_CONFIG_HPP__