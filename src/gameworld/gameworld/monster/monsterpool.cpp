#include "monsterpool.h"
#include "obj/character/monster.h"
#include "scene/scene.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"

#include "servercommon/configcommon.h"
#include "monster/drop/droppool.hpp"
#include "item/itempool.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
MonsterPool *gs_monster_pool_config = NULL;

MonsterPool * MonsterPool::Instance()
{
	if (NULL == gs_monster_pool_config)
	{
		gs_monster_pool_config = new MonsterPool();
	}

	return gs_monster_pool_config;
}

MonsterPool::MonsterPool()
	: m_max_aoi_range(0)
{
	memset(m_monster_pool, 0, sizeof(m_monster_pool));
}

MonsterPool::~MonsterPool()
{
	Release();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

void MonsterPool::Release()
{
	for (int i = 0; i < MAX_MONSTER_ID; ++i)
	{
		if (m_monster_pool[i] != 0)
		{
			delete m_monster_pool[i];
		}
	}
	memset(m_monster_pool, 0, sizeof(m_monster_pool));
}

bool MonsterPool::IsMonsterExist(UInt16 monster_id)
{
	MonsterInitParam *mp = m_monster_pool[monster_id];
	return (NULL != mp);
}

Monster * MonsterPool::CreateMonster(UInt16 monster_id, Scene *scene, const Posi &pos, long long add_maxhp, long long add_gongji, long long add_fangyu, int set_level, int special_param)
{
	MonsterInitParam *mp = m_monster_pool[monster_id];
	if (NULL == mp) return NULL;

	{
		mp->hp += add_maxhp;
		mp->gongji += add_gongji;
		mp->fangyu += add_fangyu;
	}
	
	if (set_level > 0)
	{
		int old_level = mp->level;
		mp->level = set_level;
		set_level = old_level;
	}

	Monster *monster = new Monster(mp->monster_name);
	monster->SetPos(pos);
	monster->Init(*mp, scene->GetMonsterUniqueKey());
	monster->SetSpecialParam(special_param);
	scene->AddObj(monster);

	{
		mp->hp -= add_maxhp;
		mp->gongji -= add_gongji;
		mp->fangyu -= add_fangyu;
	}

	if (set_level > 0)
	{
		mp->level = set_level;
	}

	// modify 2019-03-27  策划需求去除世界BOSS的单个BOSS刷新传闻
	//if (mp->flush_notice && monster->IsWorldBossMonster())
	//{
	//	int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_refresh_boss, monster->GetName(), scene->GetSceneName());
	//	if (len > 0) 
	//	{
	//		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE);
	//	}
	//}

	return monster;
}

Monster * MonsterPool::CreateMonster(UInt16 monster_id, Scene *scene, const Posi &pos, Dir dir, Scalar dis)
{
	MonsterInitParam *mp = m_monster_pool[monster_id];
	if (NULL == mp) return NULL;

	Monster *monster = new Monster(mp->monster_name);
	monster->SetPos(pos);
	monster->Init(*mp, scene->GetMonsterUniqueKey());
	monster->SetMonsterInitDir(dir, dis);
	scene->AddObj(monster);

	// modify 2019-03-28  策划需求去除单个BOSS刷新传闻
	//if (mp->flush_notice && monster->IsWorldBossMonster())
	//{
	//	int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_refresh_boss, monster->GetName(), scene->GetSceneName());
	//	if (len > 0) 
	//	{
	//		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE);
	//	}
	//}

	return monster;
}

const MonsterInitParam * MonsterPool::GetMonsterParam(int monster_id)
{
	if (monster_id < 0 || monster_id >= MAX_MONSTER_ID)
	{
		return NULL;
	}

	return m_monster_pool[monster_id];
}

bool MonsterPool::Init(const std::string &dir, const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};

	TiXmlDocument document;
	if (configname == "" || !document.LoadFile(dir+configname))
	{
		sprintf(errinfo,"%s: Load MonsterManager Config Error, %s.", (dir+configname).c_str(), document.ErrorDesc());
		*err = errinfo;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	TiXmlElement *currentElement = NULL;

	if (!RootElement)
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	TiXmlElement *listElement = RootElement->FirstChildElement("monster_list");
	if (NULL == listElement)
	{
		*err = configname + ": xml node error in getting node [monster_list]. ";
		return false;
	}

	currentElement = listElement->FirstChildElement("path");
	if (NULL == currentElement)
	{
		*err = configname + ": xml node error in getting node [path]. ";
		return false;
	}

	while (NULL != currentElement)
	{
		MonsterInitParam *monster_param = new MonsterInitParam();

		std::string path;

		if (!GetNodeValue(currentElement,path))
		{
			*err = configname + ": xml node error in node [monstermanager->path] ";
			delete monster_param;
			this->Release();
			return false;
		}

		if (!monster_param->Init((dir+path), *err))
		{
			delete monster_param;
			this->Release();
			return false;
		}

		if (monster_param->monster_id <= 0)
		{
			sprintf(errinfo,"load path: %s error: monster id[%d] is not allowed.", path.c_str(),monster_param->monster_id);
			*err = errinfo;

			delete monster_param;
			this->Release();
			return false;
		}

		if (m_monster_pool[monster_param->monster_id] != 0)
		{
			sprintf(errinfo,"load path: %s error: monster id[%d] repeat", path.c_str(),monster_param->monster_id);
			*err = errinfo;

			delete monster_param;
			this->Release();
			return false;
		}

		if ((int)monster_param->aoi_range > m_max_aoi_range) m_max_aoi_range = monster_param->aoi_range;

		m_monster_pool[monster_param->monster_id] = monster_param;

		currentElement = currentElement->NextSiblingElement();
	}

	return true;
}

bool MonsterPool::Reload(const std::string &dir, const std::string &configname, std::string *err)
{
	MonsterPool *temp = new MonsterPool();

	if (!temp->Init(dir, configname, err))
	{
		delete temp; temp = NULL;
		return false;
	}

	if (NULL != gs_monster_pool_config) delete gs_monster_pool_config;

	gs_monster_pool_config = temp;

	return true;
}

// 怪物掉落，通常是满足通关条件（一般是怪物死亡，可能是有其他）执行
void MonsterPool::GetMonsterDrop(int monster_id, ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT], 
								 int *index_count, long long *reward_exp, int *reward_coin, int *reward_yuanli)
{
	if (0 == monster_id) return;

	const MonsterInitParam *monster_param = this->GetMonsterParam(monster_id);
	if (NULL == monster_param)
	{
		return;
	}

	if (NULL != reward_exp) *reward_exp += monster_param->exp_reward;
	if (NULL != reward_yuanli) *reward_yuanli += monster_param->yuanli_reward;

	if (NULL != reward_coin)
	{
		int rand_value = RandomNum(100);
		if (rand_value < monster_param->drop_money_prob)
		{
			*reward_coin += monster_param->drop_money_count;
		}
	}

	if (NULL != index_count && NULL != reward_item_list)
	{
		for (std::vector<UInt16>::const_iterator it = monster_param->dropid_list.begin(); it != monster_param->dropid_list.end(); ++it)
		{
			static ItemConfigData drop_item_list[DropConfig::DROP_ITEM_PROB_MAX];
			memset(drop_item_list, 0, sizeof(drop_item_list));

			int count = DROPPOOL->GetDropItemList(*it, drop_item_list);

			for (int i = 0; i < count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
			{
				const ItemBase *item = ITEMPOOL->GetItem(drop_item_list[i].item_id);
				if (NULL == item) continue;

				ItemExtern::PutInItemConfigList(MonsterInitParam::MAX_DROP_ITEM_COUNT, reward_item_list, index_count, drop_item_list[i].item_id,
					drop_item_list[i].num, drop_item_list[i].is_bind);
			}
		}
	}
}


