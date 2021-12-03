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
	class CSCreateTeam											// 9150������봴����������
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

	class SCTeamInfo											// 9100���Ͷ�����Ϣ�����
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
		OFT_KICK_OFF,		// ���߳�����
		OFT_SECEDE,			// �Լ��˳�
	};

	class SCOutOfTeam												// 9101֪ͨ����뿪�˶���
	{
	public:
		SCOutOfTeam();
		MessageHeader		header;
		int					reason;
		int					user_id;
		GameName			user_name;
	};

	class SCJoinTeam												// 9105 ֪ͨ��Ҽ����˶���  
	{
	public:
		SCJoinTeam();
		MessageHeader		header;
		int					user_id;
		GameName			user_name;
	};

	class SCTeamLeaderChange										//  9108֪ͨ�ӳ��������
	{
	public:
		SCTeamLeaderChange();
		MessageHeader		header;
		int					user_id;
		GameName			user_name;
	};

	class CSInviteUser												// 9151������Ҽ����������
	{
	public:
		CSInviteUser();
		MessageHeader		header;
		int					role_id;

	};

	class CSInviteUniqueUser										// 9166������Ҽ����������UUID
	{
	public:
		CSInviteUniqueUser();
		MessageHeader		header;
		long long			role_uuid;

	};

	class SCInviteUserTransmit										// 9102������Ϣת�����������û�
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

	class CSInviteUserTransmitRet										// 9152�ظ�����
	{
	public:
		CSInviteUserTransmitRet();
		MessageHeader		header;
		int					inviter;
		int					result;	 // 0 ͬ��
	};

	class SCReqJoinTeamTransmit											// 9103�ӳ���������������Ա
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

	class SCRoleTeamInfo												// 9106��ɫ�Լ���ص������Ϣ
	{
	public:
		SCRoleTeamInfo();
		MessageHeader		header;

		int					is_auto_apply_join_team;
	};

	class SCTeamRollDropRet												// 9107����ҡ��
	{
	public:
		SCTeamRollDropRet();
		MessageHeader		header;

		char roll1;
		char roll2;
		char roll3;
		char roll4;
	};

	class CSReqJoinTeamRet												//  9153�ӳ��ظ��������������
	{
	public:
		CSReqJoinTeamRet();
		MessageHeader		header;
		int					req_role_id;
		int					result;  // 0 ͬ��
	};

	class CSReqJoinTeam													// 5194����������ĳ����
	{
	public:
		CSReqJoinTeam();
		MessageHeader		header;
		int					team_index;
		int					is_call_in_ack;	// �Ƿ��Ӧ��ļ
	};

	class CSDismissTeam													// 9155�ӳ���ɢ����
	{
	public:
		CSDismissTeam();
		MessageHeader		header;
	};

	class CSKickOutOfTeam												// 9156�߳�����
	{
	public:
		CSKickOutOfTeam();
		MessageHeader		header;
		int					role_id;
	};

	class CSChangeTeamLeader											// 9157���ӳ�
	{
	public:
		CSChangeTeamLeader();
		MessageHeader		header;
		int					role_id;
	};

	class CSChangeMustCheck												// 9160�ı��������Ƿ���Ҫ�ӳ�ͨ��
	{
	public:
		CSChangeMustCheck();
		MessageHeader		header;
		int					must_check;
	};

	class CSChangeAssignMode											// 9161�ı�������ģʽ
	{
	public:
		CSChangeAssignMode();
		MessageHeader		header;
		int					assign_mode;
	};

	class CSChangeMemberCanInvite										// 9162�ı�����Ƿ���ͨ��Ա������
	{
	public:
		CSChangeMemberCanInvite();
		MessageHeader		header;
		int					member_can_invite;
	};

	class CSChangeTeamLimit												// 9163�ı������������
	{
	public:
		CSChangeTeamLimit();
		MessageHeader		header;
		int					limit_capability;
		int					limit_level;
	};

	class CSAutoHaveTeam												// 9164����Զ�ӵ�ж���
	{
	public:
		CSAutoHaveTeam();
		MessageHeader		header;
	};

	class CSAutoApplyJoinTeam											// 9165�Զ���Ӧ����
	{
	public:
		CSAutoApplyJoinTeam();
		MessageHeader header;

		int is_auto_join_team;
	};

	class CSExitTeam													// 9158����˳�����
	{
	public:
		CSExitTeam();
		MessageHeader		header;
	};

	class CSTeamListReq													// 9159�������ڳ����Ķ����б�
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

	class SCTeamListAck													// 9104�������ڳ��������б�ظ�
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

