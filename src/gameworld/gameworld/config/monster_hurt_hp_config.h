#ifndef __MONSTER_HURT_MAX_HP_
#define __MONSTER_HURT_MAX_HP_

#include <map>
#include "gamedef.h"
#include "servercommon/configbase.h"
#include "servercommon/configcommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"


struct HurtConfig
{
	HurtConfig() 
	{
		monster_id = 0;
		percent = 0;
	}
	UInt16 monster_id;
	unsigned int percent;
};

struct LevelDeltaCfg
{

	int level_delta = 0; 
	int add_hurt_per = 0;
	int dec_attack_per = 0;
};

class MonsterHurtHpConfig : public ConfigBase
{
public:
	MonsterHurtHpConfig() {};
	~MonsterHurtHpConfig() {};

	bool Init(const std::string &path, std::string *err);
	int InitHurtCfg(PugiXmlNode RootElement);
	int InitLevelDeltaCfg(PugiXmlNode RootElement);
	int InitIgnoreLvDeltaCfg(PugiXmlNode RootElement);

	HurtConfig* GetConfig(UInt16 monster_id);
	LevelDeltaCfg *GetLevelDeltaCfg(int level_delta);
	bool IsIgnoreLevelDelta(int scene_type);

private:
	std::map<UInt16, HurtConfig> m_hurt_cfg_map;
	typedef std::map<UInt16, HurtConfig>::iterator hurt_cfg_iter;

	std::map<int, LevelDeltaCfg> m_level_delta_map;

	std::vector<int> m_ignore_lv_delta_vec;
};



#endif