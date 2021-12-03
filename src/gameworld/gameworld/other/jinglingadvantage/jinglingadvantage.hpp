#ifndef __JINGLING_ADVANTAGE_HPP__
#define __JINGLING_ADVANTAGE_HPP__

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"

#include <map>
#include <vector>
#include <deque>

class Role;
class Scene;

class JinglingAdvantage
{
public:
	struct GatherScene
	{
		struct EggPos 
		{
			EggPos() : exist(0), pos(0, 0){}

			int exist;
			Posi pos;
		};

		GatherScene() : scene_id(0), purple_count(0), blue_count(0) {}

		int scene_id;							// 场景id
		short purple_count;						// 紫色精灵数量
		short blue_count;						// 蓝色精灵数量

		//std::vector<Posi> valid_pos_vec;
		std::vector<EggPos> egg_pos_vec;
	};

	static JinglingAdvantage & Instance();

	void OnServerStart();
	void OnAdvantageReq(Role *role);

	bool CanGather(Role *role, int gather_id);
	void Update(unsigned int now_second);
	void OnGather(Role *role ,int gather_id, const Posi &gather_pos);
	void OnSkipGather(Role *role);
	void OnSkipGather(Role *role, int skip_times);

	void SynJinglingCountToRole(Role *role);

	void OnEnterBossScene(Role *role, int monster_id);
	void OnGetEggPosBySceneId(Role *role, int scene_id);

private:
	JinglingAdvantage();
	~JinglingAdvantage();

	bool RefreshOneGather(Scene *scene, GatherScene &info, int gather_type);
	void SyncGatherSceneInfo(Role *role = NULL);

	void CheckRefreshBoss(Role *role, const Posi &gather_pos);

	struct BossSceneCfg
	{
		BossSceneCfg() : scene_id(0), scene_key(0), pos(0, 0), monster_id(0),invalid_time(0)
		{}

		short scene_id;								//场景id
		short scene_key;							//场景路线
		Posi pos;
		int monster_id;								//怪物id
		unsigned int invalid_time;					//失效时间
	};

	static const int BOSS_SCENE_CFG_VALID_TIME_S = 15;

	std::map<int, GatherScene> m_gather_scene_map;
	std::deque<BossSceneCfg> m_boss_scene_deque;
};

#endif // __JINGLING_ADVANTAGE_HPP__
