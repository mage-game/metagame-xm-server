#include "monsterinvadeconfig.hpp"
#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/map.h"
#include <string>
#include "scene/worldshadow/worldshadow.hpp"

MonsterInvadeConfig::MonsterInvadeConfig() : m_refresh_interval_s(0), m_monster_count(0)
{
	memset(m_monster_list, 0, sizeof(m_monster_list));
}

MonsterInvadeConfig::~MonsterInvadeConfig()
{

}

bool MonsterInvadeConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "MonsterInvadeConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 
		PugiXmlNode pos_element = RootElement.child("position");
		if (pos_element.empty())
		{
			*err = configname + ": no [position].";
			return false;
		}

		iRet = this->InitPosCfg(pos_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPosCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 
		PugiXmlNode monster_element = RootElement.child("monsterlist");
		if (monster_element.empty())
		{
			*err = configname + ": no [monsterlist].";
			return false;
		}

		iRet = this->InitMonsterCfg(monster_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMonsterCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int MonsterInvadeConfig::InitPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		PosItem positem;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", positem.scene_id))
		{
			return -1;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(positem.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "pos_x", positem.born_pos.x))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", positem.born_pos.y))
		{
			return -3;
		}

		m_pos_vec.push_back(positem);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int MonsterInvadeConfig::InitMonsterCfg(PugiXmlNode RootElement)
{
	m_monster_count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (m_monster_count >= MAX_MONSTER_COUNT)
		{
			break;
		}

		int monster_id = 0;

		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || 
			!MONSTERPOOL->IsMonsterExist(monster_id))
		{
			return -1;
		}

		m_monster_list[m_monster_count++] = monster_id;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int MonsterInvadeConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "refresh_interval_s", m_refresh_interval_s) )
		{
			return -1;
		}
	}

	return 0;
}

void MonsterInvadeConfig::RefreshAll()
{
	for (std::vector<PosItem>::iterator iter = m_pos_vec.begin(); iter != m_pos_vec.end(); ++iter)
	{
		PosItem &pos_item = *iter;

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(pos_item.scene_id, 0);
		if (NULL == scene)
		{
			continue;
		}

		int monster_id = this->RandMonster();
		if (monster_id <= 0)
		{
			continue;
		}

		if (scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos_item.born_pos.x, pos_item.born_pos.y))
		{
			int add_maxhp = 0, add_gongji = 0, add_fangyu = 0;
			
			this->CalcMonsterAddAttrs(monster_id, &add_maxhp, &add_gongji, &add_fangyu);
			MONSTERPOOL->CreateMonster(monster_id, scene, pos_item.born_pos, add_maxhp, add_gongji, add_fangyu);
		}
	}
}

int MonsterInvadeConfig::RandMonster()
{
	if (m_monster_count <= 0)
	{
		return 0;
	}

	int rand_val = RandomNum(m_monster_count);
	if (rand_val >= 0 && rand_val < m_monster_count)
	{
		return m_monster_list[rand_val];
	}

	return 0;
}


void MonsterInvadeConfig::CalcMonsterAddAttrs(int monster_id, int *add_maxhp, int *add_gongji, int *add_fangyu)
{
	if (monster_id <= 0 || NULL == add_maxhp || NULL == add_fangyu || NULL == add_gongji) return;

	*add_maxhp = 0; *add_gongji = 0; *add_fangyu = 0;
}

