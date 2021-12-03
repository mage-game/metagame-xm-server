#ifndef __XINGZUOYIJI_HPP__
#define __XINGZUOYIJI_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>
#include <map>
#include <utility>

struct XingzuoYijiOtherCfg
{
	XingzuoYijiOtherCfg() : scene_id(0), relive_pos(0, 0), refresh_time(0), common_box_gather_limit(0), great_box_gather_limit(0), common_box_refresh_time(0), boss_disappear_time(0), boss_max_refresh_times(0)
	{}

	int scene_id;
	Posi relive_pos;
	unsigned int refresh_time;
	int common_box_gather_limit;
	int great_box_gather_limit;
	unsigned int common_box_refresh_time;
	unsigned int boss_disappear_time;
	int boss_max_refresh_times;
};

struct XingzuoYijiBossCfg
{
	XingzuoYijiBossCfg() : world_level_min(0), world_level_max(0), monster_id(0), boss_pos(0, 0)
	{}

	short world_level_min;
	short world_level_max;
	int monster_id;
	int monster_id_second;
	int monster_id_third;
	Posi boss_pos;
};

struct XingzuoYijiBoxTypeCfg
{
	XingzuoYijiBoxTypeCfg() : gather_id(0), gather_time_s(0), gather_num(0), gather_limit_time(0)
	{}

	int gather_id;
	unsigned int gather_time_s;
	int gather_num;
	unsigned int gather_limit_time;
};

struct XingzuoYijiBoxItemCfg
{
	XingzuoYijiBoxItemCfg(): world_level_min(0), world_level_max(0)
	{}

	short world_level_min;
	short world_level_max;

	static const int MAX_BOX_TYPE = 4;

	ItemConfigData item[MAX_BOX_TYPE];
};

struct XingzuoYijiSkipCfg
{
	XingzuoYijiSkipCfg() : limit_level(0), consume(0)
	{}

	int limit_level;
	int consume;
};

class XingzuoYiJiConfig : public ConfigBase
{
public:
	XingzuoYiJiConfig();
	~XingzuoYiJiConfig();

	bool Init(const std::string &configname, std::string *er);

	const XingzuoYijiOtherCfg* XingzuoYijiGetOtherCfg(){return &m_other_cfg;}
	const ItemConfigData* XingzuoYijiGetBoxItem(int world_level, int box_type);
	const XingzuoYijiBossCfg* XingzuoYijiGetBossCfg(int world_level);
	const XingzuoYijiSkipCfg* XingzuoYijiGetSkipCfg(int quality);
	const int XingzuoYijiGetCommonBoxCfgCount(){return MEDIUM_BOX - SENIOR_BOX + 1;}
	const int XingzuoYijiGetGreatBoxCfgCount(){return MOLONG_BOX - JUNIOR_BOX + 1;}
	const XingzuoYijiBoxTypeCfg* XingzuoYijiGetBoxTypeCfg(int index);
	const int XingzuoYijiGetBoxType(int gather_id);
	std::vector<Posi>& XingzuoYijiGetRandomPos();

	enum BOXTYPE
	{
		SENIOR_BOX = 0,
		MEDIUM_BOX,
		JUNIOR_BOX,
		MOLONG_BOX,

		MAX_GRADE_BOX,
	};

private:
	int InitXingzuoYijiOtherCfg(PugiXmlNode RootElement);
	int InitXingzuoYijiBossCfg(PugiXmlNode RootElement);
	int InitXingzuoYijiBoxTypeCfg(PugiXmlNode RootElemen);
	int InitXingzuoYijiBoxItemCfg(PugiXmlNode RootElemen);
	int InitXingzuoYijiBoxRandomPosCfg(PugiXmlNode RootElemen);
	int InitXingzuoYijiSkipCfg(PugiXmlNode RootElemen);

	XingzuoYijiOtherCfg m_other_cfg;						
	std::vector<XingzuoYijiBossCfg> m_boss_cfg_vec;							// BOSS列表
	XingzuoYijiBoxTypeCfg m_box_type_cfg[MAX_GRADE_BOX];					// 宝箱列表
	std::vector<XingzuoYijiBoxItemCfg> m_box_item_cfg_vec;					// 物品列表

	std::vector<Posi> m_box_pos;											// 宝箱随机坐标

	XingzuoYijiSkipCfg m_skip_cfg[MEDIUM_BOX - SENIOR_BOX + 1];
};

#endif // __XINGZUOYIJI_HPP__
