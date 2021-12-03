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
		REQ_ROOM_LIST = 1,			// 请求房间列表
		CREATE_ROOM,				// 创建房间
		JOIN_ROOM,					// 加入指定房间
		START_ROOM,					// 开始房间
		EXIT_ROOM,					// 退出房间
		CHANGE_MODE,				// 选择模式
		KICK_OUT,					// T人
		ENTER_AFFIRM,				// 确认进入
	};

	int GetFbTypeByTeamType(int team_type);
	bool CheckIsCanEnterFbOnJoin(Role *role, int fb_type, int param_1, int check_reason);

private:
	TeamFbRoom(); 
	~TeamFbRoom(); 

	enum TeamMemberState
	{
		DEFAULT_STAE = 0,			// 默认状态
		REJECT_STATE,				// 拒绝进入
		AGREE_STATE,				// 同意进入
	};

};

#endif
