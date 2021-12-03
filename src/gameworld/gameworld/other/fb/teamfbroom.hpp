#ifndef __TEAMFBROOM_HPP__
#define __TEAMFBROOM_HPP__

#include <map>
#include "protocal/msgfb.h"
class Role;

class TeamFbRoom
{
public:

	static TeamFbRoom & Instance();

	void ReqOperate(Role *role, Protocol::CSTeamFbRoomOperate *cmd);
	
	void SyncRoomList(Role *role, int team_type, int mode, int layer);
	void CreateRoom(Role *role, int team_type, int mode, int limit_capability, int layer, int is_auto_start = 0);
	void JoinRoom(Role *role, int team_type, int team_index, int layer);
	void Start(Role *role);
	void EnterAffirm(Role *role, int is_ready);
	void Exit(Role *role);
	void ChangeMode(Role *role, int team_type, int mode, int layer);
	void KickOut(Role *role, int role_id);

	enum OperateType
	{
		REQ_ROOM_LIST = 1,			// ���󷿼��б�
		CREATE_ROOM,				// ��������
		JOIN_ROOM,					// ����ָ������
		START_ROOM,					// ��ʼ����
		EXIT_ROOM,					// �˳�����
		CHANGE_MODE,				// ѡ��ģʽ
		KICK_OUT,					// T��
		ENTER_AFFIRM,				// ȷ�Ͻ���
	};

	int GetFbTypeByTeamType(int team_type);
	bool CheckIsCanEnterFbOnJoin(Role *role, int fb_type, int param_1, int check_reason);

private:
	TeamFbRoom(); 
	~TeamFbRoom(); 

	enum TeamMemberState
	{
		DEFAULT_STAE = 0,			// Ĭ��״̬
		REJECT_STATE,				// �ܾ�����
		AGREE_STATE,				// ͬ�����
	};

};

#endif
