#ifndef MSGNPC_H
#define MSGNPC_H

#include "servercommon/userprotocal/msgheader.h"
#include "gamedef.h"
#include "servercommon/taskdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	class CSTaskListReq	 // �����ѽ������б�
	{
	public:
		CSTaskListReq();
		MessageHeader	header;
	};

	class CSTaskGiveup
	{
	public:
		CSTaskGiveup();
		MessageHeader	header;
		int				taskid;
	};

	class CSTaskAccept	// 1853
	{
	public:
		CSTaskAccept();
		MessageHeader	header;

		int				taskid;
	};

	class CSTaskCommit
	{
	public:
		CSTaskCommit();
		MessageHeader	header;

		int				taskid;
	};

	class CSFlyByShoe	 // ʹ��С��Ь
	{
	public:
		CSFlyByShoe();
		MessageHeader	header;

		int				scene_id;
		int				scene_key;
		short			pos_x;
		short			pos_y;
		short			is_auto_buy;
		short			is_force;
	};

	class CSSyncJump	 
	{
	public:
		CSSyncJump();
		MessageHeader	header;

		int				scene_id;
		int				scene_key;
		short			pos_x;
		short			pos_y;
	};

	class CSTaskTalkToNpc	// 1855 ��npc�Ի�
	{
	public:
		CSTaskTalkToNpc();
		MessageHeader	header;

		UInt16 npc_id;
		short reserve_sh;
	};

	class SCTaskListAck				// �����ѽ������б�Ż�
	{
	public:
		SCTaskListAck();
		MessageHeader	header;

		struct TaskItem
		{
			unsigned short task_id;
			char task_ver;
			char task_condition;
			short progress_num;
			char is_complete;
			char reserve_ch;
			unsigned int accept_time;
		};

		int				count;
		TaskItem		taskinfo_list[TaskParam::MAX_ACCEPTED_TASK_NUM];
	};

	class SCTaskInfo				// �����б�ı�
	{
	public:
		SCTaskInfo();
		MessageHeader	header;

		enum REASON
		{
			REASON_INFO_CHG = 0,	// ��Ϣ�ı�
			REASON_ACCEPT,			// ��ȡ
			REASON_REMOVE,			// �Ƴ� 
		};

		short			reason;
		TaskID			task_id;

		char			is_complete;
		char			is_silent;
		short			param;

		unsigned int	accept_time;
	};

	class SCTaskAccpetableTaskList				// 1808 �ɽ������б�
	{
	public:
		SCTaskAccpetableTaskList();
		MessageHeader	header;

		const static int ACCEPTABLE_TASK_MAX_COUNT = 2048;

		int				count;
		TaskID			accpetable_task_list[ACCEPTABLE_TASK_MAX_COUNT];
	};

	#define MAX_TASK_RECORDER_INFO_NUM 2048
	class SCTaskRecorderList		//��������������б�
	{
	public:
		SCTaskRecorderList();
		MessageHeader	header;
		unsigned int	count;
		TaskID			task_recorder_info_item[MAX_TASK_RECORDER_INFO_NUM];
	};

	class SCTaskRecorderInfo		// �����¼�б����ݸı�
	{
	public:
		SCTaskRecorderInfo();
		MessageHeader	header;
		TaskID			completed_taskid;
	};

	/////////////////////////////////////////////////////    ����ͨ��Э��   /////////////////////////////////////////////////////

	/////////////////////////////////////////////////////   ��ɱ����   /////////////////////////////////////////////////////
	class CSCishaTaskOpera
	{
	public:
		enum OPERA_TYPE
		{
			OPERA_TYPE_GET_INFO = 0,
			OPERA_TYPE_ACCEPT_TASK = 1,

			OPERA_TYPE_MAX,
		};

		CSCishaTaskOpera();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
	};

	class SCCishaTaskInfo
	{
	public:
		SCCishaTaskInfo();
		MessageHeader header;

		TaskID task_id;
		short commit_times;
	};

	/////////////////////////////////////////////////////  (�ճ�)��ħ����  /////////////////////////////////////////////////////
	enum TUMO_OPERA_TYPE
	{
		TUMO_OPERA_TYPE_GET_INFO = 0,						// ��ȡ������Ϣ
		TUMO_OPERA_TYPE_FETCH_COMPLETE_ALL_REWARD = 1,		// ��ȡȫ����ɽ���
		TUMO_OPERA_TYPE_COMMIT_TASK = 2,					// �ύ���� p1:����id p2: 1 �ύȫ�� 0 ����
		TUMO_OPERA_TYPE_FETCH_REWARD = 3,					// ��ȡ���� p1:�������� p2: 1 ��ͨ 2 ˫�� 3 ����
		TUMO_OPERA_TYPE_FETCH_EXP_BALL_REWARD = 4,			// ��ȡ�������� 

		TUMO_OPERA_TYPE_MAX,
	};

	class CSTuMoTaskOpera		// 1856 �ճ�����ͨ������
	{
	public:
		CSTuMoTaskOpera();
		MessageHeader header;

		short opera_type;
		unsigned short param_1;
		short param_2;
		short param_3;
	};

	enum TUMO_NOTIFY_REASON_TYPE
	{
		TUMO_NOTIFY_REASON_DEFALUT = 0,										// ��ħĬ��֪ͨ����

		TUMO_NOTIFY_REASON_ADD_TASK = 1,									// ��������
		TUMO_NOTIFY_REASON_REMOVE_TASK = 2,									// �Ƴ�����
		TUMO_NOTIFY_REASON_EXP_BALL_CHANGE = 3,								// ���������ݸı�
		TUMO_NOTIFY_REASON_ONE_KEY_FINISH = 4,								// һ�����

	};
	class SCTuMoTaskInfo	// 1805 �ճ�������Ϣ
	{
	public:
		SCTuMoTaskInfo();
		MessageHeader header;

		short notify_reason;												// ֪ͨԭ��
		short commit_times;													// �ύ�������
		char is_accept;														// �����Ƿ��Ѿ�������
		char has_fetch_complete_all_reward;									// �Ƿ���ȡ�������������Ľ���
		TaskID task_id;														// ����id
		short daily_task_can_fetch_flag;									// �ճ��������ȡ���
		short daily_task_fetch_flag;										// �ճ�������ȡ���
		unsigned int daily_task_exp_ball_valid_time;						// ������ʧЧʱ��
		long long daily_task_exp_ball_exp;									// ������ǰ����
	};

	/////////////////////////////////////////////////////  �ܻ�����  /////////////////////////////////////////////////////

	class CSPaoHuanSkipTask
	{
	public:
		CSPaoHuanSkipTask();
		MessageHeader header;

		short skip_all;
		TaskID task_id;
		int reserve;
	};

	class CSGetPaoHuanTaskInfo
	{
	public:
		CSGetPaoHuanTaskInfo();
		MessageHeader header;
	};

	enum PAOHUAN_NOTIFY_REASON_TYPE
	{
		PAOHUAN_NOTIFY_REASON_DEFALUT = 0,									// �ܻ�֪ͨ����
		PAOHUAN_NOTIFY_REASON_ADD_TASK = 1,									// ��������
		PAOHUAN_NOTIFY_REASON_REMOVE_TASK = 2,								// �Ƴ�����
		PAOHUAN_NOTIFY_REASON_FETCH_REWARD = 3,								// ������־�ı�

	};
	class SCPaohuanTaskInfo
	{
	public:
		SCPaohuanTaskInfo();
		MessageHeader header;

		short notify_reason;												// ֪ͨԭ��
		short complete_times;												// �ύ�������
		short fetch_phase_reward_count;										// ��ȡ�׶ν���
		short is_accept;													// �����Ƿ��Ѿ�������
		TaskID task_id;														// ����id
	};

	/////////////////////////////////////////////////////  ��������  /////////////////////////////////////////////////////

	class CSRefreshHusongTask												// ��������Ϣ
	{
	public:
		CSRefreshHusongTask();
		MessageHeader header;

		char is_autoflush;													// �Ƿ��Զ�ˢ��
		char is_auto_buy;													// �Ƿ��Զ�����
		short reserve_sh;													// ����
		int to_color;														// ˢ�µ�����ɫ
	};

	class CSHusongBuyTimes
	{
	public:
		CSHusongBuyTimes();
		MessageHeader header;
	};

	enum HUSONG_INFO_NOTFIY_REASON
	{
		HUSONG_INFO_NOTFIY_REASON_DEFAULT = 0,
		HUSONG_INFO_NOTFIY_REASON_ADD_TASK,									// ��������
		HUSONG_INFO_NOTFIY_REASON_GIVEUP_TASK,								// ����ʧ��
		HUSONG_INFO_NOTIFY_REASON_COMMIT_TASK,								// �������
		HUSONG_INFO_NOTFIY_REASON_FLUSHCOLOR,								// ˢ����ɫ
		HUSONG_INFO_NOTFIY_REASON_LITTLE_HELPER_ADD_TASK,					// С���ֽ�������
		HUSONG_INFO_NOTFIY_REASON_LITTLE_HELPER_COMMIT_TASK,				// С�����������

		HUSONG_INFO_NOTFIY_REASON_MAX,
	};

	class SCHusongInfo
	{
	public:
		SCHusongInfo();
		MessageHeader header;
		
		short notfiy_reason;												// ֪ͨԭ��
		ObjID obj_id;														// ����ID

		TaskID task_id;														// ����id
		char task_color;													// ������ɫ
		char accept_in_activityTime;										// �ڻʱ���ڽ�
		char is_use_hudun;													// �Ƿ�ʹ�ù�����
		char reserve_1;
		short reserve_2;
	};

	class CSHusongAddShield													// 1874�ͷŻ���
	{
	public:
		CSHusongAddShield();
		MessageHeader		header;
	};

	class SCHusongConsumeInfo
	{
	public:
		SCHusongConsumeInfo();
		MessageHeader header;

		int token_num;
		int gold_num;
		int bind_gold_num;
	};

	/////////////////////////////////////////////////////    ��������   /////////////////////////////////////////////////////
	class SCGuildTaskInfo
	{
	public:
		SCGuildTaskInfo();
		MessageHeader header;

		char double_reward_flag;
		char is_finish;
		short complete_task_count;
		int guild_task_max_count;
	};

	class CSGetGuildTaskDoubleReward
	{
	public:
		CSGetGuildTaskDoubleReward();
		MessageHeader header;
	};

	class CSFinishAllGuildTask
	{
	public:
		CSFinishAllGuildTask();
		MessageHeader header;
	};

	/////////////////////////////////////////////////////    ��������   /////////////////////////////////////////////////////
	class CSForceFinishSpecialTask		// ָ�����ĳ����������  1878
	{
	public:
		CSForceFinishSpecialTask();
		MessageHeader header;
		int special_task_type;
	};
	//////////////////////////////////////////////////// /////////////////////////////////////////////////////
	static const int MAX_TASK_RANDOM_NUM = 20;

	class SCTaskRollReward
	{
	public:
		SCTaskRollReward();
		MessageHeader header;
		
		struct RewardItem
		{
			int task_type;
			int task_index;
		};

		short task_type;
		short is_one_key_finish;
		int param_1;
		long long param_2;

		int count;
		RewardItem reward_item_list[MAX_TASK_RANDOM_NUM];
	};
}

#pragma pack(pop)

#endif

