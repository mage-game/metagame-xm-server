#ifndef __TEAM_FB_EQUIP_CONFIG_HPP__
#define __TEAM_FB_EQUIP_CONFIG_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/fbdef.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

static const int MAX_BUY_TIME_CFG_COUNT = 20;
struct TeamEquipFBOtherCfg
{
	TeamEquipFBOtherCfg() : max_count(0) {}

	int max_count;
};

struct TeamEquipFBLayerCfg
{
	TeamEquipFBLayerCfg() : layer(0), level_limit(0), scene_id(0), enter_pos(0, 0), day_exp(0), day_shengwang(0), mojing(0), assist_mojing(0),
		boss_id(0), born_pos(0, 0), first_item_count(0), pass_item_count(0) {}

	int layer;
	int level_limit;
	int scene_id;
	Posi enter_pos;
	long long day_exp;
	int day_shengwang;
	int mojing;
	int assist_mojing;
	int boss_id;
	Posi born_pos;

	int first_item_count;
	ItemConfigData first_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int pass_item_count;
	ItemConfigData pass_item_list[MAX_ATTACHMENT_ITEM_NUM];

	std::vector<UInt16> drop_id_vec[TEAM_EQUIP_FB_ROLE_MAX];
};

struct TeamEquipBuyTimesCostCfg
{
	TeamEquipBuyTimesCostCfg():buytimes(0), gold_cost(0){}

	int buytimes;
	int gold_cost;
};

class TeamEquipFBConfig : public ConfigBase
{
public:
	TeamEquipFBConfig();
	~TeamEquipFBConfig();

	bool Init(const std::string &configname, std::string *err);

	const TeamEquipFBOtherCfg & GetOtherCfg();

	int GetFBLayerBySceneID(int scene_id);
	const TeamEquipFBLayerCfg * GetTeamEquipFBLayerCfg(int layer);
	const int GetTeamEquipFbBuyTimesCost(int times);
	int GetBossHpAddPer(int enter_num);

private:
	int InitOtherCfg(PugiXmlNode root_element);
	int InitFbCfg(PugiXmlNode root_element);
	int InitBuyTimesCostCfg(PugiXmlNode root_element);
	int InitBossHpAddPer(PugiXmlNode root_element);

	TeamEquipFBOtherCfg m_other_cfg;

	int m_fb_layer_count;
	TeamEquipFBLayerCfg m_fb_layer_list[MAX_TEAM_EQUIP_FB_LAYER];

	int m_buy_times_cost_count;
	TeamEquipBuyTimesCostCfg m_buy_times_cost_cfg[MAX_BUY_TIME_CFG_COUNT];

	int m_boss_hp_add_per[MAX_TEAM_MEMBER_NUM + 1];
};

#endif // __TEAM_FB_EQUIP_CONFIG_HPP__
