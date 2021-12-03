#ifndef __XINMOFB_CONFIG_HPP__
#define __XINMOFB_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int XINMOFB_MONSTER_MAX_NUM = 3;
static const int XINMOFB_LAYER_TYPE_MAX = 4;

struct XinMoFbOtherConfig
{
	XinMoFbOtherConfig() : kick_out_time(0) {}

	int kick_out_time;
};

struct XinMoFbLayerConfig
{
	XinMoFbLayerConfig() : layer(0), role_level(0), scene_id(0), enter_posi(0, 0), fb_time(0) {}

	int layer;
	int role_level;
	int scene_id;
	Posi enter_posi;
	int fb_time;
};

struct XinMoFbMonsterConfig
{

	struct MonsterConfig
	{
		MonsterConfig() : seq(0), monster_id(0), bron_pos(0, 0) {}

		int seq;
		int monster_id;
		Posi bron_pos;
	};


	XinMoFbMonsterConfig() : monster_count(0) {}

	int monster_count;

	MonsterConfig monster_cfg_list[XINMOFB_MONSTER_MAX_NUM];
};

struct XinMoFbPassSkillConfig
{
	XinMoFbPassSkillConfig() : skill_seq(0), cd_time_s(0), percent_val(0), effect_val(0), skill_index(0){}

	int skill_seq;
	int cd_time_s;
	int percent_val;
	int effect_val;
	int skill_index;
};

class XinMoFbConfig : public ConfigBase
{
public:
	XinMoFbConfig();
	~XinMoFbConfig();

	bool Init(const std::string &configname, std::string *err);

	const XinMoFbOtherConfig &GetOtherCfg() { return m_other_cfg; }
	const XinMoFbLayerConfig * GetLayerCfg(int layer);
	const XinMoFbLayerConfig * GetLayerCfgBySceneId(int scene_id);
	const XinMoFbMonsterConfig::MonsterConfig * GetMonsterCfg(int layer, int seq);
	const XinMoFbPassSkillConfig * GetPassSkillCfg(int skill_seq);

	int GetMonsterCount(int layer);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitLayerCfg(PugiXmlNode RootElement);
	int InitMonsterCfg(PugiXmlNode RootElement);
	int InitPassSkillCfg(PugiXmlNode RootElement);

	XinMoFbOtherConfig m_other_cfg;

	XinMoFbLayerConfig m_layer_cfg_list[XINMOFB_LAYER_TYPE_MAX];

	XinMoFbMonsterConfig m_monster_cfg_list[XINMOFB_LAYER_TYPE_MAX];

	XinMoFbPassSkillConfig m_pass_skill_cfg_list[XINMOFB_LAYER_TYPE_MAX];
};

#endif // __XINMOFB_CONFIG_HPP__

