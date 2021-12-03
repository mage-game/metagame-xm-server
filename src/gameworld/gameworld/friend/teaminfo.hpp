#ifndef __TEAMINFO_HPP__
#define __TEAMINFO_HPP__

#include "gamedef.h"
#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/ggprotocal.h"

class Role;
class Team;

static const int TEAM_CALL_TOGETHER_TIME_LIMIT_INTERVAL_S = 8;		// 组队召集限制时间间隔
static const int TEAM_CALL_IN_TIME_LIMIT_INTERVAL_S = 8;			// 组队招募限制时间间隔

class TeamInfo
{
public:
	TeamInfo();
	~TeamInfo();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, int team_inde, int team_seq, char is_auto_join_team);
	void GetInitParam(int *team_index_p, int *team_seq_p, char *is_auto_join_team);

	int GetTeamIndex() { return m_team_index; }
	void LoginFixTeamIndexSeq(int index, int seq) { m_team_index = index; m_team_seq = seq; }

	bool InTeam();
	bool IsMember();
	bool IsMember(const UserID &user_id);
	bool IsLeader();

	int GetOtherMemberInTeam(Role **member_list, int max_list_size);      // 取出在周围的队友
	int GetOtherMemberInTheSameScene(Role **member_list, int max_list_size);// 取出在同一个场景的队友
	int GetOtherMemberList(Role **member_list, int max_list_size);// 取出所有队友

	void SendMyPosToMember(bool is_leave_scene = false);
	void SendTeamMemberPosToMe();
	
	void OnSelfJoinTeam(int index, int seq);
	void OnSelfLeaveTeam();

	void OnTeamCallTogether();
	void OnTeamCallIn(int fb_id);

	void ReCalcFriendBuff();

	Team * GetMyTeam();
	int GetMyTeamAssignMode();

	bool IsAutoJoinTeam() { return m_is_auto_join_team == 1; }
	void SetIsAutoJoinTeam(char is_auto_join_team) { m_is_auto_join_team = is_auto_join_team; }

	void SyncRoleTeamInfo();
private:
	Role*		m_role;
	
	int			m_team_index;							// 队伍ID
	int			m_team_seq;								// 队伍序列号 这才是队伍的唯一标记 

	time_t		m_last_call_together_time;				// 最后召集时间
	time_t		m_last_call_in_time;					// 最后招募时间

	char		m_is_auto_join_team;					// 自动加入队伍
};

#endif

