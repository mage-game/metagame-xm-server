#ifndef __BIG_CHATFACE_CONFIG_HPP__
#define __BIG_CHATFACE_CONFIG_HPP__

#include "common/tinyxml/tinyxml.h"	
#include "servercommon/serverdef.h"
#include "servercommon/bigchatfacedef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int BIG_FACE_MAX_LEVEL = 200;
struct BigFaceLevelConfig
{
	BigFaceLevelConfig(): big_face_level(0), big_face_id(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), is_notice(0){}

	short big_face_level;
	short big_face_id;

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

class BigChatFaceConfig : public ConfigBase
{
public:
	BigChatFaceConfig();
	~BigChatFaceConfig();

	bool Init(const std::string &configname, std::string *err);

	const BigFaceLevelConfig * GetBigFaceLevelCfg(short level);
	const int GetBigFaceMaxLevel(){return max_big_face_level;}

protected:

	int InitLevelConfig(PugiXmlNode RootElement);

private:

	int max_big_face_level;
	BigFaceLevelConfig m_big_face_level_cfg[BIG_FACE_MAX_LEVEL + 1];
};

#endif
