#ifndef __MSGTEAM_H__
#define __MSGTEAM_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/servercommon.h"

#include "global/team/team.hpp"
#include "protocal/msgteam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class CSCreateTeam											// 9150玩家申请创建队伍请求
	{
	public:
		CSCreateTeam();
		MessageHeader		header;
		char				must_check;
		char				assign_mode;
		char				member_can_invite;
		char				team_type;
	};

	struct TeamMemberInfo
	{
		long long			role_uuid;
		int					role_id;
		GameName			name;
		int					scene_key;
		int					scene_id;
		int					is_online;
		char				prof;
		char				camp;
		char				sex;
		char				reserve_ch1;
		short				level;
		char				reserve_ch2;
		char 				fbroom_ready;
		int					capability;
		long long			avatar_timestamp;
	};
	static const int MAX_TEAM_MEMBER_COUNT = 10;

	class SCTeamInfo											// 9100发送队伍信息给玩家
	{
	public:
		SCTeamInfo();
		MessageHeader		header;
		int					team_index;
		char				team_leader_index;
		char				assign_mode;
		char				must_check;
		char				member_can_invite;
		int					limit_capability;
		int					limit_level;
		int					member_count;
		char				team_type;
		char				teamfb_mode;
		char				teamfb_layer;
		char				reserve_sh;
		TeamMemberInfo		team_member[MAX_TEAM_MEMBER_COUNT];
	};

	enum OUT_OF_TEAM_REASON
	{
		OFT_DISMISS =  0,	
		OFT_KICK_OFF,		// 被踢出队伍
		OFT_SECEDE,			// 自己退出
	};

	class SCOutOfTeam												// 9101通知玩家离开了队伍
	{
	public:
		SCOutOfTeam();
		MessageHeader		header;
		int					reason;
		int					user_id;
		GameName			user_name;
	};

	class SCJoinTeam												// 9105 通知玩家加入了队伍  
	{
	public:
		SCJoinTeam();
		MessageHeader		header;
		int					user_id;
		GameName			user_name;
	};

	class SCTeamLeaderChange										//  9108通知队长发生变更
	{
	public:
		SCTeamLeaderChange();
		MessageHeader		header;
		int					user_id;
		GameName			user_name;
	};

	class CSInviteUser												// 9151邀请玩家加入队伍请求
	{
	public:
		CSInviteUser();
		MessageHeader		header;
		int					role_id;

	};

	class CSInviteUniqueUser										// 9166邀请玩家加入队伍请求UUID
	{
	public:
		CSInviteUniqueUser();
		MessageHeader		header;
		long long			role_uuid;

	};

	class SCInviteUserTransmit										// 9102邀请信息转发给被邀请用户
	{
	public:
		SCInviteUserTransmit();
		MessageHeader		header;
		int					inviter;
		GameName			inviter_name;
		char				inviter_camp;
		char				inviter_prof;
		char				inviter_sex;
		char				inviter_team_num;;
		int					inviter_level;
		long long			inviter_avatar_timestamp;
	};

	class CSInviteUserTransmitRet										// 9152回复邀请
	{
	public:
		CSInviteUserTransmitRet();
		MessageHeader		header;
		int					inviter;
		int					result;	 // 0 同意
	};

	class SCReqJoinTeamTransmit											// 9103队长审核申请加入队伍成员
	{
	public:
		SCReqJoinTeamTransmit();
		MessageHeader		header;
		int					req_role_id;
		GameName			req_role_name;
		char				req_role_camp;
		char				req_role_prof;
		char				req_role_sex;
		char				reserved;
		int					req_role_level;
		long long			req_avatar_timestamp;
		int					req_total_cabality;
	};

	class SCRoleTeamInfo												// 9106角色自己相关的组队信息
	{
	public:
		SCRoleTeamInfo();
		MessageHeader		header;

		int					is_auto_apply_join_team;
	};

	class SCTeamRollDropRet												// 9107掉落摇点
	{
	public:
		SCTeamRollDropRet();
		MessageHeader		header;

		char roll1;
		char roll2;
		char roll3;
		char roll4;
	};

	class CSReqJoinTeamRet												//  9153队长回复申请加入队伍审核
	{
	public:
		CSReqJoinTeamRet();
		MessageHeader		header;
		int					req_role_id;
		int					result;  // 0 同意
	};

	class CSReqJoinTeam													// 5194玩家申请加入某队伍
	{
	public:
		CSReqJoinTeam();
		MessageHeader		header;
		int					team_index;
		int					is_call_in_ack;	// 是否回应招募
	};

	class CSDismissTeam													// 9155队长解散队伍
	{
	public:
		CSDismissTeam();
		MessageHeader		header;
	};

	class CSKickOutOfTeam												// 9156踢出队友
	{
	public:
		CSKickOutOfTeam();
		MessageHeader		header;
		int					role_id;
	};

	class CSChangeTeamLeader											// 9157换队长
	{
	public:
		CSChangeTeamLeader();
		MessageHeader		header;
		int					role_id;
	};

	class CSChangeMustCheck												// 9160改变队伍加入是否是要队长通过
	{
	public:
		CSChangeMustCheck();
		MessageHeader		header;
		int					must_check;
	};

	class CSChangeAssignMode											// 9161改变队伍分配模式
	{
	public:
		CSChangeAssignMode();
		MessageHeader		header;
		int					assign_mode;
	};

	class CSChangeMemberCanInvite										// 9162改变队伍是否普通队员可邀请
	{
	public:
		CSChangeMemberCanInvite();
		MessageHeader		header;
		int					member_can_invite;
	};

	class CSChangeTeamLimit												// 9163改变队伍限制条件
	{
	public:
		CSChangeTeamLimit();
		MessageHeader		header;
		int					limit_capability;
		int					limit_level;
	};

	class CSAutoHaveTeam												// 9164玩家自动拥有队伍
	{
	public:
		CSAutoHaveTeam();
		MessageHeader		header;
	};

	class CSAutoApplyJoinTeam											// 9165自动答应邀请
	{
	public:
		CSAutoApplyJoinTeam();
		MessageHeader header;

		int is_auto_join_team;
	};

	class CSExitTeam													// 9158玩家退出队伍
	{
	public:
		CSExitTeam();
		MessageHeader		header;
	};

	class CSTeamListReq													// 9159请求所在场景的队伍列表
	{
	public:
		CSTeamListReq();
		MessageHeader		header;
	};

	struct TeamItem 
	{
		int					team_index;
		GameName			leader_name;
		int					leader_level;
		int					cur_member_num;
		char				leader_vip_level;
		char				leader_prof;
		char				leader_camp;
		char				leader_sex;
		char				team_type;
		char				reserve_ch;
		short				reserve_sh;
		long long			avatar_timestamp;
		int					member_uid_list[MAX_TEAM_MEMBER_NUM];
		long long			member_uuid_list[MAX_TEAM_MEMBER_NUM];
	};

	static const int MAX_TEAM_LIST_NUM = 128;

	class SCTeamListAck													// 9104请求所在场景队伍列表回复
	{
	public:
		SCTeamListAck();
		MessageHeader		header;
		int					count;
		TeamItem			team_list[MAX_TEAM_LIST_NUM];
	};
}

#pragma pack(pop)

#endif

