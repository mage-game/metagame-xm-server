#pragma once

#ifndef RAND_ACTIVITY_HOLIDAY_GUARD_HPP
#define RAND_ACTIVITY_HOLIDAY_GUARD_HPP

#include "global/randactivity/randactivity.hpp"

typedef Protocol::KillRankNoticeInfo KillRankNoticeInfo;

struct KillMonsterRank
{
	KillMonsterRank() : uid(0), kill_monster_count(0), pass_time(0), meet_requirement_timestamp(0) {}

	void SetData(const KillRankNoticeInfo &param)
	{
		this->uid = param.uid;
		this->kill_monster_count = param.kill_monster_count;
		this->meet_requirement_timestamp = param.pass_time;
	}

	int uid;
	short kill_monster_count;
	unsigned short pass_time;
	unsigned int meet_requirement_timestamp;
};

class RandActivityHolidayGuard : public RandActivity
{
public:
	RandActivityHolidayGuard(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityHolidayGuard();

	void Init(const RandActivityData & data);
	void GetInitParam(RandActivityData * data);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnUserLogin(Role *user);

	void AddOrReplaceRole(Role *role);
	void SendRankInfo(Role *role = nullptr);
	void SendNpcINfo(Role *role);
	void DelNpc(int npc_index);
	void DelAllNpc();

	int GetRankListInfo(UID role_uid, int &kill_monster_count);

private:

	void SetRankInfo(Role *role);
	void OnRefreshNpc();
	void SetNpcRefreshTime();
	void UpdateRankInfo(Role *role, int kill_count, int meet_requirement_timestamp);
	void SyncRankInfo();

	int m_cur_rank_num;
	KillRankNoticeInfo m_person_kill_notice_list[RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX];
	unsigned int m_send_rank_info_interval;

	KillMonsterRank m_rank_param_list[RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX];

	unsigned int m_can_enter_fb_begin_timestamp;
	unsigned int m_can_enter_fb_end_timestamp;
	unsigned int m_refresh_interval_timestamp;

	HolidayGuardNpcRefreshCfg m_npc_info_list[HOLIDAYGUARD_NPC_CFG_MAX_COUNT];
	int m_left_npc_num;														// npc剩余数量
	int m_is_need_refresh;													// 是否刷新

	unsigned int m_fixed_point_refresh_npc_timestamp;
};

#endif //RAND_ACTIVITY_HOLIDAY_GUARD_HPP