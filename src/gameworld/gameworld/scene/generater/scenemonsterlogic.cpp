#include "config/sceneconfig.h"

#include "scenemonsterlogic.h"
#include "monstergenerater.h"
#include "gathergenerater.h"

SceneMonsterLogic::SceneMonsterLogic()
{
	
}

SceneMonsterLogic::~SceneMonsterLogic()
{
	
}

bool SceneMonsterLogic::Init(Scene *scene, const ConfigSceneMonsterPointList &monster_point, const ConfigSceneGatherList &gather_point)
{
	this->Release();

	// ¹ÖÎï
	for (ConfigSceneMonsterPointList::const_iterator iter = monster_point.begin(); iter != monster_point.end(); ++iter)
	{
		MonsterGenerater *generater = new MonsterGenerater(scene);
		if (!generater->Init(iter->histroytotalnum, iter->num, iter->interval, Posi(iter->x, iter->y), iter->monsterid))
		{
			delete generater;
			Release();
			return false;
		}
		m_monster_gen_list.push_back(generater);
	}

	// ²É¼¯
	for(ConfigSceneGatherList::const_iterator iter = gather_point.begin(); iter != gather_point.end(); ++iter)
	{
		GatherGenerater *gather_gen = new GatherGenerater(scene);
		if (!gather_gen->Init(iter->create_interval, Posi(iter->x, iter->y), iter->id, (int)iter->gather_time, iter->evil_add, iter->disappear_after_gather, iter->gather_index))
		{
			delete gather_gen;
			Release();
			return false;
		}
		m_gather_gen_list.push_back(gather_gen);
	}

	return true;
}

void SceneMonsterLogic::Release()
{
	for (MonsterGeneraterList::iterator i = m_monster_gen_list.begin(); i != m_monster_gen_list.end(); ++i)
	{
		delete (*i);
	}

	for (GatherGeneraterList::iterator i = m_gather_gen_list.begin(); i != m_gather_gen_list.end(); ++i)
	{
		delete (*i);
	}

	m_monster_gen_list.clear();
	m_gather_gen_list.clear();
}

void SceneMonsterLogic::Update(unsigned long interval)
{
	for (MonsterGeneraterList::iterator i = m_monster_gen_list.begin(); i != m_monster_gen_list.end(); ++i)
	{
		(*i)->Update(interval);
	}

	for (GatherGeneraterList::iterator i = m_gather_gen_list.begin(); i != m_gather_gen_list.end(); ++i)
	{
		(*i)->Update(interval);
	}
}

void SceneMonsterLogic::Reset()
{
	for (MonsterGeneraterList::iterator i = m_monster_gen_list.begin(); i != m_monster_gen_list.end(); ++i)
	{
		(*i)->Reset();
	}
}

void SceneMonsterLogic::AddMonsterGenerate(Scene *scene, int histroytotalnum, int num, int interval, Posi pos, int monster_id)
{
	MonsterGenerater *generater = new MonsterGenerater(scene);

	if (!generater->Init(histroytotalnum, num, interval, pos, monster_id))
	{
		delete generater;
		Release();
		return;
	}

	m_monster_gen_list.push_back(generater);
}


