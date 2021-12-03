#ifndef __FOLLOW_BOSS_H__
#define __FOLLOW_BOSS_H__

#include "servercommon/followbossdef.hpp"
#include "servercommon/serverdef.h"

class Role;

class FollowBossManager
{
public:

	FollowBossManager();
	~FollowBossManager();

	void SetRole(Role *role);

	void Init(Role *role, const FollowBossParam &followboss_param);
	void GetInitParam(FollowBossParam *followboss_param);

	void SendFollowBossList();
	void FollowBoss(int boss_type, int boss_id, int scene_id);
	void UnFollowBoss(int boss_type, int boss_id, int scene_id);

private:
	Role *m_role;			
	FollowBossParam	m_followboss_param;
};

#endif
