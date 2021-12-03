#ifndef __CROSS_ACTIVITY_HPP__
#define __CROSS_ACTIVITY_HPP__ 

#include "engineadapter.h"

#include "servercommon/serverdef.h"
#include "servercommon/crossdef.hpp"

class CrossUser;
class CrossActivityManager;

class CrossActivity
{	
public:
	CrossActivity(CrossActivityManager *cross_activity_manager, int cross_activity_type);
	virtual ~CrossActivity();

	virtual void Init(const CrossActivityData &data);
	virtual void GetInitParam(CrossActivityData *data);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange() {}
	virtual void OnWeekChange() {}

	bool IsLoadFinish() { return m_is_init_finish; }

	virtual void OnRegisterGameServer(NetID global_netid);
	virtual void OnGSDisconnect(NetID global_netid, const UniqueServerID &unique_server_id) {}
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual void OnCrossUserLogout(CrossUser *cross_user);
	
	int GetCrossActivityType() { return m_cross_activity_type; }
	int GetActivityStatus() { return m_activity_status; }
	virtual int GetActivitySpecialParam() { return 0; }

	virtual int GetWaitSceneID(const UniqueUserID &unique_user_id, Posi *pos);

	void ForceToNextState();

	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return false; }

	// ͬ���������������
	void SyncCrossChallengefieldToGame(NetID global_netid);

protected:
	virtual void OnChangeToNextStatus();

	void SyncActivityStatusToGlobal(NetID global_netid = UINT_MAX);

	CrossActivityManager *m_cross_activity_manager;
	
	int m_cross_activity_type;						// ��������
	bool m_is_init_finish;							// ��ʼ����ɱ�־

	int m_activity_status;							// ��ǰ�״̬
	time_t m_status_begin_time;						// ��ǰ״̬��ʼʱ��
	time_t m_status_end_time;						// ��ǰ״̬����ʱ��
};

#endif // __CROSS_ACTIVITY_HPP__

