#ifndef __TUHAOJIN_CONFIG_H__
#define __TUHAOJIN_CONFIG_H__

#include "obj/character/attribute.hpp"
#include "servercommon/tuhaojindef.hpp"
#include "item/itembase.h"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int TUHAOJIN_MAX_LEVEL = 200;
struct TuhaojinLevelConfig
{
	TuhaojinLevelConfig(): tuhaojin_level(0), tuhaojin_color_id(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), is_notice(0){}

	short tuhaojin_level;
	short tuhaojin_color_id;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int is_notice;

	ItemConfigData common_item;
	ItemConfigData prof_one_item;
	ItemConfigData prof_two_item;
	ItemConfigData prof_three_item;
	ItemConfigData prof_four_item;
};


class TuHaoJinConfig : public ConfigBase
{
public:
	TuHaoJinConfig();
	~TuHaoJinConfig();

	bool Init(const std::string &configname, std::string *err);

	const TuhaojinLevelConfig * GetTuhaojinLevelCfg(short level);
	const int GetTuhaojinMaxLevel(){return max_tuhaojin_level;}

private:

	int InitLevelConfig(PugiXmlNode RootElement);

	int max_tuhaojin_level;
	TuhaojinLevelConfig m_tuhaojin_level_cfg[TUHAOJIN_MAX_LEVEL + 1];
};

#endif

