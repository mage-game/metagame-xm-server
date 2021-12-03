#ifndef __GLOBALMSGCODE_H__
#define __GLOBALMSGCODE_H__

namespace Protocol
{
	enum _MsgTypeGlobal
	{
		//////////////////////////////////////////////// ȫ�ַ�������� /////////////////////////////////////////////////////////
		MT_ENTER_GLOBAL_SERVER_ACK_SC = 9000,									// �����½ȫ�ַ������ظ�
		MT_TIME_ACK_SC = 9001,													// ������ʱ�䷵��
		MT_WALLOW_NOTICE_SC = 9002,												// �û������Է���
		MT_DISCONNECT_NOTICE_SC = 9003,											// ֪ͨ�ͻ��˶Ͽ�������

		MT_ENTER_GLOBAL_SERVER_REQ_CS = 9050,									// �����½ȫ�ַ�����
		MT_TIME_REQ_CS = 9051,													// ���������ʱ��

		//////////////////////////////////////////////// ������ /////////////////////////////////////////////////////////
		MT_TEAM_INFO_SC = 9100,													// ������Ϣ
		MT_OUT_OF_TEAM_SC = 9101,												// �����Ѿ���ɢ
		MT_INVITE_USER_TRANSMIT_SC = 9102,										// ֪ͨ������
		MT_REQ_JOIN_TEAM_SC = 9103,												// ֪ͨ�ӳ���������������
		MT_TEAM_LIST_ACK_SC = 9104,												// �������ڳ��������б�ظ�
		MT_JOIN_TEAM_SC = 9105,													// ֪ͨ��Ҽ����˶���
		MT_ROLE_TEAM_INFO_SC = 9106,											// ��ɫ��صĶ�����Ϣ
		MT_TEAM_ROLL_DROP_RET_SC = 9107,										// ��ȡ��Ʒʱҡ������
		MT_TEAM_LEADER_CHANGE_SC = 9108,										// ֪ͨ�ӳ��������
		
		MT_CREATE_TEAM_CS = 9150,												// ��������
		MT_INVITE_USER_CS = 9151,												// ����������
		MT_INVITE_RET_CS = 9152,												// �ظ�����
		MT_REQ_JOIN_TEAM_RET_CS = 9153,											// �ӳ��ظ����������߼������
		MT_REQ_JOIN_TEAM_CS = 9154,												// ����������ĳ����
		MT_DISMISS_TEAM_CS = 9155,												// �ӳ���ɢ����
		MT_KICK_OUT_OF_TEAM_CS = 9156,											// �ӳ�����Ա�߳�����
		MT_CHANGE_TEAM_LEADER_CS = 9157,										// ���ӳ�
		MT_EXIT_TEAM_CS = 9158,													// ����˳�����
		MT_TEAM_LIST_REQ_CS = 9159,												// �������ڳ����Ķ����б�
		MT_CHANGE_MUST_CHECK_CS = 9160,											// �ı��������Ƿ���Ҫ�ӳ�ͨ��
		MT_CHANGE_ASSIGN_MODE_CS = 9161,										// �ı�������ģʽ
		MT_CHANGE_MEMBER_CAN_INVITE_CS = 9162,									// �ı������ͨ��Ա�Ƿ������
		MT_CHANGE_TEAM_LIMIT_CS = 9163,											// �޸Ķ����������
		MT_AUTO_HAVE_TEAM_CS = 9164,											// �Զ����
		MT_AUTO_APPLY_JOIN_TEAM_CS = 9165,										// �Զ���Ӧ��������
		MT_INVITE_UNIQUE_USER_CS = 9166,										// ���������飨ʹ��uuid��

		//////////////////////////////////////////////// ������� /////////////////////////////////////////////////////////
		MT_GET_ROLE_SIMPLE_INFO_SC = 9300,										// �鿴��Ҽ����Ϸ���
		MT_ROLE_IS_OFFLINE_OR_NOT_EXISTED,										// ������߻��ɫ������
		MT_RANDOM_ROLE_LIST_SC,													// �������б���							
		MT_ACTIVITY_STATUS_SC,													// �״̬����
		MT_ACTIVITY_ROOM_INFO_ACK_SC,											// ������Ȫ��Ϣ����
		MT_QUESTION_RANK_INFO_ACK_SC,											// �������񷵻�
		MT_QUESTION_TOTAL_RESULT_SC,											// ��Ҵ������ս��
		MT_QUESTION_REBEGIN_SC,													// �������¿�ʼ
		MT_QUESTION_ANWER_RESULT_SC,											// ����ش���
		MT_ROLE_EVALUATE_INFO_SC,												// �������۽��
		MT_GET_ROLE_LIST_CAP_SC,												// �鿴��ɫ�б�ս����
		MT_XIANNV_ALL_LIMIT_BUY_COUNT_SC,										// ��������Ůȫ���޹�ʣ������
		MT_PET_ALL_LIMIT_BUY_COUNT_SC,											// ����������ȫ���޹�ʣ������
		MT_RANDACTIVITY_LUCKY_NEWS_SC,											// ����������ת������б�
		MT_DATING_TARGET_LIST_SC,												// Լ��Ŀ���б���Ϣ
		MT_DATING_NOTIFY_SC,													// Լ���������
		MT_COMBINE_QINGDIAN_ROLL_NEWS_SC,										// �Ϸ�����۳�ת������б�
		
