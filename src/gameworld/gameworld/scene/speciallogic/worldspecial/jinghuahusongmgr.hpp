#ifndef _JINGHUASONG_MGR_HPP_
#define _JINGHUASONG_MGR_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include <map>

#include "config/activityconfig/jinghuahusongconfig.hpp"

class Role;

enum JH_HUSONG_STATUS
{
	JH_HUSONG_STATUS_NONE = 0,	// 没护送
	JH_HUSONG_STATUS_HS_FULL,   // 满载护送中
	JH_HUSONG_STATUS_HS_LOST,	// 损失护送中
	JH_HUSONG_STATUS_MAX,
};

class JingHuaHuSongMgr
{
public:
	static JingHuaHuSongMgr & Instance();

	void OnServerStart();
	void OnRoleLogin(Role *role);

	void Update(unsigned long interval, time_t now_second);

	void CheckJingHuaHusongTime(Role *role, time_t now_second);

	void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	void OnRoleDie(Role *die_role, Role *killer_role);
	void OnCommit(Role *role);
	void OnHusongTimeout(Role *role);
	void CheckBuffStatus(Role *role, bool is_add);

	void OnRoleHusongStatusChange(Role *role, int status);
	bool IsLevelLimit(Role *role);
	bool IsHusongStatus(Role *role);
	int GetRemainGatherTimes() { return m_cur_remain_gather_times; }

	void SyncRoleHusongInfo(Role *role);
	void BroadcastHusongInfo(Role *role);

	void GmRefresh();

private:
	JingHuaHuSongMgr();
	~JingHuaHuSongMgr();

	UInt32 last_dayid;
	UInt32 m_next_try_refresh_time;

	bool m_isopen;
	int m_last_refresh_gather_times;
	int m_cur_remain_gather_times;

	ObjID m_gather_objid;
};

#endif




