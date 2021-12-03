#ifndef __BOSSCARD_CONFIG_HPP__
#define __BOSSCARD_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/bosscarddef.hpp"
#include <map>

struct BossCardOtherConfig
{
	BossCardOtherConfig() : open_level(0) {}
	
	int open_level;
};

struct BossCardActiveConfig
{
	BossCardActiveConfig() : monster_id(0), monster_seq(0), gongji(0), maxhp(0), fangyu(0), per_pojia(0), card_type(0), scene_id(0)
	{
	}

	int monster_id;
	int monster_seq;

	Attribute gongji;																			
	Attribute maxhp;																		
	Attribute fangyu;																		
	Attribute per_pojia;
	int card_type;
	int scene_id;
};

struct BossCardRewardConfig
{
	BossCardRewardConfig() : card_type(0){}

	int card_type;
	ItemConfigData reward_item;
};

class BossCardConfig : public ConfigBase
{
public:
	BossCardConfig();
	~BossCardConfig() {}

	bool Init(const std::string &configname, std::string *err);
	const BossCardOtherConfig & GetOtherCfg() const { return m_other_cfg; }

	int GetBossCardCfgSeq(int monster_id);
	const BossCardActiveConfig * GetBossCardCfg(int monster_seq);
	int GetBossCardMaxCardType() { return now_max_bosscard_type; }
	const BossCardRewardConfig *GetBossCardRewardCfg(int card_type);

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitBossCardInfoCfg(PugiXmlNode RootElement);
	int InitBossCardRewardCfg(PugiXmlNode RootElement);

private:
	BossCardOtherConfig m_other_cfg;
	int now_max_bosscard_type;

	std::map<int, BossCardActiveConfig> m_boss_card_cfg_list;		//Í¼¼øÁÐ±í
	BossCardRewardConfig m_bosscard_reward_cfg[BOSS_CARD_MAX_TYPE + 1]; //Í¼¼ø½±Àø
};

#endif
