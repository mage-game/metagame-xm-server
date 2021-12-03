#ifndef __PERSONALIZE_WINDOW_CONFIG_H__
#define __PERSONALIZE_WINDOW_CONFIG_H__

#include "obj/character/attribute.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/personalizewindowdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <set>

static const int BUBBLE_WINDOW_MAX_LEVEL = 100;
struct BubbleLevelConfig
{
	BubbleLevelConfig():bubble_type(0), bubble_level(0), is_active(0), is_notice(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0){}

	short bubble_type;
	short bubble_level;
	short is_active;
	short is_notice;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;

	ItemConfigData common_item;
	ItemConfigData prof_one_item;
	ItemConfigData prof_two_item;
	ItemConfigData prof_three_item;
	ItemConfigData prof_four_item;
};

static const int AVATAR_WINDOW_MAX_LEVEL = 100;
struct AvatarLevelConfig
{
	AvatarLevelConfig() : avatar_type(0), avatar_level(0), is_active(0), is_notice(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	short avatar_type;
	short avatar_level;
	short is_active;
	short is_notice;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;

	ItemConfigData common_item;
};

class PersonalizeWindowConfig : public ConfigBase
{
public: 
	PersonalizeWindowConfig();
	~PersonalizeWindowConfig();

	bool Init(const std::string &configname, std::string *err);
	const BubbleLevelConfig * GetBubbleWindowLevelCfg(short bubble_type, short bubble_level);
	const int GetTuhaojinMaxLevel(short bubble_type);

	const AvatarLevelConfig * GetAvatarWindowLevelCfg(short avatar_type, short avatar_level);
	const int GetAvatarMaxLevel(short avatar_type);

private:

	int InitLevelConfig(PugiXmlNode RootElement);
	int InitAvatarLevelConfig(PugiXmlNode RootElement);

	int max_bubble_level[BUBBLE_WINDOW_MAX_TYPE];
	BubbleLevelConfig m_bubble_level_cfg[BUBBLE_WINDOW_MAX_TYPE][BUBBLE_WINDOW_MAX_LEVEL + 1];

	int max_avatar_level[AVATAR_WINDOW_MAX_TYPE];
	AvatarLevelConfig m_avatar_level_cfg[AVATAR_WINDOW_MAX_TYPE][AVATAR_WINDOW_MAX_LEVEL + 1];
};

#endif