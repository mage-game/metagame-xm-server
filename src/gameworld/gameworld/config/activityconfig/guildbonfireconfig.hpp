#ifndef _GUILD_BONFIRE_CONFIG_HPP_
#define _GUILD_BONFIRE_CONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

static const int GUILD_BONFIRE_LV_CFG_MAX = 10;
static const int GUILD_BONFIRE_SCENE_MAX = 10;

struct GuildBonfireLvCfg
{
	GuildBonfireLvCfg() : min_lv(0), max_lv(0), base_exp(0) {}

	int min_lv;
	int max_lv;

	long long base_exp;
	ItemConfigData gather_reward[MAX_ATTACHMENT_ITEM_NUM];	
};

struct GuildBonfireTimesCfg
{
	GuildBonfireTimesCfg() : min_lv(0), max_lv(0), base_exp(0), base_bangong(0) {}

	int min_lv;
	int max_lv;

	long long base_exp;
	int base_bangong;
};

class GuildBonfireConfig : public ConfigBase
{
public:
	GuildBonfireConfig();
	~GuildBonfireConfig();

	bool Init(const std::string &configname, std::string *err);
	
	int GetGuildExpCost() { return m_guildexp_cost; }
	
	int GetTimeRewardCDs() { return m_timereward_cd_s; }
	int GetTimeRewardRange() { return m_timereward_range; }
	const GuildBonfireTimesCfg * GetTimeRewardByLv(int role_lv);

	int GetGatharID() { return m_gathar_id; }
	int GetGatharTimeS() { return m_gathar_time_s; }
	int GetGatharExistTimeS() { return m_gathar_exist_time_s; }
	int GetGatharSelfMax() { return m_gathar_self_max; }
	int GetGatharMax() { return m_gathar_max; }
	const GuildBonfireLvCfg * GetGatharRewardByLv(int role_lv);
	const void GetAddMucaiReward(ItemConfigData *mucai_add_reward);
	const int GetAddMucaiBangong(){return mucai_add_banggong;}
	const int GetAddMucaiMaxCount() {return mucai_add_count_limit;}

	int GetMuCaiItemID() { return m_mucai_itemid; }
	bool IsOpenFireScene(int scene_id);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitTimeRewardCfg(PugiXmlNode RootElement);
	int InitGatharRewardCfg(PugiXmlNode RootElement);
	int InitOpenFireSceneCfg(PugiXmlNode RootElement);

	int m_guildexp_cost;

	int m_timereward_cd_s;
	int m_timereward_range;
	GuildBonfireTimesCfg m_timereward_cfg_list[GUILD_BONFIRE_LV_CFG_MAX];
	int m_timereward_gongxian;

	int m_gathar_id;
	int m_gathar_time_s;
	int m_gathar_exist_time_s;

	int m_gathar_max;
	int m_gathar_self_max;
	GuildBonfireLvCfg m_gatharreward_cfg_list[GUILD_BONFIRE_LV_CFG_MAX];
	
	int m_mucai_itemid;
	ItemConfigData m_mucai_add_reward[MAX_ATTACHMENT_ITEM_NUM];
	int mucai_add_banggong;
	int mucai_add_count_limit;

	int m_openfire_scene_list[GUILD_BONFIRE_SCENE_MAX];
};

#endif // _GUILD_BONFIRE_CONFIG_HPP_





