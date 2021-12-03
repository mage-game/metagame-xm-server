#include "monsterextradropconfig.hpp"
#include "item/itempool.h"
#include <set>
#include "item/itembase.h"
#include "monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "monster/monsterinitparam.h"
#include "monster/drop/droppool.hpp"

MonsterExtraDropConfig::MonsterExtraDropConfig()
{

}

MonsterExtraDropConfig::~MonsterExtraDropConfig()
{

}

bool MonsterExtraDropConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	PUGI_XML_LOAD_CONFIG("extra_drop", InitMonsterExtraDropConfig);

	return true;
}

int MonsterExtraDropConfig::InitMonsterExtraDropConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}


	unsigned short last_monster_id = -1;
	unsigned short monster_id = 0;
	int last_kill_count = -1;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || !MONSTERPOOL->IsMonsterExist(monster_id))
		{
			return -1;
		}

		if (last_monster_id != monster_id)
		{
			if (m_monster_drop_map.find(monster_id) != m_monster_drop_map.end())
			{
				return -2;
			}
			last_kill_count = -1;
			m_monster_drop_map[monster_id] = std::vector<MonsterExtraDropCfg>();
		}
		last_monster_id = monster_id;
		MonsterExtraDropCfg cfg;
		if (!PugiGetSubNodeValue(dataElement, "kill_count", cfg.kill_count) || cfg.kill_count < 0)
		{
			return -3;
		}
		if (last_kill_count >= cfg.kill_count)
		{
			return -4;
		}
		last_kill_count = cfg.kill_count;
		PugiXmlNode dropListElement = dataElement.child("dropid_list");
		if (dropListElement.empty())
		{
			return -5;
		}
		PugiXmlNode dropidElement = dropListElement.child("dropid");
		if (dropidElement.empty())
		{
			return -6;
		}
		while (!dropidElement.empty())
		{
			UInt16 dropid = 0;
			if (!PugiGetNodeValue(dropidElement, dropid))
			{
				return -7;
			}
			if (0 != dropid) 
			{
				if (NULL == DROPPOOL->GetDropConfig(dropid))
				{
					return -8;
				}
				cfg.drop_id_list.push_back(dropid);
				if (cfg.drop_id_list.size() >= (size_t)MonsterInitParam::DROP_LIST_MAX_COUNT)
				{
					return -9;
				}
			}
			dropidElement = dropidElement.next_sibling();
		}
		m_monster_drop_map[monster_id].push_back(cfg);

		dataElement = dataElement.next_sibling();

	}

	return 0;
}

const MonsterExtraDropCfg *MonsterExtraDropConfig::GetMonsterDropCfg(unsigned short monster_id, int kill_count)
{
	if (m_monster_drop_map.find(monster_id) == m_monster_drop_map.end())
	{
		return NULL;
	}
	const std::vector<MonsterExtraDropCfg> &vec = m_monster_drop_map[monster_id];
	const size_t size = vec.size();
	for (size_t i = size - 1; i >= 0 && i < size; --i)
	{
		if (kill_count >= vec[i].kill_count)
		{
			return &m_monster_drop_map[monster_id][i];
		}
	}
	return NULL;
}
