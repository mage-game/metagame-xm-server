#ifndef __RAND_ACTIVITY_HPP__
#define __RAND_ACTIVITY_HPP__

#include "iclockmodule.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/rankdef.hpp"

#include "engineadapter.h"
#include "global//rank/personrank.hpp"

class Role;
class RandActivityManager;

class RandActivity
{
public:
	RandActivity(RandActivityManager *rand_activity_manager, int activity_type);
	virtual ~RandActivity();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid) {}

	virtual void OnUserLogin(Role *user);
	virtual void OnUserLogout(Role *user) {}

	virtual void CalActivityOpenTime();

	virtual void SetRandActivityTime(unsigned int begin_time, unsigned int end_time, int open_type = RAND_ACTIVITY_OPEN_TYPE_NORMAL);
	void OnForceToNextState();

	void SyncActivityStatusToGameworld();

	int GetActivityType() { return m_rand_activity_type; }
	int GetStatus() { return m_rand_activity_status; }

	unsigned int GetBeginTime() const { return m_begin_time; }
	unsigned int GetEndTime() const { return m_end_time; }

	virtual void OnPersonRankSnapshot(int old_top_uid_list[PERSON_RANK_TYPE_MAX], int new_top_uid_list[PERSON_RANK_TYPE_MAX]) {}
	virtual bool IsSyncRankData() { return false; }
	virtual void RankBipinHistory(PersonRankItem top_user_list[Protocol::TOP_COUNT]) {}

	bool IsOpenActivityOfVersion() { return ACTIVITY_STATUS_OPEN == m_rand_activity_status &&  RAND_ACTIVITY_OPEN_TYPE_VERSION == m_open_type; }

protected:
	void NotfiyRandActivityStatus(bool is_broadcast = false, Role *user = NULL);

	RandActivityManager *m_rand_activity_manager;

	int m_rand_activity_type;
	int m_rand_activity_status;

	unsigned int m_begin_time;
	unsigned int m_end_time;
	int m_open_type;

	unsigned int m_last_notice_time;

	void Broadcast(time_t now_second);
};

#endif // __SPECIAL_ACTIVITY_HPP__
