#ifndef __ACTIVE_DEGREE_HPP__
#define __ACTIVE_DEGREE_HPP__

#include "servercommon/activedegreedef.hpp"

class Role;

class ActiveDegree
{
public:
	ActiveDegree();
	~ActiveDegree();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const ActiveDegreeParam &param);
	void GetInitParam(ActiveDegreeParam *param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void Update(unsigned int now_second);

	int GetDayOnlineTime() { return m_day_online_time; }
	void AddActiveDegreeHelper(int activedegree_type, int count = 1);
	void OnFetchActiveReward(int activedegree_type);
	bool CanAddActiveDegree(int activedegree_type);

	void SendActiveDegreeInfo();
	void OnFetchReward(int reward_index);
	void OnFetchRewardOnDay(int reward_index);           // ����ÿ�µڼ�����ȡ����
	int GetTotalDegree() { return m_total_degree; }

	void OnUserLogin();

	void AddSetOnlineTime(int minute);

	void GMAddActiveDegree();

private:

	Role *m_role;

	unsigned int m_last_online_time;								// �������ʱ��
	int m_day_online_time;											// �����ۼ�����ʱ��

	int m_total_degree;
	char m_activedegree_list[ACTIVEDEGREE_TYPE_MAX];				// ���ֻ�Ծ�ȵĴ���
	char m_reward_flag_list[ACTIVEDEGREE_REWARD_ITEM_MAX_NUM];
	
	unsigned long long m_activedegree_fetch_flag;							// ��Ծ����ȡ��־

	char m_reward_on_day_flag_list[ACTIVEDEGREE_REWARD_ITEM_MAX_NUM];		// ����ÿ�µڼ����ȡ�Ľ�����ʶ
};

#endif // __ACTIVE_DEGREE_HPP__