		MT_FIND_ROLE_BY_NAME_CS = 9350,											// ��������Ʋ������
		MT_GET_ROLE_SIMPLE_INFO_CS,												// �鿴��Ҽ����ϣ�global������Ϣ ����9301Э�飩
		MT_GET_ROLE_BASE_INFO_CS,												// �鿴��һ������ϣ�ת��gameworld��ȡ��Ϣ��
		MT_GET_RANDOM_ROLE_LIST_CS,												// ��ȡ���������б���Ϊ�Ƽ������ã�
		MT_HOT_SPRING_INFO_REQ_CS,												// ������Ȫ��Ϣ
		MT_QUESTION_RANK_INFO_REQ_CS,											// ��������
		MT_GET_ONE_EQUIPMENT_INFO_CS,											// ���󵥸�װ����Ϣ
		MT_GET_ONE_PET_INFO_CS,													// ���󵥸�������Ϣ
		MT_GET_ONE_MOUNT_INFO_CS,												// ���󵥸�������Ϣ
		MT_GET_ONE_SOUL_INFO_CS,												// ���󵥸�������Ϣ
		MT_GET_TRUCK_POS_INFO_CS,												// �����ڳ�λ����Ϣ
		MT_GET_ROLE_SIGNATURE_CS,												// �����ɫǩ��
		MT_GET_ROLE_DAY_COUNT_CS,												// �����ɫ������Ϣ
		MT_GET_ROLE_LIST_CAP_CS,												// �鿴��ɫ�б�ս����
		MT_LAND_WAR_JOIN_REQ_CS,												// ���ս����
		MT_LAND_WAR_ENTER_MATCH_REQ_CS,											// ���ս�����볡
		MT_LAND_WAR_GIVEUP_LAND_CS,												// ���ս�������
		MT_LAND_WAR_GET_LAND_LIST_CS,											// ���ս��ȡ�������Ϣ
		MT_LAND_WAR_GET_JOIN_GUILD_LIST_CS,										// ���ս��ȡ���������б�
		MT_LAND_WAR_GET_REPORT_LIST_CS,											// ���ս��ȡս���б�
		MT_MARRY_RESERVE_REQ_CS,												// ���ԤԼ
		MT_MARRY_HUNYAN_ENTER_REQ_CS,											// ����������
		MT_MARRY_GET_MARRY_RESERVE_INFO_CS,										// �������ԤԼ��Ϣ
		MT_GET_ACTIVITY_ROME_STATUS_CS,											// ����������Ϣ
		MT_RAND_ACTIVITY_XIANNV_BUY_REQ_CS,										// ��������Ů�޹���������
		MT_GET_XIANNV_ALL_LIMIT_BUY_COUNT_CS,									// ��������ȡ��Ůȫ���޹�ʣ������
		MT_RAND_ACTIVITY_PET_BUY_REQ_CS,										// �����������޹���������
		MT_GET_PET_ALL_LIMIT_BUY_COUNT_CS,										// ��������ȡ����ȫ���޹�ʣ������
		MT_DATING_GET_TARGET_LIST_CS,											// ��ȡԼ������б�
		MT_DATING_CANCEL_INVITE_CS,												// ȡ��Լ������
		MT_DATING_EVALUTE_CS,													// Լ������
		MT_DATING_QUIT_CS,														// �˳�Լ��
		MT_DATING_GIVE_PRESENT_CS,												// Լ����������
		MT_DATING_ANSWER_QUESTION_CS,											// Լ�����
		MT_ACTIVITY_FIND_REQ_CS,												// ��һ�����
		MT_COMBINE_QINGDIAN_ROLL_NEWS_REQ_CS,									// �Ϸ�����۳�ת�̽ݱ�����

