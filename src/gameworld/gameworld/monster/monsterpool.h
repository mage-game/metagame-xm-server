#ifndef __MONSTERPOOL_H__
#define __MONSTERPOOL_H__

#include <limits.h>

#include "servercommon/basedef.h"
#include "servercommon/struct/itemlistparam.h"
#include "monsterinitparam.h"

#include "gamedef.h"
#include <string>

class MonsterInitParam;
class Monster;
class Scene;

#define MONSTERPOOL MonsterPool::Instance()

class MonsterPool
{
public:
	static const int MAX_MONSTER_ID = (int)USHRT_MAX + 1;

	static MonsterPool * Instance();

	bool Init(const std::string &dir, const std::string &configname, std::string *err);
	static bool Reload(const std::string &dir, const std::string &configname, std::string *err);

	Monster * CreateMonster(UInt16 monster_id, Scene *scene, const Posi &pos, long long add_maxhp = 0, long long add_gongji = 0, long long add_fangyu = 0, int set_level = 0, int special_param = 0);
	Monster * CreateMonster(UInt16 monster_id, Scene *scene, const Posi &pos, Dir dir, Scalar dis);

	bool IsMonsterExist(UInt16 monster_id);
	const MonsterInitParam * GetMonsterParam(int monster_id);
	int GetMonsterMaxAoiRange() { return m_max_aoi_range; }

	void GetMonsterDrop(int monster_id, ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT], 
						int *index_count, long long *reward_exp, int *reward_coin, int *reward_yuanli);
	
private:
	void Release();

	MonsterPool();
	~MonsterPool();

	MonsterInitParam* m_monster_pool[MAX_MONSTER_ID];

	int m_max_aoi_range;
};

#endif

