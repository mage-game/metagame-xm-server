#ifndef __CROSS_MSG_CODE_H__
#define __CROSS_MSG_CODE_H__

namespace Protocol
{
	// ע�⣡�� ÿ�ֿ���淨Э��ŵ���һ���� global����Э���ת������Ӧcross

	enum
	{
		CROSS_SERVER_MSG_BEGIN = 14000,

		CROSS_SERVER_COMMON_BEGIN = CROSS_SERVER_MSG_BEGIN,		// ͨ��
		CROSS_SERVER_COMMON_END = 14090,

		CROSS_ACTIVITY_1V1_BEGIN = 14100,						// ���1v1

		CROSS_ACTIVITY_3V3_BEGIN = 14130,						// ���3v3

		CROSS_LIEKUN_FB_BEGIN = 14160,							// �������ش�

		CROSS_XIULUO_TOWER_BEGIN = 14190,						// ���������

		CROSS_GUILD_BATTLE_BEGIN = 14220,						// �������ս

		CROSS_TEAM_FB_BEGIN = 14250,							// �����ӱ�

		CROSS_BIANJINGZHIDI_BEGIN = 14280,						// ����߾�֮��

		// �����ļ�������...

		CROSS_OTHER_BEGIN = 14800,								// ����

		CROSS_RAND_ACTIVITY_MSG_BEGIN=14900,

		CROSS_SERVER_MSG_END = 15000,
	};


	// ÿ��Э��εĿ�ʼЭ��Ŷ�Ӧ����Ķ��塣
	// ����Э��ַ���ʱ�����CrossManager::GetFuncTypeWithCrossProtocolMsgType��ȡ���(func_type)���͸���Ӧ��crossserver
	enum
	{
		// =============================================================================================================
		MT_RETURN_ORIGINAL_SERVER_SC = CROSS_SERVER_COMMON_BEGIN,			// ֪ͨ��һص�ԭ�� 14000
		MT_CROSS_GET_PERSON_RANK_LIST_ACK_SC = 14001,						// �·�������а��б�
		MT_CROSS_GET_COUPLE_RANK_LIST_ACK_SC = 14002,						// �·�������°��б�
		MT_NOTICE_CAN_ENTER_CROSS_SC = 14003,								// ֪ͨ��ҽ�����

		MT_CROSS_GET_PERSON_RANK_LIST_CS = 14010,							// ��ȡ������а��б�

		// =============================================================================================================
		MT_CROSS_1V1_MATCH_ACK_SC = CROSS_ACTIVITY_1V1_BEGIN,				// ���1V1ƥ��״̬ 14100
		MT_CROSS_1V1_WEEK_RECORD_SC = 14101,								// ���1V1���˼�¼
		MT_CROSS_1V1_RANK_LIST_SC = 14102,									// ���1V1���а�
		MT_CROSS_1V1_MATCH_RESULT_SC = 14103,								// ���1V1ƥ����
		MT_CROSS_1V1_FIGHT_RESULT_SC = 14104,								// ���1V1ս�����

		MT_CROSS_1V1_MATCH_QUERY_CS = 14115,								// ���1V1ƥ��״̬��ѯ
		MT_CROSS_1V1_WEEK_RECORD_QUERY_CS = 14116,							// ���1V1���˼�¼��ѯ
		MT_CROSS_1V1_GET_RANK_LIST_CS = 14117,								// ���1V1��ȡ���а�
		// =============================================================================================================

		MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_SC = CROSS_ACTIVITY_3V3_BEGIN, // ���3V3ƥ��״̬֪ͨ 14130
		MT_CROSS_MULTIUSER_CHALLENGE_RANKLIST_SC = 14131,					// ���3V3���а��б�
		MT_CROSS_MULTIUSER_CHALLENGE_HAS_MATCH_NOTICE_SC = 14132,			// ���3V3�Ƿ��б���֪ͨ

		MT_GET_MULTIUSER_CHALLENGE_RANK_LIST_CS = 14145,					// ��ȡ���3V3���а��б�
		MT_MULTIUSER_CHALLENGE_CHECK_USER_HAS_MATCH_CS = 14146,				// ���3V3�������Ƿ��б���
		// =============================================================================================================

		MT_CROSS_LIEKUN_FB_REQ_CS = CROSS_LIEKUN_FB_BEGIN,					// �������ش���������Ϣ 14160
		MT_CROSS_LIEKUN_FB_PLAYER_INFO_SC = 14175,							// �������ش��������Ϣ
		// =============================================================================================================

		MT_CROSS_XIULUO_TOWER_GET_DROP_LOG_CS = CROSS_XIULUO_TOWER_BEGIN,	// ���������������־��Ϣ���� 14190
		MT_CROSS_XIULUO_TOWER_DROP_LOG_SC = 14205,							// ���������������־��Ϣ�·�
		// =============================================================================================================

		MT_CROSS_GUILD_BATTLE_GET_DROP_LOG_CS = CROSS_GUILD_BATTLE_BEGIN,	// �������ս������־��Ϣ���� 14220
		MT_CROSS_GUILD_BATTLE_GET_MONSTER_INFO_CS = 14221,					// �������ս���������Ϣ����

		MT_CROSS_GUILD_BATTLE_DROP_LOG_SC = 14235,							// �������ս������־��Ϣ�·�
		// =============================================================================================================

		MT_NOTICE_CROSS_TEAM_FB_ROOM_INFO_CHANGE_SC = CROSS_TEAM_FB_BEGIN,	// �����ӱ���������Ϣ�ı� 14250
		MT_NOTICE_CROSS_TEAM_FB_ROOM_LIST_INFO_SC = 14251,					// �����ӱ��������б���Ϣ
		MT_NOTICE_CROSS_TEAM_FB_ROOM_INFO_SC = 14252,						// �����ӱ���������Ϣ

		// =============================================================================================================

		MT_CROSS_SERVER_BIANJINGZHIDI_BOSS_INFO_SC = CROSS_BIANJINGZHIDI_BEGIN,	// ����߾�֮��, �·�BOSS��Ϣ 14280

		MT_CROSS_SERVER_BIANJINGZHIDI_BOSS_INFO_CS = 14285,						// ����߾�֮�أ� ��ȡBOSS��Ϣ

		// =============================================================================================================
		
		MT_CROSS_RA_CHONGZHI_RANK_GET_RANK_CS = 14901,						// �����ֵ���л������ս�����а�
	
		MT_CROSS_RA_CHONGZHI_RANK_GET_RANK_ACK_SC = 14907,					// �����ֵ���л������ս�����а񷵻�

		MT_CROSS_RA_CONSUME_RANK_GET_RANK_CS = 14908,						// ����������л������ս�����а�
		MT_CROSS_RA_CONSUME_RANK_GET_RANK_ACK_SC = 14909,					// ����������л������ս�����а񷵻�
		
		MT_CROSS_SERVER_MSG_MAX,
	};
}

#endif	// __CROSS_MSG_CODE_H__

