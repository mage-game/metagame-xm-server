#ifndef _HUANGCHENGHUIZHAN_MGR_HPP_
#define _HUANGCHENGHUIZHAN_MGR_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include <map>
#include "obj/character/monster.h"
#include "config/activityconfig/jinghuahusongconfig.hpp"

class Role;

class HuangchenghuizhanMgr
{
public:
	static HuangchenghuizhanMgr & Instance();

	void OnRoleLogin(Role *role);

	void Update(unsigned long interval, time_t now_second);
	void OnActivityStatusChange(int status);
	void OnMonsterDie(Monster *monster, Role *killer_role);
	void KillAllMonster();
	bool IsInHuangChengHuizhan(Role *role);

	void SyncHuangChengInfoToAll();
	void SyncHuangChengInfoToRole(Role *role);

private:
	HuangchenghuizhanMgr();
	~HuangchenghuizhanMgr();

	unsigned int m_next_refresh_time;
	int m_monster_num;
};

#endif




