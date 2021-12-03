#ifndef __SCENEMONSTERLOGIC_H__
#define __SCENEMONSTERLOGIC_H__

#include <vector>
#include <map>

class Scene;
class MonsterGenerater;
class GatherGenerater;

class SceneMonsterLogic
{
public:
	typedef std::vector<MonsterGenerater *> MonsterGeneraterList;
	typedef std::vector<MonsterGenerater *>::iterator MonsterGeneraterListIt;

	typedef std::vector<GatherGenerater *> GatherGeneraterList;
	typedef std::vector<GatherGenerater *>::iterator GatherGeneraterListIt;

	SceneMonsterLogic();
	~SceneMonsterLogic();

	bool Init(Scene *scene, const ConfigSceneMonsterPointList &monster_point, const ConfigSceneGatherList &gather_point);
	void Release();

	void Update(unsigned long interval);

	void Reset();	

	void AddMonsterGenerate(Scene *scene, int histroytotalnum, int num, int interval, Posi pos, int monster_id);
	MonsterGeneraterList & GetMonsterGenList() { return m_monster_gen_list; }

	GatherGeneraterList & GetGatherGeneraterList() { return m_gather_gen_list; }

protected:
	MonsterGeneraterList m_monster_gen_list;
	GatherGeneraterList m_gather_gen_list;
};

#endif