		//////////////////////////////////////////////// �ʼ���� /////////////////////////////////////////////////////////
		MT_GLOBAL_MAIL_SEND_RESULT_SC = 9500,									// ��ȡ��������
		MT_GLOBAL_MAIL_DELETE_SC = 9501,										// ɾ���ʼ�����
		MT_GLOBAL_MAIL_LOCK_SC = 9502,											// ���ʼ�����
		MT_GLOBAL_MAIL_UNLOCK_SC = 9503,										// �����ʼ�����
		MT_GLOBAL_MAIL_LIST_SC = 9504,											// �ʼ��б���
		MT_GLOBAL_MAIL_DETAIL_SC = 9505,										// �ʼ���ϸ��Ϣ
		MT_GLOBAL_MAIL_ATTACHMENT_SC = 9506,									// ��ȡ�ʼ���������
		MT_GLOBAL_MAIL_NEW_MAIL_NOTICE_SC = 9507,								// ���ʼ�֪ͨ
		MT_GLOBAL_MAIL_HAS_UNREAD_MAIL_SC = 9508,								// δ��ȡ�ʼ�֪ͨ
		
		//////////////////////////////////////////////// �����г� /////////////////////////////////////////////////////////
		MT_PUBLICSALE_SEARCH_CS = 9650,											// ����

		MT_PUBLICSALE_ADD_ACK_SC = 9600,										// ������Ʒ�ϼܷ���
		MT_PUBLICSALE_DEL_ACK_SC = 9601,										// ������Ʒ�¼ܷ���
		MT_PUBLICSALE_BUY_ACK_SC = 9602,										// ����������Ʒ����
		MT_PUBLICSALE_GET_ITEM_LIST_SC = 9603,									// ��ȡ�Լ�������Ʒ�б�
		MT_PUBLICSALE_SEARCH_SC = 9604,											// �������� 

		//////////////////////////////////////////////// ���а���� /////////////////////////////////////////////////////////
		MT_RANK_GET_PERSONRANK_LIST_ACK_SC = 10000,
		MT_RANK_GET_GUILDRANK_LIST_ACK_SC = 10001,
		MT_RANK_GET_TEAMRANK_LIST_ACK_SC = 10002,
		MT_RANK_GET_PERSONRANK_TOP_USER_SC = 10003,
		MT_RANK_GET_WORLD_LEVEL_ACK_SC = 10004,
		MT_RANK_GET_COUPLE_RANK_SC = 10005,
		MT_RANK_GET_SPECIAL_RANK_SC = 10006, 

		MT_RANK_GET_PERSONRANK_LIST_CS = 10050,
		MT_RANK_GET_GUILDRANK_LIST_CS = 10051,
		MT_RANK_GET_TEAMRANK_LIST_CS = 10052,
		MT_RANK_GET_PERSONRANK_TOP_USER_CS = 10053,
		MT_RANK_GET_COUPLE_RANK_LIST_CS = 10054,
		MT_RANK_GET_SPECIAL_RANK_LIST_CS = 10055, 
	
