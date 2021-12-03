#ifndef __MONSTERE_EXTRA_DROP_CONFIG_HPP__
#define __MONSTERE_EXTRA_DROP_CONFIG_HPP__

#include <map>
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

struct MonsterExtraDropCfg
{
	MonsterExtraDropCfg() : kill_count(0) {}

	int kill_count;
	std::vector<UInt16> drop_id_list;
};

class MonsterExtraDropConfig : public ConfigBase
{
public:
	MonsterExtraDropConfig();
	~MonsterExtraDropConfig();

	bool Init(const std::string &path, std::string *err);

	const MonsterExtraDropCfg *GetMonsterDropCfg(unsigned short monster_id, int kill_count);

private:
	int InitMonsterExtraDropConfig(PugiXmlNode RootElement);

	std::map<unsigned short, std::vector<MonsterExtraDropCfg> > m_monster_drop_map;
};

#endif
