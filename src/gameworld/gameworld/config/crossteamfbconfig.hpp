#ifndef __CROSS_TEAM_FB_CONFIG_HPP__
#define __CROSS_TEAM_FB_CONFIG_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/crossdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

static const int MAX_CROSS_TEAM_LAYER = 20;					// 跨服组队副本最大层数

struct CrossTeamFBOtherCfg
{
	CrossTeamFBOtherCfg() : max_count(0) {}

	int max_count;
};

struct CrossTeamFBLayerCfg
{
	CrossTeamFBLayerCfg() : layer(0), level_limit(0), scene_id(0), enter_pos(0, 0), day_exp(0), day_shengwang(0), mojing(0), assist_mojing(0), first_item_count(0), pass_item_count(0) {}

	int layer;
	int level_limit;
	int scene_id;
	Posi enter_pos;
	long long day_exp;
	int day_shengwang;
	int mojing;
	int assist_mojing;

	int first_item_count;
	ItemConfigData first_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int pass_item_count;
	ItemConfigData pass_item_list[MAX_ATTACHMENT_ITEM_NUM];

	std::vector<UInt16> drop_id_vec[CROSS_TEAM_FB_ROLE_MAX];
};

class CrossTeamFBConfig : public ConfigBase
{
public:
	CrossTeamFBConfig();
	~CrossTeamFBConfig();

	bool Init(const std::string &configname, std::string *err);

	const CrossTeamFBOtherCfg & GetOtherCfg();

	int GetFBLayerBySceneID(int scene_id);
	const CrossTeamFBLayerCfg * GetCrossTeamFBLayerCfg(int layer);

private:
	int InitOtherCfg(PugiXmlNode root_element);
	int InitFbCfg(PugiXmlNode root_element);

	CrossTeamFBOtherCfg m_other_cfg;

	int m_fb_layer_count;
	CrossTeamFBLayerCfg m_fb_layer_list[MAX_CROSS_TEAM_LAYER];
};

#endif // __CROSS_TEAM_FB_CONFIG_HPP__