		//////////////////////////////////////////////// ������� /////////////////////////////////////////////////////////
		MT_CREATE_GUILD_SC = 9800,												// ���ء��������š����
		MT_DISMISS_GUILD_SC = 9801,												// ���ء���ɢ���š����
		MT_APPLY_FOR_JOIN_GUILD_SC = 9802,										// ���ء����������š� ���
		MT_NOTIFY_SUPER_SC = 9803,												// ֪ͨ���Ź����� 
		MT_QUIT_GUILD_SC = 9804,												// ���ء��˳����š� ���
		MT_INVITE_GUILD_SC = 9805,												// ���������� ����
		MT_INVITE_GUILD_NOTIFY_SC = 9806,										// ���������� ֪ͨ
		MT_KICK_OUT_GUILD_SC = 9807,											// �߳����ŷ���
		MT_KICK_OUT_GUILD_NOTIFY_SC = 9808,										// �������֪ͨ
		MT_APPOINT_GUILD_SC = 9809,												// ��������
		MT_LEAVEPOST_GUILD_SC = 9810,											// ж�η���
		MT_CHANGE_NOTICE_SC = 9811,												// �޸Ĺ��淵��
		MT_MAILALL_SC = 9812,													// Ⱥ���ʼ�����
		MT_GUILD_ALL_BASE_INFO_SC = 9813,										// �����б�
		MT_GUILD_APPLY_FOR_LIST_SC = 9814,										// ���������б�
		MT_GUILD_EVENT_LIST_SC = 9815,											// ������ͨ�¼��ʹ��¼��б�
		MT_GUILD_MEMBER_LIST_SC = 9816,											// ���ų�Ա�б�
		MT_GUILD_BASE_INFO_SC = 9817,											// ���Ż�����Ϣ
		MT_SYNC_GUILD_INFO_SC_RESERVED = 9818,									// ͬ��������Ϣ ����
		MT_APPLY_FOR_JOIN_GUILD_ACK_SC = 9819,									// �ظ���������֪ͨ
		MT_APPLY_FOR_JOIN_GUILD_LIST_SC = 9820,									// �ѷ�����ľ����б���
		MT_GUILD_MEMBER_SOS_SC = 9821,											// ���ų�Ա���֪ͨ
		MT_GUILD_INVITE_LIST_SC = 9822,											// ���������б�
		MT_GUILD_RESET_NAME_SC = 9823,											// ���Ÿ���
		MT_GUILD_CHECK_CAN_DELATE_ACK_SC = 9824,								// �����ܷ����ظ�
		MT_GUILD_OPERA_SUCC_SC = 9825,											// ���Ų����ɹ�����
		MT_GUILD_ROLE_GUILD_INFO_SC = 9826,										// ��ɫ�ľ�����Ϣ
		MT_GUILD_CUR_MAX_MEMBER_COUNT_SC = 9827,								// ���ŵ�ǰ����Ա����
		MT_GUILD_ROLE_GUILD_PAPER_INFO_SC = 9828,								// ��ɫ�Ĺ�������Ϣ
		MT_GUILD_ROLE_GUILD_PAPER_CREATE_CS = 9829,								// ��ɫ���󴴽����
		MT_GUILD_ROLE_GUILD_PAPER_SINGAL_CHAT_CS = 9830,						// ˽���к��δ�ɷ������
		MT_GUILD_MAZE_OPERATE_CS = 9831,										// �����Թ���������
		MT_GUILD_MEMBER_MAZE_INFO_SC = 9832,									// �����Ա�Թ���Ϣ
		MT_GUILD_MAZE_RANK_INFO_SC = 9833,										// �����Թ�������Ϣ
		MT_GUILD_SIGNIN_REQ_CS = 9834,											// ����ǩ������
		MT_GUILD_SIGNIN_ALL_INFO_SC = 9835,										// ����ǩ��������Ϣ
		MT_GUILD_CHANGE_AVATAR_CS = 9836,										// ����ͷ�����
		MT_GUILD_APPEARANCE_INFO_SC = 9837,										// ���������Ϣ
		MT_GUILD_BIAOCHE_OPERA_CS = 9838,										// �����ڳ�����

		//////////////////////////////////////////////// ����boss /////////////////////////////////////////////////////////
		MT_WORLD_BOSS_INFO_TO_ALL_SC = 10100,									// ��������boss�㲥��Ϣ
		MT_WORLDBOSS_INFO_SC = 10101,											// ��������boss��Ϣ
		MT_WORLDBOSS_BORN_SC = 10102,											// ��������boss����
		MT_SEND_WORLDBOSS_PERSONAL_HURT_INFO_SC = 10103,						// ��������boss�����˺�������Ϣ
		MT_SEND_WORLDBOSS_GUILD_HURT_INFO_SC = 10104,							// ��������boss�����˺�������Ϣ
		MT_WORLDBOSS_WEEK_RANK_SC = 10105,										// ���������ܰ���Ϣ
		MT_WORLDBOSS_CAN_ROLL_SC = 10106,										// ����boss���ܱ��ƣ���ҡ��
		MT_WORLDBOSS_ROLL_INFO_SC = 10107,										// �������ҡ����
		MT_WORLDBOSS_TOP_ROLL_INFO_SC = 10108,									// ����������ҡ����
		MT_WORLDBOSS_KILLER_LIST_SC = 10109,									// ����boss��ɱ�б�
		MT_WORLDBOSS_HP_INFO_SC = 10111,										// bossѪ����Ϣ

		MT_WORLDBOSS_KILLER_INFO_CS = 10150,									// ��������boss��ɱ�б�
		MT_GET_WORLDBOSS_INFO_CS = 10151,										// ��������boss��Ϣ
		MT_GET_WORLDBOSS_PRESONAL_HURT_INFO_CS = 10152,							// ��������boss�����˺�������Ϣ
		MT_GET_WORLDBOSS_GUILD_HURT_INFO_CS = 10153,							// ��������boss�����˺�������Ϣ
		MT_GET_WORLDBOSS_WEEK_RANK_INFO_CS = 10154,								// ��������boss��ɱ�����ܰ���Ϣ
		MT_GET_WORLDBOSS_ROLL_REQ_CS = 10155,									// ����ҡ��

		//////////////////////////////////////////////// ��Ӫ /////////////////////////////////////////////////////////
		MT_CAMP_INFO_SC = 10200,												// ��Ӫ��Ϣ����

		MT_CAMP_INFO_CS = 10250,												// ��ȡ��Ӫ��Ϣ
	};
}

#endif // __GLOBALMSGCODE_H__


