#ifndef __MOJIE_CONFIG_HPP__
#define __MOJIE_CONFIG_HPP__

#include <string>
#include "gamedef.h"
#include "servercommon/basedef.h"
#include "obj/character/attribute.hpp"
#include "servercommon/struct/mojieparam.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct MojieOtherCfg
{
	MojieOtherCfg() : reserve(0){}

	unsigned int reserve;
};

enum MOJIE_TYPE
{
	INVALID_MOJIE_TYPE = -1,

	MOJIE_TYPE_1 = 0,
	MOJIE_TYPE_2,
	MOJIE_TYPE_3,
	MOJIE_TYPE_4,

	MOJIE_TYPE_MAX,
};

// 两个必须是相等的，存库已经定死是MOJIE_MAX_TYPE
UNSTD_STATIC_CHECK(MojieParam::MOJIE_MAX_TYPE == MOJIE_TYPE_MAX);

struct MojieUplevelCfg
{
	MojieUplevelCfg():mojie_type(0), mojie_level(0), has_skill(0), use_global_cold_time(0), is_broadcast(0), skill_id(0), skill_level(0), up_level_stuff_id(0), up_level_stuff_num(0), up_level_limit(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), per_pofang(0), per_baoji(0), per_baoji_hurt(0), pvp_jianshang_per(0), per_mianshang(0) {}

	int mojie_type;
	int mojie_level;

	short has_skill;
	short use_global_cold_time;
	short is_broadcast;

	int skill_id;
	int skill_level;
	int up_level_stuff_id;
	short up_level_stuff_num;
	int up_level_limit;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute per_pofang;
	Attribute per_baoji;
	Attribute per_baoji_hurt;
	Attribute pvp_jianshang_per;
	Attribute per_mianshang;
};

class MojieConfig : public ConfigBase
{
public:
	MojieConfig();
	~MojieConfig();

	bool Init(std::string path, std::string *err);

	const MojieOtherCfg & GetMojieOtherCfg(){return m_mojie_other_cfg;}
	const MojieUplevelCfg * GetMojieLevelCfg(int mojie_type, int mojie_level) const;

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);

	MojieOtherCfg m_mojie_other_cfg;
	MojieUplevelCfg m_mojie_level_cfg[MojieParam::MOJIE_MAX_TYPE][MojieParam::MOJIE_MAX_LEVEL+1];	// 魔戒等级配置
	int m_mojie_max_level[MojieParam::MOJIE_MAX_TYPE];									// 记录魔戒配置表配置的最大等级
};

#endif // __MOJIE_CONFIG_HPP__
