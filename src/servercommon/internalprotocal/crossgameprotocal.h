#ifndef __CROSS_GAME_PROTOCAL_H__
#define __CROSS_GAME_PROTOCAL_H__

#include "servercommon/serverdef.h"
#include "servercommon/rankdef.hpp"
#include "servercommon/crossdef.hpp"
#include "servercommon/struct/roleallparam.hpp"
#include "servercommon/struct/global/challengeuserparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace crossgameprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		MT_CROSS_GAME_BEGIN = 29000,
		MT_CROSS_GAME_SYNC_CROSS_USER,					// ͬ���ѿ����ɫ������ԭ��
		MT_GAME_CROSS_START_CROSS_REQ,					// ��ʼ�������
		MT_GAME_CROSS_REGISTER_GAME_SERVER,				// ע��gameserver
		MT_CROSS_GAME_SAVE_CROSS_ROLE_INFO,				// ��������ɫ��Ϣ
		MT_GAME_CROSS_SAVE_CROSS_ROLE_RET,				// ��������ɫ��Ϣ����
		MT_CROSS_GAME_START_CROSS_ACK,					// ��ʼ�����Ӧ
		MT_CROSS_GAME_USER_MSG,							// cross���û�����Ϣ Protocol
		MT_CROSS_GAME_ALL_USER_MSG,						// cross�������û�����Ϣ Protocol
		MT_GAME_CROSS_USER_MSG,							// �û�����Ϣ��cross Protocol
		MT_GAME_CROSS_USER_LOGIN,						// ֪ͨcross��ɫ�ѵ�¼�����ط�
		MT_GAME_CROSS_USER_LOGOUT,						// ֪ͨcross��ɫ�˳����ط�
		MT_GAME_CROSS_KICK_CROSS_USER,					// game����cross�ߵ������ɫ
		MT_GAME_CROSS_TRANSFER_TO_GAME,					// gameͨ��crossת��������game

		MT_CROSS_GAME_SYNC_CROSS_ACTIVITY_STATUS,		// ͬ������״̬
		MT_CROSS_GAME_XIULUOTA_GOTO_SCENE,				// ֪ͨ��������������
		MT_GAME_CROSS_ACTIVITY_FORCE_NEXTSTATE,			// ǿ�ƻ������һ״̬
		MT_GAME_CROSS_SYNC_XIULUOTOWER_RESULT,			// ͬ�����������
		MT_CROSS_GAME_SYNC_XIULUOTOWER_USER_RESULT,		// ͬ�����������
		MT_GAME_CROSS_SYNC_XIULUOTOWER_INFO,			// ͬ����������Ϣ
		MT_CROSS_GAME_CROSS_SYNC_XIULUOTOWER_INFO,		// ͬ����������Ϣ
		MT_GAME_CROSS_SYNC_DROP_INFO,					// ͬ��������������Ϣ
		MT_GAME_CROSS_SYNC_XIULUOTOWER_RANK_TITLE,		// ͬ�����������з��ƺ�
		MT_CROSS_GAME_SYNC_XIULUOTOWER_RANK_TITLE,		// ͬ�����������з��ƺ�

		MT_CROSS_GAME_KILL_USER,						// cross����game����
		MT_CROSS_GAME_HELLO,							// cross��game��������
		MT_GAME_CROSS_SYNC_CROSS_PERSON_RANK_INFO,		// ͬ���������а���Ϣ�����
		MT_GAME_CROSS_QUERY_ROLE_INFO_REQUEST,			// ��ѯ�����ɫ��Ϣ���� use locate game->cross
		MT_CROSS_GAME_GET_ROLE_INFO_REQUEST,			// ��ѯ�����ɫ��Ϣ���� cross->target locate gameworld
		MT_GAME_CROSS_USER_PROTOCAL_TRANSFER_TO_GAME,	// �û�Э�����ݴ�game��cross��game��ת����
		MT_CROSS_GAME_SYNC_CROSS_PERSON_RANK_TITLE_INFO, // ͬ��������а�ƺ���Ϣ
		MT_CROSS_GAME_UPDATE_CROSS_RECORD,				// ������ҿ����¼
		MT_CROSS_GAME_SYNC_ROLE_DATA,					// ͬ������������ݻ�ԭ��
		MT_CROSS_GAME_CHANNEL_CHAT,						// ����������ͬ����ԭ��
		MT_CROSS_GAME_RANK_INFO_REQ,					// �����Ҳ鿴ԭ�����а�
		MT_GAME_CROSS_RANK_INFO_ACK,					// �����Ҳ鿴ԭ�����а񷵻�
		MT_GAME_CROSS_SEND_MAIL_TO_USER,				// ������ʼ���ԭ�������
		MT_CROSS_GAME_SEND_MAIL_TO_USER,				// ������ʼ���ԭ�������
		MT_GAME_CROSS_GM_ADD_WEEK,						// gm�������
		MT_GAME_CROSS_GM_OPERA,							// gm����
		MT_GAME_CROSS_SYNC_ROLE_NUM_TO_GAME,			// gameͬ���������ȼ���¼��ɫ������game
		MT_CROSS_GAME_TIME_INFO_TO_HIDDEN,				// CROSS����gameʱ����Ϣ�����ط�

		MT_GAME_CROSS_WORLD_COMMON_DATA_CHANGE,			// ȫ�����ݱ仯 (���ط�->���)
		MT_CROSS_GAME_WORLD_COMMON_DATA_CHANGE,			// ȫ�����ݱ仯 (���->ԭ��)

		MT_GAME_CROSS_FORBID_TALK,						// ����(origin->cross)
		MT_CROSS_GAME_FORBID_TALK,						// ���ԣ�cross->hidden��
		/////////////////////////////////////////////  ���1v1 /////////////////////////////////////////////
		MT_CROSS_GAME_1V1_GOTO_SCENE,					// ���1V1���볡��
		MT_CROSS_GAME_1V1_INFO,							// ͬ�����1V1�����Ϣ
		MT_CROSS_GAME_1V1_SYNC_MATCH_INFO,				// ͬ�����1v1��Ϣ��������Ϸ��

		MT_GAME_CROSS_1V1_MATCH_REQ,					// ��Ϸ�����1v1����ƥ��
		MT_GAME_CROSS_1V1_FIGHT_RESULT,					// ͬ�����1v1������������			
		MT_GAME_CROSS_1V1_ROLE_RANK_BASE_INFO,			// ���¿��1v1���а������Ϣ
		MT_GAME_CROSS_1V1_RANK_LIST,					// ͬ�����1v1���а�
		MT_CROSS_GAME_CROSS_1V1_SCORE_RANK_REWARD,		// ���1v1�������н���
		MT_GAME_CROSS_1V1_MATCH_RESULT_REQ,				// ��Ϸ�����1v1����ƥ��״̬

		/////////////////////////////////////////////  ���3v3 /////////////////////////////////////////////
		MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_REQ,				// ���3V3����ƥ��
		MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_CANCEL,			// ȡ�����3V3ƥ��
		MT_CROSS_MULTIUSER_CHALLENGE_START_MATCH,				// ���3V3��ʼһ������
		MT_GAMECROSS_CROSS_MULTIUSER_CHALLENGE_MATCH_RESULT,	// ���3V3�������
		MT_GAME_CROSS_MULTIUSER_CHALLENGE_RANK_LIST,			// ���3V3ͬ�����а���Ϣ
		MT_CROSS_GAME_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD,	// ���3V3ȫ�����ְ���

		MT_SYNC_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE,		// ���3V3ͬ��ƥ��״̬
		MT_CROSS_GAME_MULTIUSER_CHALLENGE_GOTO_SCENE,			// ֪ͨ������3V3��������

		MT_CROSS_GAME_RANK_WEEK_REWARD_COMMAND,					// �������ÿ�ܽ���
		MT_GAME_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE,		// ��Ϸ����ѯ��ɫ���3V3ƥ��״̬

		/////////////////////////////////////////////  �����ս /////////////////////////////////////////////
		MT_CROSS_GAME_TUANZHAN_GOTO_SCENE,				// ���볡��
		MT_GAME_CROSS_TUANZHAN_SYNC_FIGHT_RESULT,		// ��ս�����Ϣ����cross

		//////////////////////////////////////////// ������� ///////////////////////////////////////////////
		MT_CROSS_GAME_CROSS_ACTIVITY_GOTO_SCENE,        // ������볡��(ͳһ����������Э��)
		MT_GAME_CROSS_PASTURE_INFO_SYNC,				// ���ũ�������Ϣͬ����ԭ��

		/////////////////////////////////////////////  ���BOSS /////////////////////////////////////////////
		MT_GAME_CROSS_BOSS_SYNC_PLAYER_INFO,			// ���boss������Ϣ����cross������ԭ����
		MT_GAME_GAME_BOSS_CREATE_BOSS_BROADCAST,		// ���boss֪ͨԭ����boss���ɣ����ط�->ԭ����
		MT_GAME_GAME_BOSS_INFO_REQ,						// ���boss��Ϣ��ѯ��ԭ��->���ط���

		/////////////////////////////////////////////  ���ͨ����Ϣ /////////////////////////////////////////////
		MT_GAME_CROSS_COMMON_INFO_SYNC,					// ���Ԫ����Ϣͬ��
		MT_GAME_CROSS_COMMON_OPEAR_REQ_CS,				// ���ͳһ����Э��

		/////////////////////////////////////////////  ����������Ϣ /////////////////////////////////////////////
		MT_GAME_CROSS_COMMON_ATTEND_INFO_SYNC,					//  ����������Ϣ����ԭ��

		/////////////////////////////////////////////  ������ ////////////////////////////////////////////////
		MT_SYNC_CROSS_TEAM_FB_USER_INFO,				// ͬ���������û���Ϣ�����->���ط���
		MT_CROSS_TEAM_FB_USER_OPTION_INFO,				// �������û�������ԭ��->�����
		MT_CROSS_TEAM_FB_ROOM_OPTION_INFO,				// �����ӷ��������ԭ��->�����
		
		/////////////////////////////////////////////  �������ս ////////////////////////////////////////////////												/////////////////////////////////////////////  �������ս /////////////////////////////////////////////
		MT_GAME_CROSS_GUILD_BATTLE_INFO_SYNC,			// ͬ���������ս��Ϣ�� ���ط������
		MT_CROSS_GAME_GUILD_BATTLE_INFO_SYNC,			// ͬ���������ս��Ϣ�� ���ط���ԭ��
		MT_GAME_CROSS_GUILD_BATTLE_RANK_LOG_SYNC,		// ͬ���������ս���������������棨���ط� -> �����
		MT_GAME_CROSS_GET_GUILD_BATTLE_RANK_LOG_REQ,	// �����ȡ�������վ������������ݣ���Ϸ�� -> �����
		MT_CROSS_GAME_GET_GUILD_BATTLE_RANK_LOG_RESP,	// �����ȡ�������վ������������ݣ���� -> ��Ϸ����
		MT_CROSS_GAME_GET_GUILD_BATTLE_BOSS_INFO_REQ,	// �����ȡ�������վboss��Ϣ����� -> ��Ϸ����
		MT_CROSS_GAME_GET_GUILD_BATTLE_MONSTER_INFO_REQ,// �����ȡ�������վ��Ӣ����Ϣ����� -> ��Ϸ����
		MT_CROSS_GAME_GUILD_BATTLE_SPECIAL_TIME_NOTICE,	// ��������ʱ������(���->��Ϸ��)
		MT_GAME_CROSS_GUILD_BATTLE_SYNC_DROP_INFO,		// ͬ���������ս������Ϣ
		MT_CROSS_GAME_GUILD_BATTLE_USER_SCORE_RANK,		// ͬ���û���������(���->��Ϸ��)
		MT_CROSS_GAME_GUILD_BATTLE_REGISTER_SYNC,		// ���������Ϣͬ��(���->���ط�)
		MT_GAME_GAME_GUILD_BATTLE_REGISTER_SYNC,		// ���������Ϣͬ��(���ط�->ԭ��)

		/////////////////////////////////////////////  ������� /////////////////////////////////////////////
		MT_GAME_CROSS_FISHING_INFO_SYNC,				// ͬ�����������Ϣ�����ط� -> �����
		MT_CROSS_GAME_FISHING_INFO_SYNC,				// ͬ�����������Ϣ����� -> ������

		/////////////////////////////////////////////  �������ȼ�  /////////////////////////////////////////////
		MT_CROSS_GAME_CROSS_WORLD_LEVEL_INFO,					// �������ȼ� ���->ԭ��

		/////////////////////////////////////////////  �������ش� ////////////////////////////////////////////////
		MT_GAME_CROSS_LIEKUN_FB_INFO_SYNC,				// �������ش�������Ϣ��ԭ��->���)
		MT_GAME_CROSS_LIEKUN_FB_KILL_ZONE_BOSS_SYNC,	// �������ش���ɱ����boss��Ϣ�����ط�->�����
		MT_CROSS_GAME_LIEKUN_FB_KILL_ZONE_BOSS_SYNC,	// �������ش���ɱ����boss��Ϣ�����->ԭ����
		MT_CROSS_GAME_LIEKUN_FB_TOP_REWARD_SYNC,		// �������ش����Ž�����Ϣ�����->ԭ����
		MT_GAME_CROSS_LIEKUN_FB_MSG_INFO_SYNC,			// �������ش�������Ϣ�����ط�->�����
		MT_CROSS_GAME_LIEKUN_FB_MSG_INFO_SYNC,			// �������ش�������Ϣ�����->ԭ����

		/////////////////////////////////////////////  ����Ҷ�ս�� /////////////////////////////////////////////
		MT_GAME_CROSS_MESS_BATTLE_INFO_SYNC,				// ͬ�����������Ϣ�����ط� -> �����
		MT_CROSS_GAME_MESS_BATTLE_INFO_SYNC,				// ͬ�����������Ϣ����� -> ������

		/////////////////////////////////////////////  ����Ҷ�ս�� /////////////////////////////////////////////
		MT_GAME_CROSS_NIGHT_FIGHT_INFO_SYNC,				// ͬ�����������Ϣ�����ط� -> �����
		MT_CROSS_GAME_NIGHT_FIGHT_INFO_SYNC,				// ͬ�����������Ϣ����� -> ������

		/////////////////////////////////////////////  ������ʱδ�õ�  ///////////////////////////////////////////////////////////
		MT_GAMECROSS_CROSS_CHALLENGE_MATCH_RESULT,		// �������һ����������ϴ���cross
		MT_CROSSGAME_CROSS_CHALLENGE_START_MATCH,		// ������Կ�ʼһ������
		MT_GAMECROSS_GM_CMD,							// ��Ϸ������cross��GM����
		MT_GAMECROSS_SYNC_CHALLENGE_PARAM,				// ��Ϸ��ͬ��ս�����ݵ�cross
		MT_CROSS_TEAMRACE_MATCHING_REQ,					// �����Ӿ�������ƥ��
		MT_CROSS_TEAMRACE_MATCHING_CANCEL,				// ȡ�������Ӿ���ƥ��
		MT_CROSS_TEAMRACE_START_MATCH,					// �����Ӿ��ٿ�ʼһ������
		MT_CROSS_TEAMRACE_MATCH_RESULT,					// �����Ӿ��ٱ������

		/////////////////////////////////////////////	������а�	////////////////////////////////////////////
		MT_CROSS_GAME_SYNC_RANK_FIRST,							// ������˰񣨿�� -> ������
		MT_CROSS_GAME_SYNC_COUPLE_RANK_FIRST,					// ������°񣨿�� -> ������
		MT_GAME_CROSS_SYNC_COUPLE_RANK_DIVORCE,					// ������°���飨���� -> �����
		MT_GAME_CROSS_SYNC_CROSS_COUPLE_RANK_INFO, 		        // ͬ���������а���Ϣ�����
		MT_CROSS_GAME_SYNC_CROSS_COUPLE_FORCE_SNAP,		        // ǿ��ͬ���������а���Ϣ


		/////////////////////////////////////////////  �����ͨboss ////////////////////////////////////////////////	
		MT_CROSS_GAME_CROSS_COMMON_BOSS_INFO_CHANGE,	        // �����ͨboss��Ϣ���
		MT_CROSS_GAME_CROSS_COMMON_BOSS_SYNC_INFO_REQ,	        // �����ͨboss��Ϣͬ������
		MT_CROSS_GAME_CROSS_COMMON_BOSS_SYNC_INFO,		        // �����ͨboss��Ϣͬ��
		MT_CROSS_GAME_CROSS_COMMON_BOSS_DROP_INFO,              // �����ͨboss����

		/////////////////////////////////////////////  ����  ///////////////////////////////////////////////////////////
		MT_USE_GOLD_IN_HIDDEN_GS,								// �����ط�ʹ����Ԫ����֪ͨԭ��

		MT_CROSS_GAME_SINGLE_CHAT,								// ������˽��ͬ����ԭ��
		MT_CROSS_GAME_LOCAL_SPEAKER,							// �����ҷ��������Ȼ�ԭ��
		MT_CROSS_GAME_SYSTEM_MSG_TO_ORIGIN,						// ���ط�ͬ��ϵͳ��Ϣ��ԭ��(���ط������ʹ����ת��Э�飬�������ֻ�Ǵӿ����ԭ��)
		MT_CROSS_GAME_GUILD_CHAT,								// ���ط��������ͱ�����������

		MT_GAME_GAME_CROSS_GUILD_MEMBER_BE_KILL,				// �������˳�Ա����ɱ�����ط�->ԭ����
		MT_CROSS_GAME_GET_GUILD_ENEMY_LIST,						// �����һ�ȡ���˳����б����ط�->ԭ����

		/////////////////////////////////////////// ������ ////////////////////////////////////////////////////////////////////
		MT_CROSS_GAME_SYNC_CROSS_RAND_ACTIVITY_DATA,			// ͬ������������ݣ����->ԭ����
		MT_GAME_CROSS_SYNC_CROSS_RAND_ACTIVITY_DATA,			// ͬ������������ݣ�ԭ��->�����

		/////////////////////////////////////////// �����ټ� ////////////////////////////////////////////////////////////////////
		MT_GAME_GAME_CROSS_GUILD_BATTLE_SOS,					// ͬ���ټ���Ϣ�����ط�->ԭ����
																				
		/////////////////////////////////////////// ��ɱ����boss ////////////////////////////////////////////////////////////////////
		MT_GAME_GAME_KILL_CROSS_BOSS,							// ͬ����ɱ����boss��ԭ��->�����

		/////////////////////////////////////////////  �����ط���ȡ������ϡ��Ʒ  ///////////////////////////////////////////////////////////
		MT_PICK_GUILD_RARE_ITEM_IN_HIDDEN_GS,					// �����ط���ȡ������ϡ��Ʒ��֪ͨԭ��
		MT_BROADCAST_GUILD_RARE_INFO,							// �㲥������ϡ

		/////////////////////////////////////////////  ������а���Ϣ��ԭ��  ///////////////////////////////////////////////////////////
		MT_SYNC_CROSS_RANK_INFO,								// ͬ�������ǰ����ԭ���������������Ƶ�����ţ�

		/////////////////////////////////////////// ����߾�֮�� ////////////////////////////////////////////////////////////////////
		MT_HIDDEN_CROSS_BIANJINGZHIDI_BOSS_INFO,				// ͬ������߾�֮��boss�����ط�->��CROSS��

		/////////////////////////////////////////// ��������� ////////////////////////////////////////////////////////////////////
		MT_SYNC_CROSS_CHALLENGEFIELD_REQ,						// ͬ��������������ݣ����-ԭ����
		MT_SYNC_CROSS_CHALLENGEFIELD_DATA_INFO,					// ͬ��������������ݷ���
		
		/////////////////////////////////////////// �������ˮ�� ////////////////////////////////////////////////////////////////////
		MT_CROSS_HUSONG_SHUIJING_GATHER_INFO_REQ,				// �������ˮ��(ԭ��->���ط�)
 	};

	struct MessageHeader
	{
		MessageHeader(): msg_type(MT_INVALID) {}
		MessageHeader(int _msg_type): msg_type(_msg_type) {}
		int	msg_type;
	};

	struct CrossGameSyncCrossUser
	{
		CrossGameSyncCrossUser() : header(MT_CROSS_GAME_SYNC_CROSS_USER) {}
		MessageHeader	header;

		enum 
		{
			SYNC_CROSS_USER_INIT = 0,
			SYNC_CROSS_USER_ADD,
			SYNC_CROSS_USER_REMOVE,
		};

		static const int MAX_IN_CROSS_USER_NUM = 2048;

		struct CrossUserItem
		{
			int			uid;
			int			cross_activity_type;
		};

		int				sync_type;
		int				cross_type;  // crossserver��ս������
		int				count;
		CrossUserItem	in_cross_user_list[MAX_IN_CROSS_USER_NUM];
	};

	struct GameCrossStartCrossReq
	{
		GameCrossStartCrossReq() : header(MT_GAME_CROSS_START_CROSS_REQ) {}
		MessageHeader	header;

		static const int MAX_ROLE_CROSS_PARAM_DATA_LEN = 4 * 1024 * 1024;

		int				is_force;
		int				cross_activity_type;
		int				plat_type;
		PlatName		pname;
		int				uid;
		int				role_cross_param_data_len;
		char			role_cross_param_data[MAX_ROLE_CROSS_PARAM_DATA_LEN];
	};

	struct GameCrossRegisterGameServer
	{
		GameCrossRegisterGameServer() : header(MT_GAME_CROSS_REGISTER_GAME_SERVER) {}
		MessageHeader		header;

		static const int MAX_SERVER_ID_NUM = 512;

		int					cross_protocal_ver;
		int					plat_type;
		unsigned int		server_start_time;
		unsigned int		server_combine_time;
		UInt32				role_all_param_size;
		UInt32				server_version;

		int					count;
		int					server_id_list[MAX_SERVER_ID_NUM];
	};

	struct CrossGameSaveCrossRoleInfo
	{
		CrossGameSaveCrossRoleInfo() : header(MT_CROSS_GAME_SAVE_CROSS_ROLE_INFO) {}
		MessageHeader		header;

		static const int MAX_ROLE_CROSS_PARAM_DATA_LEN = 4 * 1024 * 1024;

		int				cross_activity_type;
		int				original_plat_type;
		int				original_uid;
		GameName		original_username;
		PlatName		pname;
		int				role_cross_param_data_len;
		char			role_cross_param_data[MAX_ROLE_CROSS_PARAM_DATA_LEN];
	};

	struct GameCrossSaveCrossRoleRet
	{
		GameCrossSaveCrossRoleRet() : header(MT_GAME_CROSS_SAVE_CROSS_ROLE_RET) {}
		MessageHeader	header;

		int				result;

		int				cross_activity_type;
		int				original_plat_type;
		int				original_uid;
		GameName		original_username;

		PlatName		pname;
		int				uid;

		char			login_server_ip[64];
		int				login_server_port;
		unsigned int	login_time;
		LoginStr		login_str;
		char			anti_wallow;
		char			reserved1;
		short			server;
	};

	struct CrossGameStartCrossAck
	{
		CrossGameStartCrossAck() : header(MT_CROSS_GAME_START_CROSS_ACK) {}
		MessageHeader	header;

		int				result;
		int				original_uid;
		int				cross_activity_type;
		char			login_server_ip[64];
		int				login_server_port;
		PlatName		pname;
		unsigned int	login_time;
		LoginStr		login_str;
		char			anti_wallow;
		char			reserved1;
		short			server;
	};

	struct CrossGameUserMsg
	{
		CrossGameUserMsg() : header(MT_CROSS_GAME_USER_MSG) {}
		MessageHeader		header;

		int					uid;
		// ������� Protocol Э������
	};

	struct CrossGameAllUserMsg
	{
		CrossGameAllUserMsg() : header(MT_CROSS_GAME_ALL_USER_MSG) {}
		MessageHeader		header;

		int					cross_activity_type;
		// ������� Protocol Э������
	};

	struct GameCrossUserMsg
	{
		GameCrossUserMsg() : header(MT_GAME_CROSS_USER_MSG) {}
		MessageHeader		header;

		long long			uuid;
		int					merge_server_id;
		// ������� Protocol Э������
	};

	struct GameCrossUserLogin
	{
		GameCrossUserLogin() : header(MT_GAME_CROSS_USER_LOGIN) {}
		MessageHeader	header;

		int				uid;

		int				original_plat_type;
		int				original_uid;
		GameName		original_user_name;
	};

	struct GameCrossUserLogout
	{
		GameCrossUserLogout() : header(MT_GAME_CROSS_USER_LOGOUT) {}
		MessageHeader	header;

		int				uid;
	};

	struct GameCrossKickCrossUser
	{
		GameCrossKickCrossUser() : header(MT_GAME_CROSS_KICK_CROSS_USER) {}
		MessageHeader	header;

		long long		bekick_original_uuid;
	};

	enum CROSS_TRANSFER_TYPE
	{
		CROSS_TRANSFER_TYPE_HIDDEN,			// ת�����ط�
		CROSS_TRANSFER_TYPE_COMMON,			// ת����ͨ��
		CROSS_TRANSFER_TYPE_ALL,			// ת�����з�
		CROSS_TRANSFER_TYPE_OTHER,			// ת��������
		CROSS_TRANSFER_TYPE_BY_UUID,		// ת��ָ����
		CROSS_TRANSFER_TYPE_BY_GLOBAL_NETID,// ת��ָ����--global_netid
	};
	struct GameCrossRransferToGame
	{
		GameCrossRransferToGame() : header(MT_GAME_CROSS_TRANSFER_TO_GAME) {}
		MessageHeader	header;

		int				length;
		int				transfer_type;
		long long		param;
	};
	
	struct CrossGameSyncActivityStatus
	{
		CrossGameSyncActivityStatus() : header(MT_CROSS_GAME_SYNC_CROSS_ACTIVITY_STATUS) {}
		MessageHeader	header;

		int				cross_activity_type;
		int				activity_status;
		unsigned int	status_begin_time;
		unsigned int	status_end_time;
		int				activity_special_param;
		unsigned int	cross_now_scecond;
	};

	//ȫ�����ݻش�
	enum WORLD_COMMONDATA_CHANGE_TYPE
	{
		WORLD_COMMONDATA_CHANGE_TYPE_INVALID,

		WORLD_COMMONDATA_CHANGE_TYPE_1V1,					// ���1v1����
		WORLD_COMMONDATA_CHANGE_TYPE_3V3,					// ���3v3����

		WORLD_COMMONDATA_CHANGE_TYPE_CHENGZHU_CHANGE,		// �����ı�(ԭ���������ط�)
		WORLD_COMMONDATA_CHANGE_TYPE_COMBINE_KILL_BOSS,		// �Ϸ�BOSS����
		WORLD_COMMONDATA_CHANGE_TYPE_GUILD_GONGZI,			// ���˹���
		WORLD_COMMONDATA_CHANGE_TYPE_SINGLE_CHAT_LISTEN,	// ˽�ļ���
		WORLD_COMMONDATA_CHANGE_TYPE_GUILD_KILL_BOSS,		// ������˻�ɱ��
		WORLD_COMMONDATA_CHANGE_TYPE_CROSS_CHALLENGEFIELD,	// �������������
	};

	enum WORLD_COMMONDATA_SUB_TYPE
	{
		WORLD_COMMONDATA_SUB_TYPE_INVALID,

		WORLD_COMMONDATA_SUB_TYPE_RANK,               
		WORLD_COMMONDATA_SUB_TYPE_SEANSON,				// �¿�����
		WORLD_COMMONDATA_SUB_TYPE_RANK_MIN_SCORE,		// ����������ͷ֣� ���һ����Ҷ�Ӧ�Ļ��֣�����ԭ������½��ж��Ƿ����λ������������Ҳ��ø���λ������
		WORLD_COMMONDATA_SUB_TYPE_INIT,					// ���ӿ���ɹ���ʼ��
	};

	struct GameCrossWorldCommonDataChange     // ���ط�->���
	{
	public:
		GameCrossWorldCommonDataChange() : header(MT_GAME_CROSS_WORLD_COMMON_DATA_CHANGE) {}

		MessageHeader	header;

		long long uuid;

		short change_type;
		short sub_type;
		int param_2;
		int param_3;
		int param_4;
	};

	struct CrossGameWorldCommonDataChange       // ���->ԭ��(ȫ�����ݱ��)
	{
	public:
		CrossGameWorldCommonDataChange() : header(MT_CROSS_GAME_WORLD_COMMON_DATA_CHANGE) {}

		MessageHeader	header;

		void Reset()
		{
			change_type = 0;
			sub_type = 0;
			param_2 = 0;
			param_3 = 0;
			param_4 = 0;
		}

		long long uuid;

		short change_type;
		short sub_type;
		int param_2;
		int param_3;
		int param_4;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	struct CrossGameXiuluotaGotoScene
	{
		CrossGameXiuluotaGotoScene() : header(MT_CROSS_GAME_XIULUOTA_GOTO_SCENE) {}
		MessageHeader	header;

		int cross_uid;
	};

	struct GameCrossActivityForceNextState
	{
		GameCrossActivityForceNextState() : header(MT_GAME_CROSS_ACTIVITY_FORCE_NEXTSTATE) {}
		MessageHeader	header;

		int activity_type;
	};

	static const int XIULUO_TOWER_USER_RESULT_MAX = 1024;
	struct GameCrossSyncXiuluoTowerResult
	{
		GameCrossSyncXiuluoTowerResult() : header(MT_GAME_CROSS_SYNC_XIULUOTOWER_RESULT) {}
		MessageHeader	header;

		struct UserResult
		{
			long long uuid;
			char max_layer;
			char rank_pos;
			short kill_role_count;
			short kill_boss_count;
			short user_level;
			int reward_cross_honor;
		};

		int user_result_count;
		UserResult user_result_list[XIULUO_TOWER_USER_RESULT_MAX];
	};

	struct CrossGameSyncXiuluoTowerUserResult
	{
		CrossGameSyncXiuluoTowerUserResult() : header(MT_CROSS_GAME_SYNC_XIULUOTOWER_USER_RESULT) {}
		MessageHeader	header;

		long long		uuid;
		char			max_layer;
		char			rank_pos;
		short			kill_role_count;
		short			kill_boss_count;
		short			user_level;
		int				reward_cross_honor;
	};

	enum XIULUOTOWER_INFO_TYPE
	{
		XIULUOTOWER_INFO_TYPE_BUFF = 0,					// BUFF�����ʼ�
		XIULUOTOWER_INFO_TYPE_FIRST,					// ��ͨ�ʼ�
	};

	struct GameCrossSyncXiuluoTowerInfo
	{
		GameCrossSyncXiuluoTowerInfo() : header(MT_GAME_CROSS_SYNC_XIULUOTOWER_INFO) {}
		MessageHeader	header;

		long long	uuid;
		int			type;
		int			param;
	};

	struct CrossGameCrossSyncXiuluoTowerInfo
	{
		CrossGameCrossSyncXiuluoTowerInfo() : header(MT_CROSS_GAME_CROSS_SYNC_XIULUOTOWER_INFO) {}
		MessageHeader	header;

		long long	uuid;
		int			type;
		int			param;
	};

	struct GameCrossSyncXiuluoTowerDropInfo
	{
		GameCrossSyncXiuluoTowerDropInfo() : header(MT_GAME_CROSS_SYNC_DROP_INFO) {}
		MessageHeader header;

		long long uuid;
		int log_type;
		GameName name;
		unsigned int timestamp;
		unsigned short item_id;
		short item_num;
	};

	struct GameCrossSyncXiuluoTowerRankTitle
	{
		GameCrossSyncXiuluoTowerRankTitle() :header(MT_GAME_CROSS_SYNC_XIULUOTOWER_RANK_TITLE) {}
		MessageHeader header;

		long long uuid;
		int rank;
	};

	struct CrossGameSyncXiuluoTowerRankTitle
	{
		CrossGameSyncXiuluoTowerRankTitle():header(MT_CROSS_GAME_SYNC_XIULUOTOWER_RANK_TITLE){}
		MessageHeader header;

		int uid;
		int rank;
	};

	struct GameCrossSyncGuildBattleDropInfo
	{
		GameCrossSyncGuildBattleDropInfo() : header(MT_GAME_CROSS_GUILD_BATTLE_SYNC_DROP_INFO) {}
		MessageHeader header;

		long long uuid;
		int log_type;
		GameName name;
		unsigned int timestamp;
		unsigned short item_id;
		short item_num;
	};

	struct CrossGameKickUser
	{
		CrossGameKickUser() : header(MT_CROSS_GAME_KILL_USER) {}
		MessageHeader	header;

		int				bekick_uid;
		long long		bekick_uuid;
	};

	static const int SYNC_CROSS_PERSON_RANK_INFO_SIZE = 1024;

	struct GameCrossSyncCrossPersonRankInfo
	{
		GameCrossSyncCrossPersonRankInfo() : header(MT_GAME_CROSS_SYNC_CROSS_PERSON_RANK_INFO) {}
		MessageHeader	header;

		int plat_type;
		int role_id;
		GameName name;
		int level;
		char prof;
		char sex;
		char camp;
		char vip_level;
		long long exp;
		// RoleAppearance appearance;

		struct SyncCrossPersonRankInfoItem
		{
			int rank_type;
			long long rank_value;
			int record_index;
			long long flexible_ll;
			int flexible_int;
			GameName flexible_name;
		};

		void ResetItems()
		{
			item_count = 0;
			memset(items, 0, sizeof(items));
		}

		int item_count;
		SyncCrossPersonRankInfoItem items[SYNC_CROSS_PERSON_RANK_INFO_SIZE];
	};

	struct GameCrossQueryRoleInfoRequest
	{
		enum QueryRoleInfoType
		{
			QUERY_ROLE_INFO_TYPE_ALL,
			QUERY_ROLE_INFO_TYPE_UUID_NAME,
		};
		GameCrossQueryRoleInfoRequest() : header(MT_GAME_CROSS_QUERY_ROLE_INFO_REQUEST) {}
		MessageHeader	header;

		int target_plat_type;
		int target_uid;

		int request_plat_type;
		int request_uid;

		int request_type;
		int param;
	};

	struct CrossGameGetRoleInfoRequest
	{
		CrossGameGetRoleInfoRequest() : header(MT_CROSS_GAME_GET_ROLE_INFO_REQUEST) {}
		MessageHeader	header;

		int target_uid;
		int request_plat_type;
		int request_uid;
		int request_type;
		int param;
	};

	struct GameCrossUserProtocalTransferToGame
	{
		GameCrossUserProtocalTransferToGame() : header(MT_GAME_CROSS_USER_PROTOCAL_TRANSFER_TO_GAME) {}
		MessageHeader		header;

		long long			uuid;
		int					protocal_len;

		// ������� Protocol Э������
	};

	struct CrossGameHello
	{
		CrossGameHello() : header(MT_CROSS_GAME_HELLO) {}
		MessageHeader	header;
	};

	struct CrossGameSyncCrossPersonRankTitleInfo
	{
		CrossGameSyncCrossPersonRankTitleInfo() : header(MT_CROSS_GAME_SYNC_CROSS_PERSON_RANK_TITLE_INFO) {}
		MessageHeader header;

		int rank_type;
		int uuid_count;
		long long rank_title_user_uuid_list[CROSS_PERSON_RANK_MAX_SIZE];
	};

	struct CrossGameUpdateCrossRecord
	{
		CrossGameUpdateCrossRecord() : header(MT_CROSS_GAME_UPDATE_CROSS_RECORD) {}
		MessageHeader	header;

		enum
		{
			UPDATE_CROSS_RECORD_INVALID = 0,
			UPDATE_CROSS_RECORD_CROSS_1V1,
			UPDATE_CROSS_RECORD_CROSS_MULTIUSER_CHALLENGE,
			UPDATE_CROSS_RECORD_CROSS_TUANZHAN,
			UPDATE_CROSS_RECORD_CROSS_PASTURE,
			UPDATE_CROSS_RECORD_CROSS_BOSS,
			UPDATE_CROSS_RECORD_CROSS_COMMON,
			UNDATE_CROSS_RECORD_CROSS_ATTEND,
			UPDATE_CROSS_RECORD_CROSS_MIZANG_BOSS,
			UPDATE_CROSS_RECORD_CROSS_YOUMING_BOSS,

			UPDATE_CROSS_RECORD_MAX
		};

		static const int CROSS_RECORD_PARAM_MAX = 16;

		int				uid;
		int				update_cross_record_type;
		int				record_param_list[CROSS_RECORD_PARAM_MAX];
		RoleAppearance	appearance;
		
	};

	struct CrossGameSyncRoleData
	{
		CrossGameSyncRoleData() : header(MT_CROSS_GAME_SYNC_ROLE_DATA) {}
		MessageHeader header;

		int				is_logout;
		long long		uuid;
		
		static const int MAX_DATA_LEN = 1024 * 1024 * 4;
		int				data_len;
		char			role_all_data[MAX_DATA_LEN];
	};

	struct CrossGameChannelChat
	{
		CrossGameChannelChat() : header(MT_CROSS_GAME_CHANNEL_CHAT) {}
		MessageHeader header;

		long long		uuid;
		short			channel_type;
		short			role_camp;
		int				guild_id;
		int				protocal_len;

		// ������� Protocol Э������
	};

	// �����˽��ͬ����ԭ��
	struct CrossGameSingleChat
	{
		CrossGameSingleChat() : header(MT_CROSS_GAME_SINGLE_CHAT) {}
		MessageHeader header;

		long long to_uuid;
	};

	struct CrossGameSpeaker
	{
		CrossGameSpeaker() : header(MT_CROSS_GAME_LOCAL_SPEAKER) {}
		MessageHeader header;
	};

	struct CrossGameRankInfoReq
	{
		CrossGameRankInfoReq() : header(MT_CROSS_GAME_RANK_INFO_REQ) {}
		MessageHeader header;

		long long		uuid;
		short			is_personal_rank;
		short			is_personal_rank_top_user;
		int				rank_type;
	};

	struct GameCrossRankInfoAck
	{
		GameCrossRankInfoAck() : header(MT_GAME_CROSS_RANK_INFO_ACK) {}
		MessageHeader header;

		long long		uuid;

		// ������� Protocol Э������
	};

	////////////////////////////////////����ʼ�//////////////////////////////////////
	struct GameCrossSendMailToUser
	{
	public:
		GameCrossSendMailToUser() : header(MT_GAME_CROSS_SEND_MAIL_TO_USER) {}
		MessageHeader header;

		long long unique_user_id;
		MailContentParam contentparam;
	};

	struct CrossGameSendMailToUser
	{
	public:
		CrossGameSendMailToUser() : header(MT_CROSS_GAME_SEND_MAIL_TO_USER) {}
		MessageHeader header;

		int user_id;
		MailContentParam contentparam;
	};

	////////////////////////////////////gm//////////////////////////////////////
	struct GameCrossGmAddWeek
	{
	public:
		GameCrossGmAddWeek() : header(MT_GAME_CROSS_GM_ADD_WEEK) {}
		MessageHeader header;
	};

	struct GameCrossGmOpera
	{
	public:
		GameCrossGmOpera() : header(MT_GAME_CROSS_GM_OPERA) {}
		MessageHeader header;

		int opera_type;
		int param_1;
		int param_2;
	};

	///////////////////////////////////// ͬ����ɫ /////////////////////////////////////
	struct GameCrossSyncRoleNumToGame
	{
	public:
		GameCrossSyncRoleNumToGame() : header(MT_GAME_CROSS_SYNC_ROLE_NUM_TO_GAME) {}
		MessageHeader header;

		short role_num_list[MAX_ROLE_LEVEL + 1];
		short reserve_sh;
		long long unique_serverId;
	};

	////////////////////////////////////// ͬ��ʱ�� ////////////////////////////////////
	struct CrossGameTimeInfoToHidden
	{
		CrossGameTimeInfoToHidden() : header(MT_CROSS_GAME_TIME_INFO_TO_HIDDEN) {}
		MessageHeader		header;

		struct UniqueGameSerInfo
		{
			UniqueGameSerInfo() : server_start_time(0), server_combine_time(0), server_start_day_index(0) {}
			
			UniqueServerID gs_unique_id;
			unsigned int server_start_time;
			unsigned int server_combine_time;
			int server_start_day_index;
		};

		int					count;
		UniqueGameSerInfo	server_info_list[GameCrossRegisterGameServer::MAX_SERVER_ID_NUM];
	};

	/////////////////////////////////////////////  ���1v1 /////////////////////////////////////////////
	struct CrossGame1V1GotoScene	// 29043 ���볡��
	{
		CrossGame1V1GotoScene() : header(MT_CROSS_GAME_1V1_GOTO_SCENE) {}
		MessageHeader header;

		long long uuid;
		UserID cross_uid;
	};

	struct CrossGame1V1Info			// 29044 ��Ϣ
	{
		CrossGame1V1Info() : header(MT_CROSS_GAME_1V1_INFO) {}
		MessageHeader header;

		int cross_uid;
		int cross_score_1v1;
		int cross_day_join_1v1_count;
		short reserve_sh;
		char cross_1v1_xiazhu_seq;
		char cross_1v1_fetch_xiazhu_reward_flag;
		short cross_1v1_xiazhu_gold;
	};

	struct GameCrossActivity1V1MatchReq	// 29046����ƥ��
	{
		GameCrossActivity1V1MatchReq() : header(MT_GAME_CROSS_1V1_MATCH_REQ) {}
		MessageHeader	header;

		long long		uuid;
		char			prof;
		char			sex;
		char			camp;
		char			reserve_ch;
		short			level;
		short			jingjie;			// ����ȼ�
		int				score;
		int				day_join_count;		// ÿ�ղμӴ���
		long long		capability;
		GameName		name;
		RoleAppearance	appearance;
		int				dur_win_count;		// ��ʤ����
		short           total_join_times;	// �ܲμӴ���
		short           total_win_times;	// ��ʤ������
		int             cross_1v1_gongxun;	// ��ѫ
	};

	struct GameCrossActivity1V1RoleRankBaseInfo		// 29048 �������а������Ϣ
	{
		GameCrossActivity1V1RoleRankBaseInfo() : header(MT_GAME_CROSS_1V1_ROLE_RANK_BASE_INFO) {}
		MessageHeader	header;

		long long		uuid;
		int				level;
		long long		capability;
	};

	struct CrossGameSyncCross1v1MatchInfo		// 29045
	{
	public:
		CrossGameSyncCross1v1MatchInfo() : header(MT_CROSS_GAME_1V1_SYNC_MATCH_INFO) {}
		MessageHeader header;

		static const int MAX_FIGHT_UNIT_COUNT = 1024;

		unsigned int m_next_match_left_time;
		int unit_count;
		CrossActivity1v1FightUnit fight_unit_list[MAX_FIGHT_UNIT_COUNT];
	};

	struct GameCrossCross1V1FightResult			// 29047
	{
	public:
		GameCrossCross1V1FightResult() : header(MT_GAME_CROSS_1V1_FIGHT_RESULT) {}
		MessageHeader header;

		long long uuid1;
		long long uuid2;
		short user1_is_win;
		short user2_is_win;
	};

	static const int MAX_CROSS_1V1_RANK_COUNT = 1024;

	struct GameCrossCross1V1RankList
	{
		GameCrossCross1V1RankList() : header(MT_GAME_CROSS_1V1_RANK_LIST) {}
		MessageHeader	header;

		struct RankInfo
		{
			int plat_type;
			int role_id;
			GameName name;
			short level;
			char prof;
			char sex;
			int score;
			int dur_win_count;
			short win_percent;
			char jingjie;
			char reserve_ch;
			long long capability;
			RoleAppearance appearance;
		};

		int count;
		RankInfo rank_list[MAX_CROSS_1V1_RANK_COUNT];
	};

	struct CrossGameCross1V1ScoreRankReward
	{
		CrossGameCross1V1ScoreRankReward() : header(MT_CROSS_GAME_CROSS_1V1_SCORE_RANK_REWARD) {}
		MessageHeader	header;

		int				uid;
		int				rank_pos;
		int				score;
	};

	enum CROSS1V1_MATCH_RESULT_REQ_TYPE
	{
		CROSS1V1_MATCH_RESULT_REQ_TYPE_QUERY = 0,
		CROSS1V1_MATCH_RESULT_REQ_TYPE_CANCEL,
	};
	struct CrossGameCross1V1MatchResult		
	{
		CrossGameCross1V1MatchResult() : header(MT_GAME_CROSS_1V1_MATCH_RESULT_REQ) {}
		MessageHeader	header;

		long long		uuid;
		int req_type;
	};

	//////////////////////////////////////// ���3v3 //////////////////////////////////////////////////

	struct GameCrossCrossMuliuserChallengeMatchingReq
	{
		GameCrossCrossMuliuserChallengeMatchingReq() : header(MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_REQ) {}
		MessageHeader	header;

		struct MatchingUserInfo
		{
			long long	uuid;
			GameName	user_name;
			char		camp;
			char		sex;
			char		prof;
			char		reserved;
			short		level;
			int			capability;
			int			challenge_score;
			int			challenge_exp;
			int			win_percent;
			int			challenge_level;
			int			match_total_count;
			int			match_win_count;
			int			dur_win_count;
			int			mvp_count;
		};

		long long		req_uuid;
		int				count;
		MatchingUserInfo	user_info_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT];
	};

	struct GameCrossCrossMuliuserChallengeMatchingCancel
	{
		GameCrossCrossMuliuserChallengeMatchingCancel() : header(MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_CANCEL) {}
		MessageHeader	header;

		long long		uuid;
	};

	struct CrossGameCrossMultiuserChallengeStartMatch
	{
		CrossGameCrossMultiuserChallengeStartMatch() : header(MT_CROSS_MULTIUSER_CHALLENGE_START_MATCH) {}
		MessageHeader	header;

		struct MatchingUserInfo
		{
			long long	uuid;
			GameName	user_name;
			char		camp;
			char		sex;
			char		prof;
			char		select_skill_id;
			short		level;
			long long	capability;
			int			challenge_score;
			int			challenge_exp;
			int			win_percent;
			int			challenge_level;
			int			match_total_count;
			int			match_win_count;
			int			mvp_count;
		};

		int				match_key;
		MatchingUserInfo	user_info_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	};

	struct GameCrossCrossMultiuserChallengeMatchResult
	{
		GameCrossCrossMultiuserChallengeMatchResult() : header(MT_GAMECROSS_CROSS_MULTIUSER_CHALLENGE_MATCH_RESULT) {}
		MessageHeader	header;

		struct SyncUserInfo
		{
			long long	uuid;
			GameName	user_name;
			short		level;
			char		prof;
			char		sex;
			int			challenge_score;
			int			challenge_exp;
			int			challenge_level;
			int			match_total_count;
			int			match_win_count;
			int			dur_win_count;
			int			mvp_count;
			int			add_challenge_score;
			int			add_challenge_exp;
			short		is_mvp;
			char		jingjie;
			char		reserve_ch;
			int			add_honor;
			int			add_gongxun;
			long long	reward_add_exp;
		};

		int				match_key;
		int				win_side;
		SyncUserInfo 	sync_user_info_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	};

	static const int MAX_MULTIUSER_CHALLENGE_RANK_COUNT = 1024;

	struct GlobalCrossMultiuserChallengeRankList
	{
		GlobalCrossMultiuserChallengeRankList() : header(MT_GAME_CROSS_MULTIUSER_CHALLENGE_RANK_LIST) {}
		MessageHeader	header;

		struct RankInfo
		{
			int plat_type;
			int role_id;
			GameName name;
			short level;
			char sex;
			char prof;
			short match_total_count;
			char win_percent;
			char jingjie;
			int challenge_score;
			short challenge_level;
			short mvp_count;
			int capability;
		};

		int count;
		RankInfo rank_list[MAX_MULTIUSER_CHALLENGE_RANK_COUNT];
	};

	struct CrossGameMultiuserChallengeScoreRankReward
	{
		CrossGameMultiuserChallengeScoreRankReward() : header(MT_CROSS_GAME_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD) {}
		MessageHeader	header;

		int				uid;
		int				rank_pos;
		int				socre;
	};

	struct CrossGameMultiuserChallengeMatchingStateNotify  // ���3V3ͬ��ƥ��״̬
	{
		CrossGameMultiuserChallengeMatchingStateNotify() : header(MT_SYNC_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE) {};
		MessageHeader		header;

		int					cross_uid;
		int					matching_state;
		short				auto_start_cross;	// 1��0 �ܷ���������3v3������ս���е���������������Ҫ�Զ�����3v3����
		short				notice_match;		// 0,1,2,3 ��ʾƥ����ȴ����������ս������ң��ڱ���ս������ǰ�����ٴ�ƥ��
	};

	struct CrossGameMultiuserChallengeGotoScene
	{
		CrossGameMultiuserChallengeGotoScene() : header(MT_CROSS_GAME_MULTIUSER_CHALLENGE_GOTO_SCENE) {}
		MessageHeader		header;

		int					cross_uid;
		int					fb_key;
		int					side;
	};

	struct CrossGameRankWeekRewardCommand
	{
		CrossGameRankWeekRewardCommand() : header(MT_CROSS_GAME_RANK_WEEK_REWARD_COMMAND) {}
		MessageHeader header;

		struct Item
		{
			int plat_type;
			int uid;
			long long  rank_value;
		};

		int rank_type;
		int item_count;
		Item item_list[CROSS_RANK_WEEK_REWARD_MAX_ROLE_COUNT];
	};

	struct GameCrossCrossMuliuserChallengeGetMatchingState		// ��Ϸ����ѯ��ɫ���3V3ƥ��״̬
	{
		GameCrossCrossMuliuserChallengeGetMatchingState() : header(MT_GAME_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE) {}
		MessageHeader	header;

		long long		uuid;
	};

	//=========================================== �����ս  ===========================================
	struct CrossGameTuanzhanGotoScene		// 29042
	{
		CrossGameTuanzhanGotoScene() : header(MT_CROSS_GAME_TUANZHAN_GOTO_SCENE) {}
		MessageHeader	header;

		int cross_uid;
	};

	struct GameCrossTuanzhanSyncFightResult		// 29043
	{
		GameCrossTuanzhanSyncFightResult() : header(MT_GAME_CROSS_TUANZHAN_SYNC_FIGHT_RESULT) {}
		MessageHeader	header;

		long long uuid;														// ȫƽ̨ΨһID
		unsigned int score;													// ��û���
		short user_is_win;													// �Ƿ�ʤ����0 ʧ�ܣ� 1 ʤ��
		short side_rank;													// ������Ӫ������
	};

	struct GameCrossCommonInfoSync
	{
	public:
		GameCrossCommonInfoSync() : header(MT_GAME_CROSS_COMMON_INFO_SYNC) {}
		MessageHeader header;

		long long uuid;

		int has_use_gold;												// ��ǰ�ο�����ĵ�Ԫ����һ�����ڸ��
	};

	struct GameCrossCommonAttendInfoSync
	{
	public:
		GameCrossCommonAttendInfoSync() : header(MT_GAME_CROSS_COMMON_ATTEND_INFO_SYNC) {}
		MessageHeader header;

		long long uuid;

		int attend_activity_num;										// ����û�Ļ��
	};

	enum CROSS_COMMON_OPERA_REQ
	{
		CROSS_COMMON_OPERA_REQ_INVALID = 0,

		CROSS_COMMON_OPERA_REQ_CROSS_GUILDBATTLE_BOSS_INFO,					// ��������սBoss��Ϣ

		CROSS_COMMON_OPERA_REQ_MAX
	};

	struct CSCrossCommonOperaReq
	{
	public:
		CSCrossCommonOperaReq() : header(MT_GAME_CROSS_COMMON_OPEAR_REQ_CS) {}
		MessageHeader	header;

		int req_type;
		int param_1;
		int param_2;
		long long param_3;
	};


	//============================================= ������� =============================================

	struct CrossGameCrossActivityGotoScene // ����֪ͨ��ҽ���ָ�����Ӧ�ĳ�����֮��ͳһ�����������ÿ�ζ��¼�һģһ����Э��
	{
		CrossGameCrossActivityGotoScene() : header(MT_CROSS_GAME_CROSS_ACTIVITY_GOTO_SCENE) {}
		MessageHeader	header;

		int cross_activity_type;
		int cross_uid;
	};

	struct GameCrossPastureInfoSync
	{
	public:
		GameCrossPastureInfoSync() : header(MT_GAME_CROSS_PASTURE_INFO_SYNC) {}
		MessageHeader header;

		long long uuid;
		int get_score;													// ��ǰ�ο����û���
		int get_score_times;											// ��ǰ�ο����ȡ���ֵĴ���
		int rank;
	};

	//============================================= ���BOSS =============================================
	enum CROSS_BOSS_TYPE
	{
		CROSS_BOSS_TYPE_CROSS = 0,
		CROSS_BOSS_TYPE_MIZANG,
		CROSS_BOSS_TYPE_YOUMING,
	};

	struct GameCrossBossSyncPlayerInfo
	{
		GameCrossBossSyncPlayerInfo() : header(MT_GAME_CROSS_BOSS_SYNC_PLAYER_INFO) {}
		MessageHeader	header;

		int cross_boss_type;

		long long uuid;														// ȫƽ̨ΨһID
		int today_gather_ordinary_crystal_times;							// �����Ѳɼ���ͨˮ������
		int today_gather_treasure_crystal_times;							// �����Ѳɼ���ϧˮ������
		int tire_value;														// ƣ��ֵ
		unsigned int concern_flag[CROSS_BOSS_SCENE_MAX];

		UNSTD_STATIC_CHECK(CROSS_MIZANG_BOSS_SCENE_MAX <= CROSS_BOSS_SCENE_MAX)       // Getconcernflag�ĵط�����ͬһ��Э��,��memcpy
		UNSTD_STATIC_CHECK(CROSS_YOUMING_BOSS_SCENE_MAX <= CROSS_BOSS_SCENE_MAX)
	};

	struct GameGameBossCreateBossBroadcast                    // bossˢ�¹㲥
	{
		GameGameBossCreateBossBroadcast() : header(MT_GAME_GAME_BOSS_CREATE_BOSS_BROADCAST) {}
		MessageHeader	header;

		short boss_id;
		short scene_id;
		unsigned int next_refresh_timestamp;
		long long uuid;
	};

	struct GameGameBossInfoReq
	{
		GameGameBossInfoReq() : header(MT_GAME_GAME_BOSS_INFO_REQ) {}
		MessageHeader	header;

		long long uuid;
		int cross_boss_type;
		int req_type;
		int param_1;
		int param_2;
	};

	/////////////////////////////////////////////  ������ /////////////////////////////////////////////
	struct SyncCrossTeamFBUserInfo
	{
		SyncCrossTeamFBUserInfo() : header(MT_SYNC_CROSS_TEAM_FB_USER_INFO) {}
		MessageHeader	header;

		CrossTeamFBUserInfo user_info;
	};

	struct CrossTeamFBOptionReq
	{
		CrossTeamFBOptionReq() : header(MT_CROSS_TEAM_FB_USER_OPTION_INFO) {}
		MessageHeader	header;

		int option_type;
		int param;
		int param2;
		int param3;
		CrossTeamFBUserInfo user_info;
	};

	struct CrossTeamFBOption2Req
	{
		CrossTeamFBOption2Req() : header(MT_CROSS_TEAM_FB_ROOM_OPTION_INFO) {}
		MessageHeader	header;

		int option2_type;
		int layer;
		int room;
	};

	/////////////////////////////////////////////  �������ս /////////////////////////////////////////////
	struct GameCrossGuildBattleInfoSync
	{
	public:
		GameCrossGuildBattleInfoSync() : header(MT_GAME_CROSS_GUILD_BATTLE_INFO_SYNC) {}
		MessageHeader header;

		CrossGuildBattleSceneInfo scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];

		int count;
		CrossGuildBattleUserScoreRankInfo user_core_rank_list[CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM];
	};

	struct CrossGameGuildBattleInfoSync
	{
	public:
		CrossGameGuildBattleInfoSync() : header(MT_CROSS_GAME_GUILD_BATTLE_INFO_SYNC) {}
		MessageHeader header;

		CrossGuildBattleRankItem owner_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];

		int count;
		CrossGuildBattleUserScoreRankInfo user_core_rank_list[CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM];
	};

	struct GameCrossGuildBattleRankLogSync
	{
	public:
		GameCrossGuildBattleRankLogSync() : header(MT_GAME_CROSS_GUILD_BATTLE_RANK_LOG_SYNC) {}
		MessageHeader header;

		CrossGuildBattleSceneLogInfo scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	struct GameCrossGetGuildBattleRankLogReq
	{
	public:
		GameCrossGetGuildBattleRankLogReq() : header(MT_GAME_CROSS_GET_GUILD_BATTLE_RANK_LOG_REQ) {}
		MessageHeader header;

		UniqueUserID uni_user_id;
	};

	struct CrossGameGetGuildBattleRankLogResp
	{
	public:
		CrossGameGetGuildBattleRankLogResp() : header(MT_CROSS_GAME_GET_GUILD_BATTLE_RANK_LOG_RESP) {}
		MessageHeader header;

		UniqueUserID uni_user_id;
		CrossGuildBattleSceneLogInfo scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	struct CrossGameGetGuildBattleBossInfoReq
	{
	public:
		CrossGameGetGuildBattleBossInfoReq() : header(MT_CROSS_GAME_GET_GUILD_BATTLE_BOSS_INFO_REQ) {}
		MessageHeader header;

		long long uuid;
		int scene_id;
	};

	struct CrossGameGetGuildBattleMonsterInfoReq
	{
	public:
		CrossGameGetGuildBattleMonsterInfoReq() : header(MT_CROSS_GAME_GET_GUILD_BATTLE_MONSTER_INFO_REQ) {}
		MessageHeader header;

		long long uuid;
	};

	struct CrossGameGuildBattleSpecialTimeNotice
	{
		CrossGameGuildBattleSpecialTimeNotice() : header(MT_CROSS_GAME_GUILD_BATTLE_SPECIAL_TIME_NOTICE) {}
		MessageHeader header;
	};

	struct CrossGameGuildBattleUserScoreRank
	{
		CrossGameGuildBattleUserScoreRank() : header(MT_CROSS_GAME_GUILD_BATTLE_USER_SCORE_RANK) {}
		MessageHeader header;

		int count;
		CrossGuildBattleUserScoreRankInfo user_core_rank_list[CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM];
	};

	struct CrossGameGuildBattleRegisterSync
	{
		CrossGameGuildBattleRegisterSync() :header(MT_CROSS_GAME_GUILD_BATTLE_REGISTER_SYNC) {}
		MessageHeader header;

		NetID gs_netid;
	};

	struct GameGameGuildBattleRegisterSync
	{
	public:
		GameGameGuildBattleRegisterSync() : header(MT_GAME_GAME_GUILD_BATTLE_REGISTER_SYNC) {}
		MessageHeader header;

		CrossGuildBattleRankItem owner_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	/////////////////////////////////////////////  ������� /////////////////////////////////////////////

	struct GameCrossFishingInfoSync
	{
		GameCrossFishingInfoSync() : header(MT_GAME_CROSS_FISHING_INFO_SYNC) {}
		MessageHeader header;

		Int64 unique_user_id;
		int rank;
	};

	struct CrossGameFishingInfoSync
	{
		CrossGameFishingInfoSync() : header(MT_CROSS_GAME_FISHING_INFO_SYNC) {}
		MessageHeader header;

		Int32 uid;
		int rank;
	};

	/////////////////////////////////////////////  �������ȼ� /////////////////////////////////////////////
	struct CrossGameCrossWorldLevelInfo
	{
	public:
		CrossGameCrossWorldLevelInfo() : header(MT_CROSS_GAME_CROSS_WORLD_LEVEL_INFO) {}
		MessageHeader header;

		int world_level;
	};

	/////////////////////////////////////////////  �������ش� /////////////////////////////////////////////
	struct GameCrossLieKunFBInfoSync
	{
	public:
		GameCrossLieKunFBInfoSync() : header(MT_GAME_CROSS_LIEKUN_FB_INFO_SYNC) {}
		MessageHeader header;

		CrossLieKunFBSceneInfo scene_list[LIEKUN_ZONE_TYPE_COUNT];
	};

	struct GameCrossLieKunFBKillZoneBossSync
	{
	public:
		GameCrossLieKunFBKillZoneBossSync() : header(MT_GAME_CROSS_LIEKUN_FB_KILL_ZONE_BOSS_SYNC) {}
		MessageHeader header;

		long long uuid;

		int zone;
		GuildID kill_zone_boss_guild_id;
	};

	struct CrossGameLieKunFBKillZoneBossSync
	{
	public:
		CrossGameLieKunFBKillZoneBossSync() : header(MT_CROSS_GAME_LIEKUN_FB_KILL_ZONE_BOSS_SYNC) {}
		MessageHeader header;

		long long uuid;

		int zone;
		GuildID kill_zone_boss_guild_id;
	};

	struct CrossGameLieKunFBTopRewardSync
	{
	public:
		CrossGameLieKunFBTopRewardSync() : header(MT_CROSS_GAME_LIEKUN_FB_TOP_REWARD_SYNC) {}
		MessageHeader header;
	};

 /////////////////////////////////////////////  ����Ҷ� /////////////////////////////////////////////
	struct GameCrossMessBattleInfoSync
	{
		GameCrossMessBattleInfoSync() : header(MT_GAME_CROSS_MESS_BATTLE_INFO_SYNC) {}
		MessageHeader header;

		Int64 unique_user_id;
		int rank;
	};

	struct CrossGameMessBattleInfoSync
	{
		CrossGameMessBattleInfoSync() : header(MT_CROSS_GAME_MESS_BATTLE_INFO_SYNC) {}
		MessageHeader header;

		Int32 uid;
		int rank;
	};
	/////////////////////////////////////////////  ���ҹս���� ///////////////////////////////////////////
	struct GameCrossnNightFightInfoSync
	{
		GameCrossnNightFightInfoSync() : header(MT_GAME_CROSS_NIGHT_FIGHT_INFO_SYNC) {}
		MessageHeader header;

		Int64 unique_user_id;
		int rank;
	};

	struct CrossGameNightFightInfoSync
	{
		CrossGameNightFightInfoSync() : header(MT_CROSS_GAME_NIGHT_FIGHT_INFO_SYNC) {}
		MessageHeader header;

		Int32 uid;
		int rank;
	};

	////////////////////////////////////	������а�		//////////////////////////////////

	struct CrossGameSyncRankFirst
	{
		CrossGameSyncRankFirst() : header(MT_CROSS_GAME_SYNC_RANK_FIRST) {}
		MessageHeader	header;

		int rank_type;
		int uid_list[CROSS_RANK_REWARD_COUNT];
	};

	struct CrossGameSyncCoupleRankFirst
	{
		CrossGameSyncCoupleRankFirst() : header(MT_CROSS_GAME_SYNC_COUPLE_RANK_FIRST) {}
		MessageHeader	header;

		int rank_type;
		long long male_uuid_list[CROSS_RANK_REWARD_COUNT];
		long long female_uuid_list[CROSS_RANK_REWARD_COUNT];
	};

	struct GameCrossSyncCrossCoupleRankInfo
	{
		GameCrossSyncCrossCoupleRankInfo() : header(MT_GAME_CROSS_SYNC_CROSS_COUPLE_RANK_INFO) {}
		MessageHeader	header;

		struct RankItem
		{
			bool operator< (const RankItem &rhs) const
			{
				return male_rank_value + female_rank_value < rhs.male_rank_value + rhs.female_rank_value;
			}

			int server_id = 0;				// ������id
			int male_uid = 0;				// ��uid
			int female_uid = 0;				// Ůuid
			GameName male_name = { 0 };		// ������
			GameName female_name = { 0 };	// Ů����
			char male_prof = 0;				// ��ְҵ
			char female_prof = 0;			// Ůְҵ
			short reserve_sh = 0;			// ����
			int male_rank_value = 0;		// ������ֵ
			int female_rank_value = 0;		// Ů����ֵ
		};

		int plat_type;

		int rank_type;
		int item_count;
		RankItem rank_item_list[COUPLE_RANK_MAX_SIZE];
	};

	struct GameCrossCoupleRankDivorce
	{
		GameCrossCoupleRankDivorce() : header(MT_GAME_CROSS_SYNC_COUPLE_RANK_DIVORCE) {}
		MessageHeader	header;

		int plat_type;
		int uid;
	};

	struct CrossGameSyncCrossCoupleForceSnap
	{
		CrossGameSyncCrossCoupleForceSnap() : header(MT_CROSS_GAME_SYNC_CROSS_COUPLE_FORCE_SNAP) {}
		MessageHeader	header;
	};

	//=================================================== �����ͨboss� ===========================================
	enum CROSS_COMMON_BOSS_SYNC_REASON_TYPE
	{
		CROSS_COMMON_BOSS_SYNC_REASON_TYPE_BORN = 0,		// �������
		CROSS_COMMON_BOSS_SYNC_REASON_TYPE_DIE,				// ��������
		CROSS_COMMON_BOSS_SYNC_REASON_TYPE_DROP,            // ����

		CROSS_COMMON_BOSS_SYNC_REASON_TYPE_MAX
	};

	struct CrossGameCrossCommonBossInfoChange
	{
		struct KillerInfo
		{
			int uid;
			unsigned int kill_timestamp;
			GameName name;
		};

		CrossGameCrossCommonBossInfoChange() : header(MT_CROSS_GAME_CROSS_COMMON_BOSS_INFO_CHANGE) {}
		MessageHeader header;

		short boss_type;
		short reason_type;
		unsigned short scene_id;
		unsigned short boss_id;
		unsigned int next_refresh_timestamp;
		KillerInfo killer;
	};

	struct CrossGameSyncCrossCommonBossInfoReq
	{
		CrossGameSyncCrossCommonBossInfoReq() : header(MT_CROSS_GAME_CROSS_COMMON_BOSS_SYNC_INFO_REQ) {}
		MessageHeader header;

		unsigned int global_netid;
	};

	struct CrossGameSyncCrossCommonBossInfo
	{
		static const int BOSS_MAX_COUNT = 50;
		static const int KILLER_INFO_MAX_COUNT = 5;
		struct KillerInfo
		{
			int killer_uid;
			int kill_timestamp;
			GameName killer_name;
		};

		struct BossInfo
		{
			unsigned short scene_id;
			unsigned short boss_id;
			char boss_status;
			char reserve_ch_list[3];
			unsigned int next_refresh_timestamp;

			KillerInfo killer_info_list[KILLER_INFO_MAX_COUNT];
		};

		CrossGameSyncCrossCommonBossInfo() : header(MT_CROSS_GAME_CROSS_COMMON_BOSS_SYNC_INFO) {}
		MessageHeader header;

		int boss_type;
		int boss_count;
		BossInfo boss_info_list[BOSS_MAX_COUNT];
	};

	struct CrossGameSyncCrossCommonBossDropInfo
	{
		CrossGameSyncCrossCommonBossDropInfo() : header(MT_CROSS_GAME_CROSS_COMMON_BOSS_DROP_INFO) {}
		MessageHeader header;

		long long uuid;
		GameName role_name;
		int scene_id;
		unsigned short monster_id;
		ItemID item_id;
		int item_num;
		unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];
	};

	////////////////////////////////////	����߾�֮��		//////////////////////////////////
	struct GameCrossSyncCrossBianJingZhiBossInfo
	{
		GameCrossSyncCrossBianJingZhiBossInfo() : header(MT_HIDDEN_CROSS_BIANJINGZHIDI_BOSS_INFO) {}
		MessageHeader header;

		struct BossInfo
		{
			int boss_id;
			int live_flag;
		};

		unsigned int next_boss_refresh_time;
		BossInfo boss_list[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];
	};

	///////////////////////////////////////////////////////////////////////////////////////////

	struct UseGoldInHiddenGs
	{
		UseGoldInHiddenGs() : header(MT_USE_GOLD_IN_HIDDEN_GS) {}
		MessageHeader header;

		int is_use_bind;
		int is_consume_sys;
		int uid;
		GameName user_name;
		PlatName plat_name;
		long long use_gold;
		long long cur_gold;
		char type[128];
	};

	//=================================================== ������� ===========================================
	struct CrossGameSyncCrossRandActivityData
	{
		CrossGameSyncCrossRandActivityData() : header(MT_CROSS_GAME_SYNC_CROSS_RAND_ACTIVITY_DATA) {}
		MessageHeader	header;

		int			activity_type;			// ��������

											// ����ӻ����
	};

	struct GameCrossSyncCrossRandActivityData
	{
		GameCrossSyncCrossRandActivityData() : header(MT_GAME_CROSS_SYNC_CROSS_RAND_ACTIVITY_DATA) {}
		MessageHeader	header;

		int			activity_type;			// ��������

											// ����ӻ����
	};
	
	struct GameGameCrossGuildBattleSos
	{
		GameGameCrossGuildBattleSos():header(MT_GAME_GAME_CROSS_GUILD_BATTLE_SOS){}
		MessageHeader header;

		static const int MEMBER_SCENE_NAME_LENGTH = 32;

		GuildID				guild_id;
		int					member_uid;
		GameName			member_name;
		int					member_camp;
		int					member_scene_id;
		int					member_scene_key;
		short				member_pos_x;
		short				member_pos_y;
		char				member_scene_name[MEMBER_SCENE_NAME_LENGTH];
		short				sos_type;
		short				sos_sysmsg;			// 1SOS�������촰��
	};

	struct GameGameKillCrossBoss
	{
		GameGameKillCrossBoss():header(MT_GAME_GAME_KILL_CROSS_BOSS){}
		MessageHeader header;

		GuildID guild_id;
		UserID  uid;
		short scene_type;
		short reserve_sh;
	};

	struct PickGuildRareItemInHiddenGs
	{
		PickGuildRareItemInHiddenGs() : header(MT_PICK_GUILD_RARE_ITEM_IN_HIDDEN_GS) {}
		MessageHeader header;

		struct GuildRareLogItem
		{
			GuildRareLogItem() :uuid(0), item_id(0), item_num(0), timestamp(0), scene_id(0), monster_id(0)
			{
				memset(role_name, 0, sizeof(GameName));
				memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
			}

			long long uuid;
			GameName role_name;
			ItemID item_id;
			short item_num;
			short is_from_gift;
			ItemID gift_item_id;
			unsigned int timestamp;
			int	scene_id;
			int monster_id;

			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	// ��Ʒ��������
		};

		GuildID guild_id;
		GuildRareLogItem item;
	};

	struct BroadcastGuildRareInfo
	{
		BroadcastGuildRareInfo() : header(MT_BROADCAST_GUILD_RARE_INFO) {}
		MessageHeader header;

		struct GuildRareLogItem
		{
			GuildRareLogItem() :uuid(0), item_id(0), item_num(0), timestamp(0), scene_id(0), monster_id(0)
			{
				memset(role_name, 0, sizeof(GameName));
				memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
			}

			long long uuid;
			GameName role_name;
			ItemID item_id;
			short item_num;
			short is_from_gift;
			ItemID gift_item_id;
			unsigned int timestamp;
			int	scene_id;
			int monster_id;

			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	// ��Ʒ��������
		};

		int plat_type;
		GuildID guild_id;
		long long target_uuid;

		GuildRareLogItem item;
	};

	struct SyncCrossRankInfoToOriginGs
	{
		SyncCrossRankInfoToOriginGs() : header(MT_SYNC_CROSS_RANK_INFO) {}
		MessageHeader header;

		int cross_rank_type;
		int param1;
		int param2;
		long long rank_value;
		int rank_pos;
	};

	struct SyncCrossSystemMsgToOriginServer  // ���ط�ͬ��ϵͳ��Ϣ��ԭ��(���ط������ʹ����ת��Э�飬�������ֻ�Ǵӿ����ԭ��)
	{
		SyncCrossSystemMsgToOriginServer() : header(MT_CROSS_GAME_SYSTEM_MSG_TO_ORIGIN) {}
		MessageHeader header;

		long long uuid; // Ψһ��ʶ���������ǹ���uuid�������uuid
		int msg_type;   // enum SYS_MSG_TYPE
		int msg_length;
		// ... ������Ϣ���� Protocol::SCSystemMsg
	};

	struct CrossGameRequestGuildChat			// ���ط������󱾷���������
	{
		CrossGameRequestGuildChat() : header(MT_CROSS_GAME_GUILD_CHAT) {}
		MessageHeader header;

		long long			guild_id;	// ����ID

		int					msg_origin_type;
		int					msg_length;
		char				msg_buff[2048];  // ������������
	};

	struct GameGameCrossGuildMemberBeKill				// �������˳�Ա����ɱ��ԭ��
	{
		GameGameCrossGuildMemberBeKill() :header(MT_GAME_GAME_CROSS_GUILD_MEMBER_BE_KILL) {}
		MessageHeader header;

		long long			killer_uuid;
		GameName			killer_name;
		long long			be_killer_uuid;
		GuildID				be_killer_guild_id;
	};
	
	struct CrossGameGetGuildEnemyList				// ��ԭ����ѯ��ȡ���˳����б�
	{
		CrossGameGetGuildEnemyList() :header(MT_CROSS_GAME_GET_GUILD_ENEMY_LIST) {}
		MessageHeader header;

		long long			request_uuid;
		GuildID				guild_id;
	};

	//////////////////////////////////// ��������� ////////////////////////////////

	struct CrossGameSyncCrossChallengefieldReq   // �������������
	{
		CrossGameSyncCrossChallengefieldReq() : header(MT_SYNC_CROSS_CHALLENGEFIELD_REQ) {}
		MessageHeader header;

		unsigned int global_netid;
	};

	enum SYNC_CROSS_CHALLENGEFILED_INFO_TYPE
	{
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_INVALID = 0,
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME,			// ��ʼ��ԭ������
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_INSERT_TO_HIDDEN,		// ��������ҵ����ط�
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_UPDATE_TO_HIDDEN,		// ͬ�����µ����ط�
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_RANK_CHANGE_TO_HIDDEN,	// ����������µ����ط�
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_UPDATE_TO_GAME,			// ͬ�����µ���Ϸ��
	};

	enum SYNC_CROSS_CHALLENGEFILED_INFO_INIT_SUB_TYPE
	{
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME_BEGIN,			// ��ʼ��ʼ��ԭ������
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME_INFO,			// ��Ϣ��ʼ��ԭ������
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME_END,			// ������ʼ��ԭ������
	};
	struct SyncCrossChallengefieldDataInfo		// ͬ������������
	{
		SyncCrossChallengefieldDataInfo() : header(MT_SYNC_CROSS_CHALLENGEFIELD_DATA_INFO) {}
		MessageHeader header;

		short sync_type;
		unsigned short sub_type;

		int count;
		ChallengeUserListParam::ChallengeUserAttr user_list[CHALLENGE_FIELD_MAX_USER_NUM];
	};

	struct CrossHusongShuijingGatherInfoReq
	{
		CrossHusongShuijingGatherInfoReq():header(MT_CROSS_HUSONG_SHUIJING_GATHER_INFO_REQ){}
		MessageHeader header;
		long long uuid;
	};

	struct GameCrossForbidTalk
	{
		GameCrossForbidTalk() : header(MT_GAME_CROSS_FORBID_TALK) {}
		MessageHeader header;

		long long uuid;
		int forbid_time;
	};

	struct CrossGameForbidTalk
	{
		CrossGameForbidTalk() : header(MT_CROSS_GAME_FORBID_TALK) {}
		MessageHeader header;

		int cross_role_id;
		int forbid_time;
	};
}

#pragma pack(pop)

#endif	// __CROSS_GAME_PROTOCAL_H__

