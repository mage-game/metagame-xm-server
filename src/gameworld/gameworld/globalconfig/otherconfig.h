#ifndef __OTHER_CONFIG_H__
#define __OTHER_CONFIG_H__

#include "servercommon/configbase.h"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <map>

struct OtherOtherConfig
{
	OtherOtherConfig() : cross_relive_gold(0), cross_free_relive_time(0), special_grid_open_level(0), today_free_relive_num(0), zhuizong_item_id(0){}

	int cross_relive_gold;													// 跨服统一买活所需元宝
	int cross_free_relive_time;												// 跨服统一免费复活时间（秒）
	int special_grid_open_level;
	int today_free_relive_num;												// 每天免费复活次数
	int zhuizong_item_id;													// 追踪令id
	int flyshoe_item_id;													// 小飞鞋id
};

struct OtherTeamfbMonsterAttrConfig
{
	OtherTeamfbMonsterAttrConfig() : monster_hp_percent(0), monster_gongji_percent(0), monster_fangyu_percent(0) {}
	
	int monster_hp_percent;
	int monster_gongji_percent;
	int monster_fangyu_percent;
};

class OtherConfig : public ConfigBase
{
public:
	OtherConfig();
	~OtherConfig();

	bool Init(const std::string &path, std::string *err);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitEquipSkillCfg(PugiXmlNode RootElement);
	int InitTeamFBMonsterHpCfg(PugiXmlNode RootElement);

	const OtherOtherConfig & GetOtherCfg() { return m_other_cfg; }
	int GetSkillIDWithItemId(ItemID item_id);

	const OtherTeamfbMonsterAttrConfig* GetTeamFbMonsterAttrCfg(int team_member_num);

private:
	OtherOtherConfig m_other_cfg;
	std::map<ItemID, int> m_item_to_skill_map;

	// 组队副本怪物属性修正 万分比
	OtherTeamfbMonsterAttrConfig m_teamfb_monster_attr_cfg[MAX_TEAM_MEMBER_NUM + 1];
};


#endif // __ROLEEXPCONFIG_H__

