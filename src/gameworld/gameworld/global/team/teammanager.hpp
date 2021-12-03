#ifndef __TEAM_MANAGER_HPP__
#define __TEAM_MANAGER_HPP__

#include <map>

#include "common/raobjlist.h"
#include "servercommon/serverdef.h"
#include "protocal/msgteam.hpp"

class Team;
class Role;

class TeamManager
{
public:
	enum TEAM_OPER_RESULT
	{
		TOR_SUC						= 0,		// 成功
		TOR_TEAM_NOT_EXIST			= -1,		// 队伍不存在
		TOR_MUST_CHECK				= -2,		// 必须验证
		TOR_TEAM_IS_FULL			= -3,		// 队伍满
		TOR_ALREADY_IN_TEAM			= -4,		// 已有队伍
		TOR_NO_POWER				= -5,		// 无操作权限
		TOR_NOT_IN_THIS_TEAM		= -6,		// 不在本队伍
		TOR_INVALID_ASSIGN_MODE		= -7,		// 分配模式无效
		TOR_TEAM_LIMIT				= -8,		// 不满足队伍限制条件
		TOR_IN_TEAM_FB				= -9,		// 队伍已经在副本中
		TOR_IN_TEAM_MATCHING		= -10,		// 队伍在活动匹配状态中
		TOR_IN_ROLE_MATCHING		= -11,		// 加入者在活动匹配状态中
		TOR_IN_THIS_TEAM			= -12,		// 已在该队伍
		TOR_LEADER_NOT_EXIST		= -13,		// 队长不存在
	};

	/*
		无尽祭坛、组队塔防 特殊组队模式处理如下，主要是解决用户体验导致的
		1、队长下线，解散队伍
		2、队员下线，自动退队
		3、有人加入，如果队伍已经进入副本，则提示玩家
		4、客户端在副本结束，离开场景的时候，会自动离开队伍，所有人离开，队伍自动解散
	*/

	enum TEAM_TYPE
	{
		TEAM_TYPE_DEFAULT = 0,					// 默认组队
		TEAM_TYPE_YAOSHOUJITANG = 1,			// 妖兽祭坛副本
		TEAM_TYPE_TEAM_TOWERDEFEND = 2,			// 组队塔防副本
		TEAM_TYPE_TEAM_MIGONGXIANFU = 3,		// 迷宫仙府副本
		TEAM_TYPE_TEAM_EQUIP_FB = 4,			// 组队装备副本
		TEAM_TYPE_TEAM_DAILY_FB = 5,			// 日常经验副本
		TEAM_TYPE_EQUIP_TEAM_FB = 6,			// 精英组队装备副本
		TEAM_TYPE_QINGYUAN_FB = 7,				// 情缘组队副本

		TEAM_TYPE_MAX,
	};

	static TeamManager & Instance();

	void SyncPersonBaseInfo(Role *user);
	void OnUserLogin(Role *user);
	void OnUserLogout(Role *user);
	bool OnUserChangeScene(int team_index, const UserID &user_id, int scene_id, int scene_key, ObjID obj_id);

	static bool IsValidTeamIndex(int team_index) { return team_index > 0; }

	int CreateTeam(Role *leader, bool must_check, bool member_can_invite, int assign_mode, int team_type = TEAM_TYPE_DEFAULT);
	bool DismissTeam(int team_index, Role *user);
	int AddTeamMember(Role *executor, int team_index, Role *user, bool is_call_in_ack = false);
	int RemoveTeamMember(Role *executor, int team_index, const UserID &user_id);	
	int	ChangeTeamLeader(Role *executor, Role *user);
	bool ChangeMustCheck(Role *executor, bool must_check);
	bool ChangeMemberCanInvite(Role *executor, bool member_can_invite);
	bool ChangeAssignMode(Role *executor, int assign_mode);
	bool ChangeTeamLimit(Role *executor, int limit_capability, int limit_level);
	int Invite(Role *executor, Role *be_invited_user);

	//bool ChangeTowerDefendAttrType(Role *executor, int team_index, const UserID &user_id, char type);
	bool CheckTeamInTeamTypeFB(Team *team);   // 是否在组队型副本模式
	int CheckCanMergeTeam(Role *role, Team *target_team, bool is_ret);		// 检查是否可以合并role队伍到target_team
	void SendCheckMergeResultToRole(Role *role, int check_ret, int is_ret);		// 发送提示给角色

	void AutoHaveTeam(Role *user);

	Role * GetLeader(int team_index);
	bool IsTeamFull(int team_index);
	void SendTeamListToUser(Role *user);

	void OnCreateTeam(Role *role, Protocol::CSCreateTeam *ct);
	void OnInviteUser(Role *role, Protocol::CSInviteUser *iu);
	void OnInviteUniqueUser(Role *role, Protocol::CSInviteUniqueUser *iu);
	void OnInviteRet(Role *role, Protocol::CSInviteUserTransmitRet *iutr);
	void OnJoinTeamRet(Role *role, Protocol::CSReqJoinTeamRet *jtr);
	void ReqJoinTeamHelper(Role *user, int team_index, bool is_call_in_ack);
	void OnReqJoinTeam(Role *role, Protocol::CSReqJoinTeam *rjt);
	void OnDismissTeam(Role *user);
	void OnKickOutOfTeam(Role *user, Protocol::CSKickOutOfTeam *koof);
	void OnChangeTeamLeader(Role *user, Protocol::CSChangeTeamLeader *ctl);
	void OnExitTeam(Role *user);
	void OnTeamListReq(Role *role);

	Team * GetTeam(int index);
	RAObjList<Team*> &GetTeamList() { return m_team_list; }
	
private:
	TeamManager();
	~TeamManager();

	void DestroyTeamHelper(Team *team);

	void AddTeamLocalMap(Role *user, int team_index);
	void RemoveTeamLocalMap(Role *user, int team_index);
	void AddTeamLocalMap(int scene_id, int scene_key, int team_index);
	void RemoveTeamLocalMap(int scene_id, int scene_key, int team_index);

	void SaveTeamIndex(int uid, int team_index);
	int GetTeamIndex(int uid, bool is_remove = false);

	bool OnUserBaseInfoChange(int team_index, const UserID &user_id);

	typedef RAObjList<Team*> TeamList;
	TeamList m_team_list;

	typedef std::set<int> TeamIndexSet;
	struct SceneTeamInfo 
	{
		SceneTeamInfo() {}

		Protocol::SCTeamListAck msg;
		TeamIndexSet team_index_set;
	};

	struct SceneLocal 
	{
		SceneLocal() : scene_id(-1), scene_key(-1) {}
		SceneLocal(int sid, int skey) : scene_id(sid), scene_key(skey) {}

		int scene_id;
		int scene_key;
		bool operator < (const SceneLocal &v) const
		{
			return scene_id < v.scene_id || (scene_id == v.scene_id && scene_key < v.scene_key); 
		}
	};

	typedef std::map<SceneLocal, SceneTeamInfo>	TeamLocalMap;
	TeamLocalMap m_team_local_map;

	int m_team_seq_aotu_increment;

	typedef std::map<int, int> UserTeamIndexMap;
	typedef std::map<int, int>::iterator UserTeamIndexMapIt;
	UserTeamIndexMap m_user_teamindex_map;
};

#endif // __TEAM_MANAGER_HPP__
