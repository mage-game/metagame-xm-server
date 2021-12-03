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
		TOR_SUC						= 0,		// �ɹ�
		TOR_TEAM_NOT_EXIST			= -1,		// ���鲻����
		TOR_MUST_CHECK				= -2,		// ������֤
		TOR_TEAM_IS_FULL			= -3,		// ������
		TOR_ALREADY_IN_TEAM			= -4,		// ���ж���
		TOR_NO_POWER				= -5,		// �޲���Ȩ��
		TOR_NOT_IN_THIS_TEAM		= -6,		// ���ڱ�����
		TOR_INVALID_ASSIGN_MODE		= -7,		// ����ģʽ��Ч
		TOR_TEAM_LIMIT				= -8,		// �����������������
		TOR_IN_TEAM_FB				= -9,		// �����Ѿ��ڸ�����
		TOR_IN_TEAM_MATCHING		= -10,		// �����ڻƥ��״̬��
		TOR_IN_ROLE_MATCHING		= -11,		// �������ڻƥ��״̬��
		TOR_IN_THIS_TEAM			= -12,		// ���ڸö���
		TOR_LEADER_NOT_EXIST		= -13,		// �ӳ�������
	};

	/*
		�޾���̳��������� �������ģʽ�������£���Ҫ�ǽ���û����鵼�µ�
		1���ӳ����ߣ���ɢ����
		2����Ա���ߣ��Զ��˶�
		3�����˼��룬��������Ѿ����븱��������ʾ���
		4���ͻ����ڸ����������뿪������ʱ�򣬻��Զ��뿪���飬�������뿪�������Զ���ɢ
	*/

	enum TEAM_TYPE
	{
		TEAM_TYPE_DEFAULT = 0,					// Ĭ�����
		TEAM_TYPE_YAOSHOUJITANG = 1,			// ���޼�̳����
		TEAM_TYPE_TEAM_TOWERDEFEND = 2,			// �����������
		TEAM_TYPE_TEAM_MIGONGXIANFU = 3,		// �Թ��ɸ�����
		TEAM_TYPE_TEAM_EQUIP_FB = 4,			// ���װ������
		TEAM_TYPE_TEAM_DAILY_FB = 5,			// �ճ����鸱��
		TEAM_TYPE_EQUIP_TEAM_FB = 6,			// ��Ӣ���װ������
		TEAM_TYPE_QINGYUAN_FB = 7,				// ��Ե��Ӹ���

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
	bool CheckTeamInTeamTypeFB(Team *team);   // �Ƿ�������͸���ģʽ
	int CheckCanMergeTeam(Role *role, Team *target_team, bool is_ret);		// ����Ƿ���Ժϲ�role���鵽target_team
	void SendCheckMergeResultToRole(Role *role, int check_ret, int is_ret);		// ������ʾ����ɫ

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
