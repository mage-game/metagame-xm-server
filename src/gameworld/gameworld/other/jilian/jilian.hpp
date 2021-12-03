#ifndef __JILIAN_HPP__
#define __JILIAN_HPP__

class Role;
#include "servercommon/serverdef.h"
#include "gamedef.h"

enum JILIAN_TYPE
{
	JILIAN_TYPE_INVALID,
	JILIAN_TYPE_COMMON,
	JILIAN_TYPE_SUPER,

	JILIAN_TYPE_MAX,
};

class Jilian
{
public:
	Jilian();
	~Jilian();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, char jilian_type, char is_working, short progress, char once_join, char qte_succ_times) ;
	void GetInitParam(char *jilian_type, char *is_working, short *progress, char *once_join, char *qte_succ_times);

	void Update(unsigned int now_second);
	void OnRoleLeaveScene();
 
	void Start(int jilian_type);
	void Continue();

	void CheckCanQte();
	void OnReportQteResult(bool qte_succ);
	void SendQteInfo();

	void SendInfo();

	bool IsWorking() { return (0 != m_is_working); }
	char GetJilianType() { return m_jilian_type; }

	void BroadcastJilianType();

private:
	Role *m_role;

	char m_once_join;
	char m_jilian_type;
	char m_is_working;
	short m_progress;

	unsigned int m_last_work_timestamp;

	bool m_qte_double_reward;
	unsigned int m_qte_limit_timestamp;

	char m_qte_succ_times;
};

#endif

