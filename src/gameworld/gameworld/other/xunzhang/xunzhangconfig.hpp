#ifndef __XUNZHANG_CONFIG_HPP__
#define __XUNZHANG_CONFIG_HPP__

#include <string>
#include "gamedef.h"
#include "servercommon/basedef.h"
#include "servercommon/struct/xunzhangparam.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "obj/character/attribute.hpp"


static const int MAX_XUNZHANG_SUIT_CFG_COUNT = 10;

struct XunZhangLevelCfg
{
	XunZhangLevelCfg() : xunzhang_id(0), level(0), uplevel_stuff_id(0), uplevel_stuff_num(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), 
		per_jingzhun(0), per_baoji(0), per_pofang(0), per_mianshang(0), per_boss_hurt(0), per_monster_exp(0), per_monster_hurt(0) {}

	int xunzhang_id;
	int level;
	ItemID uplevel_stuff_id;
	short uplevel_stuff_num;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute per_jingzhun;
	Attribute per_baoji;
	Attribute per_pofang;
	Attribute per_mianshang;
	int per_boss_hurt;
	int per_monster_exp;
	int per_monster_hurt;
};

struct XunZhangSuitCfg
{
	XunZhangSuitCfg() : total_level(0), mount_attr_add(0), wing_attr_add(0), halo_attr_add(0), magic_bow_attr_add(0), magic_wing_attr_add(0), footprint_attr_add(0){}

	int total_level;
	int mount_attr_add;
	int wing_attr_add;
	int halo_attr_add;
	int magic_bow_attr_add;
	int magic_wing_attr_add;
	int footprint_attr_add;
};

class XunZhangConfig : public ConfigBase
{
public:
	XunZhangConfig();
	~XunZhangConfig();

	bool Init(std::string path, std::string *err);

	XunZhangLevelCfg * GetXunZhangLevelCfg(int xunzhang_id, int level);
	XunZhangSuitCfg * GetXunZhangSuitCfg(int total_level);

private:
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitSuitCfg(PugiXmlNode RootElement);

	XunZhangLevelCfg m_level_cfg_list[MAX_XUNZHANG_COUNT][MAX_XUNZHANG_LEVEL + 1];

	int m_suit_cfg_count;
	XunZhangSuitCfg m_suit_cfg_list[MAX_XUNZHANG_SUIT_CFG_COUNT];
};

#endif // __XUNZHANG_CONFIG_HPP__
