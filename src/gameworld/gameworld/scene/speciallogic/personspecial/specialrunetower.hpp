#ifndef __SPECIAL_RUNETOWER_HPP__
#define __SPECIAL_RUNETOWER_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "other/runesystem/runetowerconfig.hpp"

struct RuneTowerLayerCfg;

class SpecialRuneTower : public SpecialLogic
{
public:
	SpecialRuneTower(Scene *scene);
	virtual ~SpecialRuneTower();

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnLeaveFB(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool OnTimeout();
	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool CanDestroy() { return m_is_finish; }

	void OnReqNextLevel();
	void OnReqRefreshMonster();

protected:
	Role * GetOwnerRole();

	void OnFinish(bool is_pass);
	void RefreshMonster();
	void SyncFBSceneLogicInfo();
	void SyncFBPassRewardInfo(int reward_count, int reward_list[RUNE_TOWER_RAND_REWARD_MAX_COUNT]);

	ObjID m_owner_objid; 
	UserID m_owner_user_id;

	bool m_is_finish;
	bool m_is_pass;
	bool m_is_first;

	int m_fb_layer;
	int m_total_monster_count;
	int m_kill_monster_count;

	unsigned int m_die_timestamp;

	bool m_refresh_monster;
	unsigned int m_fresh_monster_timestamp;
};

#endif // __SPECIAL_RUNETOWER_HPP__


