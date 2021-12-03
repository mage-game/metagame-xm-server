#ifndef __TEAM_HPP__
#define __TEAM_HPP__

#include <vector>
#include <string>
#include <set>

#include "servercommon/servercommon.h"
#include "servercommon/chatserverdef.h"

class Role;

#define INVALID_TEAM_INDEX -1

class Team
{
public:

	struct TeamMember
	{
		TeamMember() : user_id(INVALID_USER_ID), user_uuid(INVALID_UNIQUE_USER_ID), join_time(0), 
			scene_id(-1), scene_key(-1), obj_id(INVALID_OBJ_ID), fbroom_ready(0)
		{
		}

		bool Invalid() { return user_id == INVALID_USER_ID; }
		void Reset() 
		{ 
			user_id = INVALID_USER_ID; join_time = 0; fbroom_ready = 0;
		}
	
		UserID user_id;
		UniqueUserID user_uuid;
		unsigned int join_time;

		int scene_id;
		int scene_key;
		ObjID obj_id;
		short fbroom_ready;
	};

	Team();
	Team(Role *leader_user, bool must_check, bool member_can_invite, int assign_mode, int team_type);
	~Team();

	int GetTeamType() { return m_team_type; }
	int GetTeamIndex() { return m_team_index; }
	int GetTeamSeq() { return m_team_seq; }
	int GetMemberNum() { return m_cur_member_num; }
	int GetMemberUidList(int *member_id_list, int max_count);
	int GetMemberUUidList(long long *member_id_list, int max_count);
	bool IsMustCheck()  { return m_must_check; }
	bool IsMemberCanInvite() { return m_member_can_invite; }
	int GetLimitCapability() { return m_limit_capability; }
	int GetLimitLevel() { return m_limit_level; }
	Role * GetLeader();
	int GetTeamAverageLevel();

	bool GetTeamInfo(GameName leader_name, int *leader_level, char *leader_prof, char *leader_sex, 
					char *leader_vip_level, char *leader_camp, char *team_type, long long *avatar_timestamp);
	int GetTeamLocal(int *scene_key);
	int GetMemberSceneId(const UserID &user_id);

	bool IsLeader(const UserID &user_id);
	bool IsFull() { return m_cur_member_num >= MAX_TEAM_MEMBER_NUM; }
	bool IsNobodyOnline(const UserID &except_userid);
	bool IsHasCouple();
	bool RemoveIfInvite(const UserID &user_id);
	void SendTeamInfo();

	void SetMustCheck(bool must_check);
	void SetMemberCanInvite(bool member_can_invite);
	void SetTeamLimit(int limit_capability, int limit_level);

	void SetTeamIndex(int team_index);
	void SetTeamSeq(int team_seq) { m_team_seq = team_seq; }

	void AutoSelectTeamLeader();
	bool ChangeTeamLeader(const UserID &user_id);
	bool CheckTeamLimit(Role *user);
	bool AddTeamMember(Role *user);
	bool RemoveMember(const UserID &user_id, bool is_kick_off);	
	void Dismiss();
	void Invite(const UserID &user_id);
	bool ChangeAssignMode(int assign_mode);
	bool ChangeTeamFbTeamType(int team_type);
	bool ChangeTeamFbMode(int mode);
	bool ChangeTeamFbLayer(int layer);
	bool ChangeTeamFbAutoStart(int auto_start);
	bool ChangeMenberFbRoomIsRead(const UserID &user_id, int is_ready);
	void BroadcastToAllMember(const char *data, int len);

	bool OnUserChangeScene(const UserID &user_id, int scene_id, int scene_key, ObjID obj_id);
	bool OnUserBaseInfoChange(const UserID &user_id);
	bool OnUserLogin(const UserID &user_id, GSNetID net_id);
	bool OnUserLogout(const UserID &user_id);

	TeamMember * GetTeamMember(const UserID &user_id, int *member_index = NULL);
	TeamMember * GetTeamMember(int index);

	int GetAssignMode() { return m_assign_mode; }
	int GetTeamFbMode() { return m_teamfb_mode;}
	int GetTeamFbLayer() { return m_teamfb_layer;}
	int GetTeamFbAutoStart() { return m_teamfb_autostart; }

	void * operator new(size_t c);
	void operator delete(void *m);

private:
	typedef std::set<UserID> InviterSet;
	typedef std::set<UserID>::iterator InviterSetIt;
	InviterSet m_inviter_set;

	int m_team_index;
	int m_team_seq;

	bool m_must_check;
	bool m_member_can_invite;
	int m_limit_capability;
	int m_limit_level;

	TeamMember m_member_list[MAX_TEAM_MEMBER_NUM];
	int m_cur_member_num;
	int m_leader_index;

	int m_assign_mode;
	int m_team_type;									// 队伍类型 1是无尽祭坛
	int m_teamfb_mode;
	int m_teamfb_layer;
	int m_teamfb_autostart;
};

#endif // __TEAM_HPP__

